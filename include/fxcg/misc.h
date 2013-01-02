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

#endif

#ifdef __cplusplus
}
#endif
