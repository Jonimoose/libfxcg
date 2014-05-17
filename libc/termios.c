#include <stdio.h>
#include <termios.h>
static const char * formatErr="Not supported %d\n";
speed_t cfgetispeed(const struct termios *termios_p){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
speed_t cfgetospeed(const struct termios *termios_p){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int cfsetispeed(struct termios *termios_p, speed_t speed){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int	cfsetospeed(struct termios * termios_p, speed_t speed){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int	tcgetattr(int fd, struct termios * termios_p){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int tcsetattr(int fd, int optional_actions,const struct termios *termios_p){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int	tcdrain(int fd){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int tcflow(int fildes, int action){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int tcflush(int fildes, int queue_selector){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
int tcsendbreak(int fildes, int duration){
	fprintf(stderr,formatErr,__LINE__);
	return -1;
}
