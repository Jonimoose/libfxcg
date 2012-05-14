#include <errno.h>
#include <stdio.h>
#include <string.h>

FILE _impl_stdin = {0, 0, 0};
FILE _impl_stdout = {1, 0, 0};
FILE _impl_stderr = {2, 0, 0};

static int isstdstream(FILE *f) {
    return f->fileno < 3;
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
    unsigned short *chars16 = malloc(2 * (plen + 1));
    if (chars16 == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // Get a handle from the system
    Bfile_StrToName_ncpy(chars16, path, plen);
    int syshandle = Bfile_OpenFile_OS(chars16, sysmode);
    free(chars16);  // No longer needed
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
    FILE *f = malloc(sizeof(FILE));
    if (f == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    f->fileno = syshandle + 3;
    f->error = 0;
    f->has_unput = 0;
    return f;
}

int fclose(FILE *fp) {
    if (!isstdstream(fp)) {
        // TODO Not sure what this can return.  Assuming never fails.
        Bfile_CloseFile_OS(fp->fileno - 3);
        free(fp);
    } else {
        // std streams are just serial I/O.  Do nothing.
    }
    return 0;
}

static size_t fwrite_serial(const void *restrict ptr, size_t size, size_t nitems,
                            FILE *restrict stream) {
    if (size > 256) {   // tx buffer is 256 bytes, don't get stuck waiting
        errno = EIO;
        return 0;
    }

    // Init comms if necessary
    if (Serial_IsOpen() != 1) { // syscall 0x1bc6
        unsigned char mode[6] = {0, 5, 0, 0, 0, 0};    // 9600 bps 8n1
        if (Serial_Open(mode) != 0) {   // syscall 0x1bb7
            stream->error = 1;
            errno = EIO;
            return 0;
        }
    }

    // Transmit
    size_t remain = nitems;
    while (remain > 0) {
        if (Serial_GetFreeTransmitSpace() > size) {
            Serial_BufferedTransmitNBytes(ptr, size);
            remain--;
        } else {
            // Wait for space in tx buffer
            // TODO sleep for an interrupt or something
        }
    }
    return nitems - remain;
}

size_t fwrite(const void *restrict ptr, size_t size, size_t nitems,
              FILE *restrict stream) {
    if (isstdsteam(stream)) {
        return fwrite_serial(ptr, size, nitems, stream);
    }
    // TODO this must be able to fail, but how?
    Bfile_WriteFile_OS(stream->fileno - 3, ptr, size * nitems);
    return nitems;
}


