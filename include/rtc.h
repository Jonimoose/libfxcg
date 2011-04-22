int RTC_Reset( int );
void RTC_GetTime( unsigned int*hour, unsigned int*minute, unsigned int*second, unsigned int*millisecond );
int RTC_GetTicks(void);
int RTC_Elapsed_ms( int start_value, int duration_in_ms );
void RTC_SetDateTime( unsigned char*timestr );
