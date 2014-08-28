#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/heap.h>
#include <fxcg/serial.h>

#include <alloca.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Unspecified members initialized to zero.
FILE _impl_stdin = {0};
FILE _impl_stdout = {1};
FILE _impl_stderr = {2};

enum {
    SYSFILE_MODE_READ = 0,
    SYSFILE_MODE_WRITE = 2,
    SYSFILE_MODE_READWRITE = 3
};

#define IOERR(stream, err) errno = err; stream->error = 1

static inline int isstdstream(FILE *f) {
    return f->fileno < 3;
}

static inline int handle_tonative(int fileno) {
    return fileno - 3;
}

int feof(FILE *stream) {
    return stream->eof;
}

FILE *fopen(const char *path, const char *mode) {
    // Resolve mode
    int sysmode = 0;
    if (mode[0] == 'r') {
        sysmode = SYSFILE_MODE_READ;
    } else if (mode[0] == 'w' || mode[0] == 'a') { 
        sysmode = SYSFILE_MODE_WRITE;
    } else if (strchr(mode, '+')) {
        sysmode = SYSFILE_MODE_READWRITE;
    } else {
        errno = EINVAL;
        return NULL;
    }

    // Convert string to system native
    size_t plen = strlen(path);
    // We have several potential exits, so alloca simplifies ensuring this
    // gets freed (an early revision of the following logic had memory leaks).
    unsigned short *chars16 = alloca(2 * (plen + 1));
    if (chars16 == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // Get a handle from the system
    Bfile_StrToName_ncpy(chars16, path, plen);
    int syshandle = Bfile_OpenFile_OS(chars16, sysmode, 0);

    if (syshandle < 0) {
        if (sysmode == SYSFILE_MODE_WRITE ||
            sysmode == SYSFILE_MODE_READWRITE) {
            // Doesn't exist, want to write. Create.
            // FIXME Size 0 doesn't work?
            size_t size = 10;
            switch (Bfile_CreateEntry_OS(chars16, CREATEMODE_FILE, &size)) {
            case 0:
                // Success. Open file.
                syshandle = Bfile_OpenFile_OS(chars16, sysmode, 0);
                break;
            case -13:
                errno = EEXIST;
                return NULL;
            case -3:
                errno = ENOENT;
                return NULL;
            default:
                errno = EIO;
                return NULL;
            }
        } else {
            // Doesn't exist for reading.
            errno = ENOENT;
            return NULL;
        }
    }

    if (mode[0] == 'a') {
        // TODO Need to seek to end and do some junk.
        errno = ENOTIMPL;
        return NULL;
    }

    // Create a FILE and fill it in
    FILE *f = sys_malloc(sizeof(FILE));
    if (f == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    memset(f, 0, sizeof(FILE));
    f->fileno = syshandle + 3;
    return f;
}

// TODO implement me.
// Same use-case in Python as dup(), so not necessary.  Just fail.
FILE *fdopen(int fd, const char *mode) {
    return NULL;
}

int fclose(FILE *fp) {
    if (!isstdstream(fp)) {
        // TODO Not sure what this can return.  Assuming never fails.
        Bfile_CloseFile_OS(handle_tonative(fp->fileno));
        sys_free(fp);
    } else {
        // std streams can't actually be closed.
    }
    return 0;
}

static int serial_ensureopen() {
    if (Serial_IsOpen() != 1) {
        unsigned char mode[6] = {0, 5, 0, 0, 0, 0};    // 9600 bps 8n1
        if (Serial_Open(mode) != 0) {
            return 1;
        }
    }
    return 0;
}

// TODO restrict ptr, stream
static size_t fwrite_serial(const void *ptr, size_t size, size_t nitems,
                            FILE *stream) {
    // tx buffer is 256 bytes, don't get stuck waiting
    if (size > 256 || serial_ensureopen()) {
        IOERR(stream, EIO);
        return 0;
    }

    // Transmit
    size_t remain = nitems;
    while (remain > 0) {
        if (Serial_PollTX() > size) {
            Serial_Write(ptr, size);
            remain--;
        } else {
            // Wait for space in tx buffer
            // TODO sleep for an interrupt or something
        }
    }
    return nitems - remain;
}

static size_t fwrite_term(const void *ptr, size_t size, size_t nitems,
                          FILE *stream) {
    char *buffer = sys_malloc(1 + nitems * size);
    if (buffer == NULL) {
        IOERR(stream, ENOMEM);
        return 0;
    }

    memcpy(buffer, ptr, nitems * size);
    buffer[nitems * size] = '\0';
    const char *outp = buffer;
    char *eol;

    // Loop over all lines in buffer, terminate once we've printed all lines.
    do {
        eol = strchr(outp, '\n');
        if(eol) {
          *eol = '\0';
        }

        // Cast to wider type for correct pointers
        int termx = stream->termx, termy = stream->termy;
        PrintMiniMini(&termx, &termy, outp, 0, TEXT_COLOR_BLACK, 0);

        // CR/LF if applicable
        if(eol)
        {
            stream->termx = 0;
            stream->termy += 10;
            outp = eol + 1;
        }
    } while (eol);

    sys_free(buffer);
    return nitems;
}
// TODO make ptr, stream restrict for optimization
size_t fwrite(const void *ptr, size_t size, size_t nitems,
              FILE *stream) {
    if (isstdstream(stream)) {
        if (stream->fileno == 2) {
            // stderr: serial
            return fwrite_serial(ptr, size, nitems, stream);
        } else if (stream->fileno == 1) {
            // stdout: display
            return fwrite_term(ptr, size, nitems, stream);
        } else {
            // stdin..?
        }
    }
    // TODO this must be able to fail, but how?
    Bfile_WriteFile_OS(handle_tonative(stream->fileno), ptr, size * nitems);
    return nitems;
}

size_t fread_serial(void *buffer, size_t size, size_t count, FILE *stream) {
    short bytes;
    size_t read = 0;
    // Don't get stuck waiting for a block to come in (buffer is 256 bytes)
    if (size > 256 || serial_ensureopen()) {
        IOERR(stream, EIO);
        return 0;
    }

    while (count-- > 0) {
        while (Serial_PollRX() < size);
        Serial_Read(buffer, size, &bytes);
        buffer += size;
        read++;
    }
    return read;
}

size_t fread(void *buffer, size_t size, size_t count, FILE *stream) {
    size_t n = size * count;
    if (isstdstream(stream)) {
        if (stream->fileno == 0) {
            // stdin
            return fread_serial(buffer, size, n, stream);
        } else {
            // Reading stdout or stderr? No.
            return EOF;
        }
    }

    // TODO failure modes unknown
    size_t ret = Bfile_ReadFile_OS(handle_tonative(stream->fileno), buffer, n, -1);
    if (ret < 0) {
        stream->error = 1;
    } else if (ret < n) {
        stream->eof = 1;
    }
    return ret;
}

int fputc(int c, FILE *stream) {
    unsigned char uc = (unsigned char)c;
    if (fwrite(&uc, 1, 1, stream) != 1)
        return EOF;
    return uc;
}

int putchar(int c) {
    return fputc(c, stdout);
}

int fputs(const char *s, FILE *stream) {
    size_t len = strlen(s);
    if (fwrite(s, 1, len, stdout) != len)
        return EOF;
    return 0;
}

int puts(const char *s) {
    int ret = fputs(s, stdout);
    putchar('\n');
    return ret;
}

char *fgets(char *s, int n, FILE *stream) {
    unsigned char c;
    char *s_in = s;
    int count = 0;
    if (n < 1)
        return NULL;

    do {
        if (fread(&c, 1, 1, stream) != 1)
            return NULL;
        *s++ = c;
        count++;
    } while (c != '\n' && count < n && c != EOF);

    if (count < n) // Back up unless didn't consume a terminator
        s--;
    *s = 0;
    return s_in;
}

int fgetc(FILE *f) {
    unsigned char c;
    fread(&c, 1, 1, f);
    return c;
}

int ungetc(int c, FILE *f) {
    if (c == EOF || f->has_unput)
        return EOF;
    f->unput = (char)c;
    f->has_unput = 1;
    return f->unput;
}

int fseek(FILE *f, long offset, int whence) {
    if (isstdstream(f)) {
        IOERR(f, ERANGE);
        return -1;
    }

    switch (whence) {
        case SEEK_CUR:
            offset = ftell(f) + offset;
            break;
        case SEEK_END:
            // TODO this probably doesn't work. Wild guessing.
            // Speculation says we have Bfile_GetFileSize_OS
            offset = INT_MAX;
            break;
        case SEEK_SET:
            break;
        default:
            return -1;
    }

    int fileno = handle_tonative(f->fileno);
    // TODO can this fail? Probably.
    Bfile_SeekFile_OS(fileno, (int)offset);

    f->error = 0;
    return 0;
}

long ftell(FILE *f) {
    return Bfile_TellFile_OS(handle_tonative(f->fileno));
}

int ferror(FILE *f) {
    return f->error;
}

void perror(const char *s) {
    if (s != NULL && strlen(s) > 0) {
        fprintf(stderr, "(%s): ", s);
    }
    fprintf(stderr, "code %i (xref errno.h)\n", -errno);
}

int remove(const char *name) {
    uint16_t *widepath = alloca(2 * strlen(name) + 1);
    Bfile_StrToName_ncpy(widepath, name, strlen(name));

    int ret = Bfile_DeleteEntry(widepath);
    if (ret >= 0) {
        return 0;
    } else {
        switch (ret) {
        case -1:
        case -3:
        case -5:
            errno = ENOENT;
            break;
        default:
            errno = EIO;
        }
        return 1;
    }
}

int rename(const char *old, const char *new) {
    uint16_t *wideold = alloca(2 * strlen(old) + 1);
    Bfile_StrToName_ncpy(wideold, old, strlen(old));
    uint16_t *widenew = alloca(2 * strlen(new) + 1);
    Bfile_StrToName_ncpy(widenew, new, strlen(new));

    int ret = Bfile_RenameEntry(wideold, widenew);
    if (ret >= 0) {
        return 0;
    } else {
        // Unknown meanings. Fail with success.
        errno = 0;
        return 1;
    }
}

int mkdir(const char *path, unsigned mode) {
    uint16_t *widepath = alloca(2 * strlen(path) + 1);
    Bfile_StrToName_ncpy(widepath, path, strlen(path));

    int ret = Bfile_CreateEntry_OS(widepath, CREATEMODE_FOLDER, 0);
    if (ret >= 0) {
        return 0;
    } else {
        if (ret == -3) {
            errno = EEXIST;
        } else {
            // Unknown meaning. Fail with success.
            errno = 0;
        }
        return 1;
    }
}

