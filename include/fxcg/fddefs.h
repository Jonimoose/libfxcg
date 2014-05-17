#define urandom_FD 4
#define frameBuf_FD 5
static inline int fromNativeFD(int x){
	return x+6;
}
static inline int toNativeFD(int x){
	return x-6;
}
