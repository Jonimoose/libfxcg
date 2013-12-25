//Gcc supports builtin in byte swap functions that use SH's swap instruction use that

#define bswap_16        __builtin_bswap16
#define bswap_32        __builtin_bswap32
#define bswap_64        __builtin_bswap64
