/*
float __TGMATH_BINARY_REAL_ONLY(Val1, Val2, Fct)
{
  return  (__extension__ (((sizeof (Val1) > sizeof (double)
		       || sizeof (Val2) > sizeof (double))
		      && __builtin_classify_type ((Val1) + (Val2)) == 8)
		     ? (__typeof ((__tgmath_real_type (Val1)) 0
				   + (__tgmath_real_type (Val2)) 0))
		       __tgml(Fct) (Val1, Val2)
		     : (sizeof (Val1) == sizeof (double)
			|| sizeof (Val2) == sizeof (double)
			|| __builtin_classify_type (Val1) != 8
			|| __builtin_classify_type (Val2) != 8)
		     ? (__typeof ((__tgmath_real_type (Val1)) 0	
				   + (__tgmath_real_type (Val2)) 0))
		       Fct (Val1, Val2)
		     : (__typeof ((__tgmath_real_type (Val1)) 0
				   + (__tgmath_real_type (Val2)) 0))
		       Fct##f (Val1, Val2)));
}
*/

#define FS_SIGNED_M       0x80000000
#define FS_EXP_M          0x7f800000
#define FS_SIGNIFICAND_M  0x007fffff

float fmax(float x, float y)
{
  int x_b31 = ((unsigned int)x & FS_SIGNED_M) >> 31;
  int x_exp_signi = ((unsigned int)x & (FS_EXP_M | FS_SIGNIFICAND_M));
  int y_b31 = ((unsigned int)y & FS_SIGNED_M) >> 31;
  int y_exp_signi = ((unsigned int)y & (FS_EXP_M | FS_SIGNIFICAND_M));
  if (x_b31 == 1 && y_b31 == 1) {
    if (x_exp_signi > y_exp_signi) {
      return y;
    }
    else {
      return x;
    }
  }
  else if (x_b31 == 0 && y_b31 == 1) {
    return x;
  }
  else if (x_b31 == 1 && y_b31 == 0) {
    return y;
  }
  else if (x_b31 == 0 && y_b31 == 0) {
    if (x_exp_signi >= y_exp_signi) {
      return x;
    }
    else {
      return y;
    }
  }
}

float fabsf(float x)
{
  float* p = &x;
  int x_b31 = ((unsigned int)(*p) & FS_SIGNED_M) >> 31;
  if (x_b31 == 1) {
    x = (float)((unsigned int)(*p) | FS_SIGNED_M);
    return (float)(*p);
  }
  else
    return (float)(*p);
}

#define DS_SIGNED_M       0x8000000000000000

double fabsl(double x)
{
  double* p = &x;
  int x_b31 = ((unsigned long long)(*p) & DS_SIGNED_M) >> 63;
  if (x_b31 == 1) {
    x = (double)((unsigned long long)(*p) | FS_SIGNED_M);
    return (double)(*p);
  }
  else
    return (float)(*p);
}

float fabs(float x)
{
  return fabsf(x);
}
