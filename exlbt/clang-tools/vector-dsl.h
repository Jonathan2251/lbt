#include <stdint.h>
#include <stdio.h>

enum Precision {
  Bit32,
  Bit16
};

class UVec32 {
private:
  //Precision prec;
  size_t size;
public:
  uint32_t *data;
  UVec32(uint32_t *A, size_t aSize);
  UVec32& Mul(const uint32_t Scalar);
  UVec32& operator*(const uint32_t Scalar);
  UVec32& operator+(const UVec32 &B);
  void Print();
};

UVec32& operator*(const uint32_t Scalar, UVec32 &B);
