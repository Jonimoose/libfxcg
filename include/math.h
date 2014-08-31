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

/* Constants rounded for 21 decimals. */
#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_1_SQRTPI 0.564189583547756286948
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT_2 0.707106781186547524401

#ifdef __cplusplus
}
#endif

#endif
