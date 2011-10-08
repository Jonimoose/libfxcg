int RTC_Elapsed_ms( int start_value, int duration_in_ms );
int RTC_GetTicks(void);
int RTC_Reset( int );
void RTC_GetTime( unsigned int*hour, unsigned int*minute, unsigned int*second, unsigned int*millisecond );
void RTC_SetDateTime( unsigned char*timestr );
