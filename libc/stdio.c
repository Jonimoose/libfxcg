#include <fxcg/file.h>
#include <fxcg/heap.h>
#include <fxcg/serial.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

FILE _impl_stdin = {0, 0, 0};
FILE _impl_stdout = {1, 0, 0};
FILE _impl_stderr = {2, 0, 0};

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
        sysmode = 0;   // READ
    } else if (mode[0] == 'w' || mode[0] == 'a') { 
        sysmode = 2;   // WRITE
    } else if (strchr(mode, '+')) {
        sysmode = 3;   // READWRITE
    } else {
        errno = EINVAL;
        return NULL;
    }

    // Convert string to system native
    size_t plen = strlen(path);
    unsigned short *chars16 = sys_malloc(2 * (plen + 1));
    if (chars16 == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // Get a handle from the system
    Bfile_StrToName_ncpy(chars16, (unsigned char *)path, plen);
    int syshandle = Bfile_OpenFile_OS(chars16, sysmode);
    sys_free(chars16);  // No longer needed
    if (syshandle < 0) {
        // Failure codes here aren't documented.  Let's just fail with
        // errno = success, then.
        errno = 0;
        return NULL;
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
    f->fileno = syshandle + 3;
    // TODO reenable when dup() works
    /*if (_dtable_register(f->fileno)) {
        errno = ENFILE;
        return NULL;
    }*/
    f->error = 0;
    f->has_unput = 0;
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
        // TODO reenable when dup() does something
        //_dtable_unregister(fp->fileno);
        sys_free(fp);
    } else {
        // std streams are just serial I/O.  Do nothing.
    }
    return 0;
}

static int serial_ensureopen() {
    if (Serial_IsOpen() != 1) { // syscall 0x1bc6
        unsigned char mode[6] = {0, 5, 0, 0, 0, 0};    // 9600 bps 8n1
        if (Serial_Open(mode) != 0) {   // syscall 0x1bb7
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

// TODO make ptr, stream restrict for optimization
size_t fwrite(const void *ptr, size_t size, size_t nitems,
              FILE *stream) {
    if (isstdstream(stream)) {
        return fwrite_serial(ptr, size, nitems, stream);
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
        return fread_serial(buffer, size, n, stream);
    }

    // TODO failure modes unknown
    Bfile_ReadFile_OS(handle_tonative(stream->fileno), buffer, n, 0);
    return n;
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

void perror(const char *s) {
    if (s != NULL && strlen(s) > 0) {
        fprintf(stderr, "(%s): ", s);
    }
    fprintf(stderr, "code %i (xref errno.h)\n", -errno);
}
