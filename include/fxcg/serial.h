#ifndef _FXCG_SERIAL_H
#define _FXCG_SERIAL_H

int Serial_Open(unsigned char *mode);
int Serial_IsOpen(void);
int Serial_Close(int mode);

int Serial_Read(unsigned char *out, int sz, short *count);
int Serial_ReadSingle(unsigned char *out);
int Serial_Peek(int idx, unsigned char *out);
int Serial_PollRX(void);
int Serial_ClearRX(void);

int Serial_Write(const unsigned char *buf, int count);
int Serial_WriteSingle(unsigned char x);
int Serial_WriteUnbuffered(unsigned char x);
int Serial_PollTX(void);
int Serial_ClearTX(void);

#ifdef _FXCG_MINICOMPAT

#define Serial_ReadNBytes Serial_Read
#define Serial_ReadOneByte Serial_ReadSingle
#define Serial_SpyNthByte Serial_Peek
#define Serial_GetReceivedBytesAvailable Serial_PollRX
#define Serial_ClearReceiveBuffer Serial_ClearRX

#define Serial_BufferedTransmitNBytes Serial_Write
#define Serial_BufferedTransmitOneByte Serial_WriteSingle
#define Serial_DirectTransmitOneByte Serial_WriteUnbuffered
#define Serial_GetFreeTransmitSpace Serial_PollTX
#define Serial_ClearTransmitBuffer Serial_ClearTX

#endif

#endif
