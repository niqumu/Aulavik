/*====------------- math.h - Standard mathematics header -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

/* Created by clamoureux on 2023/11/1 */

// TODO 2023/11/1: §7.12.8
// Per §7.12.9, FE_DIVBYZERO, FE_INVALID, and FE_OVERFLOW shall be defined
//      in <fenv.h> when an implementation is written.

#ifndef _MATH_H
#define _MATH_H

/*
 * Provide definitions for the float_t and double_t types
 *
 *      float_t         a floating type at least as wide as float
 *
 *      double_t        a floating type at least as wide as double, and
 *                      at least as wide as float_t
 *
 * @ ISO/IEC 9899:201 §7.12.0.2
 */
#if __FLT_EVAL_METHOD__ == 0
        typedef float float_t
        typedef double double_t
#elif __FLT_EVAL_METHOD__ == 1
        typedef double float_t
        typedef double double_t
#elif ___FLT_EVAL_METHOD__ == 2
        typedef long double float_t
        typedef long double double_t
#else /* Default to __FLT_EVAL_METHOD 0 */
        #define __FLT_EVAL_METHOD__ 0
        typedef float float_t
        typedef double double_t
#endif

/* Constant expressions */
#ifdef __GNUC__
        #define HUGE_VAL __builtin_huge_val()
        #define HUGE_VALF __builtin_huge_valf()
        #define HUGE_VALL __builtin_huge_vall()
        #define INFINITY __builtin_inff()

        /* IEEE 754 NAN @ ISO/IEC 9899:201 §7.12.0.5 */
        #define NAN (0.0f / 0.0f)
#else
        /*
         * Definitions for HUGE_VAL, a positive double constant expression, as
         * well as HUGE_VALF and HUGE_VALL, float and long double counterparts,
         * respectively.
         *
         * @ ISO/IEC 9899:201 §7.12.0.3
         */
        #define HUGE_VAL 1e10000
        #define HUGE_VALF 1e10000f
        #define HUGE_VALL 1e10000l

        /* IEEE 754 infinity @ ISO/IEC 9899:201 §7.12.0.4 */
        #define INFINITY HUGE_VALF

        /* IEEE 754 NAN @ ISO/IEC 9899:201 §7.12.0.5 */
        #define NAN (0.0f / 0.0f)
#endif

/* Number classification macros @ ISO/IEC 9899:201 §7.12.0.6 */
enum
{
        FP_ZERO =
    #define FP_ZERO 0
        FP_ZERO,
        FP_INFINITE =
    #define FP_INFINITE 1
        FP_INFINITE,
        FP_NAN =
    #define FP_NAN 2
        FP_NAN,
        FP_NORMAL =
    #define FP_NORMAL 3
        FP_NORMAL,
        FP_SUBNORMAL =
    #define FP_SUBNORMAL 4
        FP_SUBNORMAL
};

/* @ ISO/IEC 9899:201 §7.12.0.9 */
#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

/* Implementation-specific error constants, per §7.12.1 */
#define MATH_ERRDOMAIN 4        /* input outside domain of function */
#define MATH_ERRPOLE 8          /* function defined as exactly infinity, and
                                   possess an infinite limit, at the finite
                                   input */
#define MATH_SINGULARITY MATH_ERRPOLE
#define MATH_ERRRANGE 16        /* result cannot fit in the specified type */

/* Classification macros @ ISO/IEC 9899:201 §7.12.3 */

/*
 * Provide a definition for the fpclassify macro, classifying a provided
 * floating point number into one of the FP types defined.
 *
 * If GCC is used, the GNUC __builtin implementation is used instead.
 *
 * @ ISO/IEC 9899:201 §7.12.3.1
 */
#ifdef __GNUC__
        #define fpclassify(x) __builtin_fpclassify(FP_NAN, FP_INFINITE,       \
        FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#else
        #ifndef __STDC_IEC_559__
        #error "IEEE 754 not implemented, and GNUC missing!"
        #endif

        #include <stdint.h> /* uint32_t */

        typedef struct {
            uint32_t sign, exponent, mantissa;
        } f_parts;

        inline f_parts deconstruct(float f)
        {
                uint32_t bytes = *(uint32_t)&f;
                f_parts fp;
                fp.fraction = bytes & 0x007fffff;
                bytes >>= 23;
                fp.exponent = bytes & 0x000000ff;
                bytes >>= 8;
                fp.sign = bytes & 0x00000001;
                bytes >>= 1;
                return fp;
        }

        // TODO this is horrifically slow, how does GNUC do it?
        inline int __fpclassify(float val)
        {
                if (val == 0.0f)
                        return FP_ZERO;
                if (val == HUGE_VALF)
                        return FP_INFINITE;
                if (val == NAN)
                        return FP_NAN;

                f_parts fp = deconstruct(val);
                if (!fp.exponent && fp.mantissa)
                        return FP_SUBNORMAL;

                return FP_NORMAL;
        }

        inline int __fpclassify(double val)
        {
                return __fpclassify((float) val); // TODO does this work?
        }

        #define fpclassify(x) __fpclassify(x)
#endif

#define isinf(x) (fpclassify(x) == FP_INFINITE)
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isnormal(x) (fpclassify(x) == FP_NORMAL)

#ifdef __GNUC__
        #define isfinite(x) (__builtin_isfinite(x))
        #define signbit(x) __builtin_signbitl(x)
#else
        inline int __isfinite(float x)
        {
                int type = fpclassify(x);
                return (type != FP_INFINITE && type != FP_NAN);
        }

        inline int __isfinite(double x)
        {
                int type = fpclassify(x);
                return (type != FP_INFINITE && type != FP_NAN);
        }

        #define isfinite(x) (__isfinite(x))
        #define signbit(x) (x < 0)
#endif

/* Comparison macros @ ISO/IEC 9899:201 §7.12.14 */
// TODO fallbacks might not be compliant is all edge-cases

#ifdef __GNUC__
#define isgreater(x, y) __builtin_isgreater(x, y)
#else
#define isgreater(x, y) (x > y)
#endif

#ifdef __GNUC__
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#else
#define isgreaterequal(x, y) (x >= y)
#endif

#ifdef __GNUC__
#define isless(x, y) __builtin_isless(x, y)
#else
#define isless(x, y) (x < y)
#endif

#ifdef __GNUC__
#define islessequal(x, y) __builtin_islessequal(x, y)
#else
#define islessequal(x, y) (x <= y)
#endif

#ifdef __GNUC__
#define isgreater(x, y) __builtin_isgreater(x, y)
#else
#define isgreater(x, y) (x > y)
#endif

#ifdef __GNUC__
#define isgreater(x, y) __builtin_isgreater(x, y)
#else
#define isgreater(x, y) (x > y)
#endif

#ifdef __GNUC__
#define islessgreater(x, y) __builtin_islessgreater(x, y)
#else
#define islessgreater(x, y) (isless(x, y) || isgreater(x, y))
#endif

#ifdef __GNUC__
#define isunordered(x, y) __builtin_isunordered(x, y)
#else
#define isunordered(x, y) (isnan(x) || isnan(y))
#endif


/* Trigonometric functions @ ISO/IEC 9899:201 §7.12.4 */

/*
 * The acos family of functions compute the principal value of the arc cosine
 * of x. A domain error occurs for arguments not in the interval [-1, 1]
 */
double acos(double x);
float acosf(float x);
long double acosl(long double x);

/*
 * The acos family of functions compute the principal value of the arc sine
 * of x. A domain error occurs for arguments not in the interval [-1, 1]
 */
double asin(double x);
float asinf(float x);
long double asinl(long double x);

/*
 * The atan family of functions compute the principal value of the arc tangent
 * of x.
 */
double atan(double x);
float atanf(float x);
long double atanl(long double x);

/*
 * The atan2 family of functions compute the value of the arc tangent of y/x,
 * using the signs of both arguments to determine the quadrant of the return
 * value. A domain error occurs where x and y are both zero.
 */
double atan2(double y, double x);
float atan2f(float y, float x);
long double atan2l(long double y, long double x);

/* The cos family of functions compute the cosine of x, measured in radians */
double cos(double x);
float cosf(float x);
long double cosl(long double x);

/* The sin family of functions compute the sine of x, measured in radians */
double sin(double x);
float sinf(float x);
long double sinl(long double x);

/* The tan family of functions compute the tangent of x, measured in radians */
double tan(double x);
float tanf(float x);
long double tanl(long double x);

/* Hyperbolic functions @ ISO/IEC 9899:201 §7.12.5 */

double asinh(double x);
float asinhf(float x);
long double asinhl(long double x);

double atanh(double x);
float atanhf(float x);
long double atanhl(long double x);

double cosh(double x);
float coshf(float x);
long double coshl(long double x);

double sinh(double x);
float sinhf(float x);
long double sinhl(long double x);

double tanh(double x);
float tanhf(float x);
long double tanhl(long double x);

/* Exponential/logarithmic functions @ ISO/IEC 9899:201 §7.12.6 */

/*
 * The exp family of functions compute the base-e exponential of x. A range
 * error occurs if the magnitude of x is too large.
 */
double exp(double x);
float expf(float x);
long double expl(long double x);

/*
 * The exp2 family of functions compute the base-2 exponential of x. A range
 * error occurs if the magnitude of x is too large.
 */
double exp2(double x);
float exp2f(float x);
long double exp2l(long double x);

/*
 * The expm1 family of functions compute the base-e exponential of x, minus
 * one. A range error occurs if the magnitude of x is too large.
 */
double expm1(double x);
float expm1f(float x);
long double expm1l(long double x);

/*
 * The frexp family of functions break a floating-point number into a
 * normalized fraction and an integral power of 2. They store the integer in
 * the object pointed to by exp.
 *
 * If value is not a floating-point number, or if the integral power of 2 is
 * outside the range of int, the return value is unspecified. Otherwise, the
 * value x shall be returned, such that x has a magnitude in the interval
 * [1/2, 1) or zero, and value equals x * 2^exp. If value is zero, both parts
 * of the result shall be zero.
 */
double frexp(double value, int *exp);
float frexpf(float value, int *exp);
long double frexpl(long double value, int *exp);

/*
 * The ilogb family of functions extract the exponent of x as a signed int
 * value. A domain error occurs if x is zero, infinite, or NaN.
 */
int ilogb(double x);
int ilogbf(float x);
int ilogbl(long double x);

/*
 * The ldexp family of functions multiply a floating-point number by an
 * integral power of 2. A range error may occur. TODO: when?
 */
double ldexp(double x, int exp);
float ldexpf(float x, int exp);
long double ldexpl(long double x, int exp);

/*
 * The log family of functions compute the base-e/natural logarithm of x.
 * A domain error occurs if the argument is negative. A pole error occurs
 * if the argument is zero.
 */
double log(double x);
float logf(float x);
long double logl(long double x);

/*
 * The log10 family of functions compute the base-10/common logarithm of x.
 * A domain error occurs if the argument is negative. A pole error occurs
 * if the argument is zero.
 */
double log10(double x);
float log10f(float x);
long double log10l(long double x);

/*
 * The log1p family of functions compute the base-e/natural logarithm of one
 * plus x. A domain error occurs if the argument is less than -1. A pole error
 * occurs if the argument is equal to -1.
 */
double log1p(double x);
float log1pf(float x);
long double log1pl(long double x);

/*
 * The log 2family of functions compute the base-2 logarithm of x. A domain
 * error occurs if the argument is negative. A pole error occurs if the
 * argument is zero.
 */
double log2(double x);
float log2f(float x);
long double log2l(long double x);

/*
 * The logb family of functions extract the exponent of x, as a signed integer
 * value in a floating-point format. A pole error occurs if the argument is
 * zero.
 */
double logb(double x);
float logbf(float x);
long double logbl(long double x);

/*
 * The modf family of functions break the argument value into integral and
 * fractional parts, each of which has the same type and sign as the argument.
 */
double modf(double value, double *iptr);
float modff(float value, float *iptr);
long double modfl(long double value, long double *iptr);

/*
 * The scalbn and scalbln function families efficiently compute
 * x * FLT_RADIX^n, not normally by explicitly computing FLT_RADIX^n.
 */
double scalbn(double x, int n);
float scalbnf(float x, int n);
long double scalbnl(long double x, int n);
double scalbln(double x, long int n);
float scalblnf(float x, long int n);
long double scalblnl(long double x, long int n);

/* Power and absolute-value functions @ ISO/IEC 9899:201 §7.12.7 */

/*
 * The cbrt family of functions compute the real cube root of x.
 */
double cbrt(double x);
float cbrtf(float x);
long double cbrtl(long double x);

/*
 * The fabs family of functions compute the absolute value of a floating-point
 * number, x.
 */
double fabs(double x);
float fabsf(float x);
long double fabsl(long double x);

/*
 * The hypot family of functions compute the square root of the sum of the
 * squares of x and y.
 */
double hypot(double x, double y);
float hypotf(float x, float y);
long double hypotl(long double x, long double y);

/*
 * The pow family of functions compute x raised to the power of y. A domain
 * error occurs of x is finite and negative and y is finite and not an integer.
 * A domain error occurs if both x and y are zero. A pole error occurs if x is
 * zero and y is less than zero.
 */
double pow(double x, double y);
float powf(float x, float y);
long double powl(long double x, long double y);

/*
 * The sqrt family of functions compute the non-negative square root of x. A
 * domain error occurs if the argument is less than zero.
 */
double sqrt(double x);
float sqrtf(float x);
long double sqrtl(long double x);

/* Error and gamma functions @ ISO/IEC 9899:201 §7.12.8 */
// TODO: God save us.

/* Nearest integer functions @ ISO/IEC 9899:201 §7.12.9 */

/*
 * The ceil family of functions compute the smallest integer value not less
 * than x.
 */
double ceil(double x);
float ceilf(float x);
long double ceill(long double x);

/*
 * The floor family of functions compute the largest integer value not greater
 * than x.
 */
double floor(double x);
float floorf(float x);
long double floorl(long double x);

/*
 * The nearbyint family of functions round their argument to an integer value
 * in a floating point context, using the current rounding direction.
 */
double nearbyint(double x);
float nearbyintf(float x);
long double nearbyintl(long double x);

/*
 * The rint family of functions is identical to the nearbyint family, however
 * they may raise the "inexact" floating-point exception should the result
 * differ from the argument.
 */
double rint(double x);
float rintf(float x);
long double rintl(long double x);

/*
 * The lrint and llrint function families round their argument to the nearest
 * integer value, rounding according to the current rounding direction. Should
 * the value be outside the range of the return type, a domain error occurs.
 */
long int lrint(double x);
long int lrintf(float x);
long int lrintl(long double x);
long long int llrint(double x);
long long int llrintf(float x);
long long int llrintl(long double x);

/*
 * The round family of functions round their argument to the nearest integer
 * value in floating-pont format. Half-way cases are rounded away from zero,
 * regardless of the current rounding direction.
 */
double round(double x);
float roundf(float x);
long double roundl(long double x);

/*
 * The lround and llround function families round their argument to the
 * nearest integer value. Half-way cases are rounded away from zero, regardless
 * of the current rounding direction.
 */
long int lround(double x);
long int lroundf(float x);
long int lroundl(long double x);
long long int llround(double x);
long long int llroundf(float x);
long long int llroundl(long double x);

/*
 * The trunc family of functions round their argument to the integer value,
 * in floating point format, nearest to but no larger in magnitude than the
 * argument.
 */
double trunc(double x);
float truncf(float x);
long double truncl(long double x);

/* Remainder functions @ ISO/IEC 9899:201 §7.12.10 */
// TODO finish function declarations


#endif /* _MATH_H */