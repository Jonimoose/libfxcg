#ifdef __cplusplus
extern "C" {
#endif 

#ifndef _MISC_H_
#define _MISC_H_

#include <stddef.h>

// Don't know of anybody using these
// gbl08ma: but I do know, at least itoa is widely used. And since they're syscalls, their headers should be defined somewhere. Keeping this here.
int ItoA_10digit( int, void* );
void ByteToHex( unsigned char value, unsigned char*result );
void HexToByte( unsigned char*value, unsigned char*result );
void HexToNibble( unsigned char value, unsigned char*result );
void HexToWord( unsigned char*value, unsigned short*result );
void itoa( int value, unsigned char*result );
void LongToAscHex( int, unsigned char*, int );
void NibbleToHex( unsigned char value, unsigned char*result );
void WordToHex( unsigned short value, unsigned char*result );

#ifdef USE_FXCG_STDLIB
//Not sure if these should go into their proper headers or be deleted. We already have our own implementations of some of these functions, it seems.
#define atoi sys_atoi
#define rand sys_rand
#define srand sys_rand
#define malloc sys_malloc
#define calloc sys_calloc
#define realloc sys_realloc
#define free sys_free
#define memcmp sys_memcmp
#define memcmp3 sys_memcmp3
#define memcpy sys_memcpy
#define memset sys_memset
#define memmove sys_memmove
#define strlen sys_strlen
#define strcpy sys_strcpy
#define strncpy sys_strncpy
#define strchr sys_strchr
#define strcmp sys_strcmp
#define strcat sys_strcat

#endif

#endif

#ifdef __cplusplus
}
#endif
