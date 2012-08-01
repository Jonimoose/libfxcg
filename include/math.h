#ifndef _FXCG_MATH_H
#define _FXCG_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#define INFINITY __builtin_inff()
#define NAN __builtin_nan("0")
#define HUGE_VAL __builtin_huge_val()
#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VALL __builtin_huge_vall()

double atan2(double y, double x);
double cos(double x);
double exp(double x);
double fabs(double x);
double floor(double arg);
double fmod(double x, double y);
double frexp(double arg, int *exp);
long labs(long n);
double ldexp(double arg, int exp);
double log(double x);
double modf(double x, double *iptr);
double pow(double x, double y);
double sin(double x);

#ifdef __cplusplus
}
#endif

#endif
