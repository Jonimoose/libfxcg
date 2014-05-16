#include <time.h>
#include <fxcg/rtc.h>
#include <stdio.h>
clock_t clock() {
    return RTC_GetTicks();
}
time_t mktime (struct tm * timeptr){
	fputs("mktime\n",stderr);
	return 0;
}
//Huge thanks to gbl08ma these time functions are borowed from utilities
#define R64CNT	(unsigned char*)0xA413FEC0
#define RYRCNT	(unsigned short*)0xA413FECE
#define RMONCNT	(unsigned char*)0xA413FECC
#define RDAYCNT	(unsigned char*)0xA413FECA
#define RHRCNT	(unsigned char*)0xA413FEC6
#define RMINCNT	(unsigned char*)0xA413FEC4
#define RSECCNT	(unsigned char*)0xA413FEC2
#define RWKCNT	(unsigned char*)0xA413FEC8
#define RCR2	(unsigned char*)0xA413FEDE

static const char monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int getMonthDays(int month) {
  return monthDays[month-1];
}

static int isLeap(int y){
    return ( ((y % 4) == 0) && ((y % 100) != 0) ) || ((y % 400) == 0);
}

static int getDayOfYear(int y, int m, int d) {
  // year must be provided because of leap years
  int days = 0;
  int i;
  for(i = 1; i<m; i++) {
    if(i!=2) days+=getMonthDays(i);
    else days+=(isLeap(y)? 29 : 28);
  }
  return days+d;
}

static inline unsigned int bcd_to_2digit(unsigned char* bcd) {
        return ((((*(bcd))&0xf0)>>4)*10) + ((*(bcd))&0x0f);
}
static inline int getCurrentYear() {
  return ((*RYRCNT >> 12) & 0b1111)*1000 + ((*RYRCNT >> 8) & 0b1111)*100 + ((*RYRCNT >> 4) & 0b1111)*10 + (*RYRCNT & 0b1111);
}
static inline int getCurrentMonth() {
  return ((*RMONCNT & 0b10000)>>4)*10 + (*RMONCNT & 0b1111);
}
static inline int getCurrentDay() {
  return ((*RDAYCNT >> 4) & 0b11)*10 + (*RDAYCNT & 0b1111);
}
static inline int getCurrentHour() {
  return bcd_to_2digit(RHRCNT);
}
static inline int getCurrentMinute() {
  return bcd_to_2digit(RMINCNT);
}
static inline int getCurrentSecond() {
  return bcd_to_2digit(RSECCNT);
}
time_t time (time_t* timer){
	//return seconds since 1970-01-01
	int y,m,d;
	y=getCurrentYear();
	m=getCurrentMonth();
	d=getCurrentDay();
	time_t r=getCurrentSecond()+(getCurrentMinute()*60)+(getDayOfYear(y,m,d)*24*60*60);
	r+=((y-1970)/4)*24*60*60;//extra days due to leap year
	r+=(y-1970)*365*24*60*60;
	return r;
}
int gettimeofday(struct timeval *tv, struct timezone *tz){
	tv->tv_usec=0;//caiso prizm RTC does not have that much presison don't even bother
	tv->tv_sec=getCurrentSecond()+(getCurrentMinute()*60)+(getCurrentHour()*60*60);
	return 0;
}
