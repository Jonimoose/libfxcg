#ifndef __FXCG_APP_H
#define __FXCG_APP_H

#ifdef __cplusplus
extern "C" {
#endif

void APP_FINANCE( int, int );
void APP_SYSTEM_BATTERY( int opt );
void APP_SYSTEM_DISPLAY( int );
void APP_SYSTEM_LANGUAGE( int );
void APP_SYSTEM_POWER( int opt );
void APP_SYSTEM_RESET( void );
void APP_SYSTEM_VERSION( void );
void APP_SYSTEM( void );
void APP_RUNMAT(int, int);

void ResetAllDialog( void );
unsigned char*GetAppName( unsigned char*name );

void App_InitDlgDescriptor( void*P1, unsigned char P2 );
void APP_LINK_transmit_select_dialog( void*buffer1, void*buffer2 );

#ifdef __cplusplus
}
#endif

#endif
