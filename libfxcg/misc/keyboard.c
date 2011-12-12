#include "fxcg_syscalls.h"

int PRGM_GetKey() {
    unsigned char buffer[12];
    PRGM_GetKey_OS( buffer );
    return (( buffer[1] - 1) | ( buffer[2]  >> 4 ));
}
