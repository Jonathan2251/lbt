
#ifndef __LIMITS_H_
#define __LIMITS_H_ 1

/* Number of bits in a `char'.	*/
#  define CHAR_BIT	8

/* Minimum and maximum values a `signed int' can hold.  */
#  define INT_MIN	(-INT_MAX - 1)
#  define INT_MAX	2147483647

#  define LONG_MAX	2147483647L
#  define LONG_MIN	(-LONG_MAX - 1L)

#  define ULONG_MAX	4294967295UL

#if 1
# define UINT32_C(c)	(c ## U)
#  define UINT64_C(c)	(c ## ULL)
#endif
#undef KERNEL_USE
/*
# define __TGMATH_BINARY_REAL_ONLY(Val1, Val2, Fct) \
     (__extension__ (((sizeof (Val1) > sizeof (double)			      \
		       || sizeof (Val2) > sizeof (double))		      \
		      && __builtin_classify_type ((Val1) + (Val2)) == 8)      \
		     ? (__typeof ((__tgmath_real_type (Val1)) 0		      \
				   + (__tgmath_real_type (Val2)) 0))	      \
		       __tgml(Fct) (Val1, Val2)				      \
		     : (sizeof (Val1) == sizeof (double)		      \
			|| sizeof (Val2) == sizeof (double)		      \
			|| __builtin_classify_type (Val1) != 8		      \
			|| __builtin_classify_type (Val2) != 8)		      \
		     ? (__typeof ((__tgmath_real_type (Val1)) 0		      \
				   + (__tgmath_real_type (Val2)) 0))	      \
		       Fct (Val1, Val2)					      \
		     : (__typeof ((__tgmath_real_type (Val1)) 0		      \
				   + (__tgmath_real_type (Val2)) 0))	      \
		       Fct##f (Val1, Val2)))*/

/* Return maximum numeric value from X and Y.  */
//#define fmax(Val1, Val2) __TGMATH_BINARY_REAL_ONLY (Val1, Val2, fmax)

#endif

