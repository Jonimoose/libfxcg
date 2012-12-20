#ifndef _FXCG_RTC_H
#define _FXCG_RTC_H

#ifdef __cplusplus
extern "C" {
#endif
//RTC and time-related syscalls

//Control the RTC:
int RTC_Reset( int );
void RTC_SetDateTime( unsigned char*timestr );
//Get information from/based on the RTC:
void RTC_GetTime( unsigned int*hour, unsigned int*minute, unsigned int*second, unsigned int*millisecond );
int RTC_GetTicks(void);
int RTC_Elapsed_ms( int start_value, int duration_in_ms );


#ifdef __cplusplus
}
#endif

#endif
