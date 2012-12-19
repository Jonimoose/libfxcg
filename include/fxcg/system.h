#ifdef __cplusplus
extern "C" {
#endif

// This adds in syscalls for interfacing with the OS and hardware.

void SetAutoPowerOffTime(int durationInMinutes);
int GetAutoPowerOffTime(); //returns duration in minutes

void SetBacklightDuration(char durationInHalfMinutes);
char GetBacklightDuration(); //returns duration in half-minutes

void SetBatteryType(int type);
int GetBatteryType(void);

int GetMainBatteryVoltage(int one); //parameter should be 1
void PowerOff(int displayLogo);
void SpecialMatrixcodeProcessing( int*col, int*row );
void TestMode( int );

void SetSetupSetting( unsigned int SystemParameterNo, unsigned char SystemParameterValue );
unsigned char GetSetupSetting( unsigned int SystemParameterNo );

int Timer_Install( int InternalTimerID, void (*hander)(void), int elapse );
int Timer_Deinstall( int InternalTimerID );
int Timer_Start( int InternalTimerID );
int Timer_Stop( int InternalTimerID );
#ifdef __cplusplus
}
#endif
