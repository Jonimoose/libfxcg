#ifndef _FXCG_SERIAL_H
#define _FXCG_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

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

// higher-level link functions. used to transfer files between calculators

struct TTransmitBuffer {
    char device[0x008]; // fls0
    char directoryname[0x10A];
// 0x112
    char fname1[0x10A];
    char dummy[0x28];
    char fname2[0x00E];
// 0x252
    unsigned short filename[0x10A];
// 0x466
    char dummy2[0x1A];
// 0x480
    int filesize;
// 0x484
    short dummy3;
// 0x486
    char command;
    char subcommand;
// 0x488
    char datatype;
    char dummy4_3;
    char dummy4_4;
    char dummy4_5;
// 0x48C
    int handle;
// 0x490
    char dummy5[8];
// 0x498
    char source;
    char dummy6_1;
    char dummy6_2;
    char dummy6_3;
// 0x49C
    int zero;
};

int App_LINK_GetReceiveTimeout_ms( void );
void App_LINK_SetReceiveTimeout_ms( int timeout );
int Comm_Open( unsigned short parameters );
int Comm_Close( int mode );
int Comm_TryCheckPacket( unsigned char subtype );
int Comm_Terminate( unsigned char subtype );
int App_LINK_SetRemoteBaud( void ); //Switch the remote calculator to 115200 baud, no parity and 1 stop bit (command '02'; (fxReverse.PDF, p. 17)). Close the serial interface. Open the serial interface with 115200 baud, no parity and 1 stop bit.
int App_LINK_Send_ST9_Packet( void );
int App_LINK_GetDeviceInfo( unsigned int* calcType, unsigned short* osVer);
int App_LINK_TransmitInit( struct TTransmitBuffer*ttb );
int App_LINK_Transmit( struct TTransmitBuffer*ttb );


#ifdef __cplusplus
}
#endif

#endif
