#ifndef _MATH_H_
#define	_MATH_H_

//#ifdef HAS_COMPLEX
 #ifndef HUGE_VALF
  #define HUGE_VALF (1.0e999999999F)
 #endif

 #if !defined(INFINITY)
  #define INFINITY (HUGE_VALF)
 #endif

 #if !defined(NAN)
  #define NAN (0.0F/0.0F)
 #endif

 float cabsf(float complex) ;
//#endif
#endif
