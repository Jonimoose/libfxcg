#ifndef _FXCG_USB_H
#define _FXCG_USB_H

#ifdef __cplusplus
extern "C" {
#endif

int USB_Open(short param_1);
int USB_IsOpen(void);
int USB_Close(void);

int USB_Read(unsigned char *out, int sz, short *count);
int USB_ReadSingle(unsigned char *out);
int USB_Peek(int idx, unsigned char *out);
int USB_PollRX(void);
int USB_ClearRX(void);

int USB_Write(const unsigned char *buf, int count);
int USB_WriteSingle(unsigned char x);
int USB_PollTX(void);
int USB_ClearTX(void);

#ifdef __cplusplus
}
#endif

#endif
