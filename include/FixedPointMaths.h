#ifndef FIXED_POINT_MATHS_H
#define FIXED_POINT_MATHS_H

/*
 *    Fixed Point Maths
 *
 *    Used for complex fixed point maths in the gte
 *
 */ 

#include <Memory.h>

// NOTE: input to fixed point operations MUST be 32 bit to get accurate results
// operations on unknown data (which could be up to 64 bits) could result in errors
// to avoid this issue convert back before doing more fixed point operations


class fixedPoint
{
      // mask used for lots of operations
      static const doubleword mask = 0xFFFFFFFFFFFFFFFF;

      // break down of fixed point data
      bool sign;
      doubleword number;
      unsigned int_range;
      unsigned frac_range;
      
      // internal constructor used for return values
      fixedPoint(bool sign_bit, doubleword value, unsigned int_bits, unsigned frac_bits) :
            sign(sign_bit), number(value), int_range(int_bits), frac_range(frac_bits) {}

public:
      // public constructor
      fixedPoint(word value, unsigned int_bits, unsigned frac_bits);
      
      // check the fixed point value is not using more than [bits]
      // if it is, saturate the value
      bool checkSaturation(unsigned bits);

      // check the sign of the value
      bool checkSign() { return sign; }

      // operations
      static fixedPoint multiply(fixedPoint a, fixedPoint b);
      //static fixedPoint divide(fixedPoint a, fixedPoint b);
      static fixedPoint add(fixedPoint a, fixedPoint b);
      static fixedPoint subtract(fixedPoint a, fixedPoint b);

      // return as 32 bit value
      word getAsWord(unsigned int_bits, unsigned frac_bits) const;

      // return as 64 bit float
      double getAsDouble() const;

      friend fixedPoint operator+(const fixedPoint& lhs, const fixedPoint& rhs);
      friend fixedPoint operator-(const fixedPoint& lhs, const fixedPoint& rhs);
      friend fixedPoint operator*(const fixedPoint& lhs, const fixedPoint& rhs);
};

fixedPoint operator+(const fixedPoint& lhs, const fixedPoint& rhs);
fixedPoint operator-(const fixedPoint& lhs, const fixedPoint& rhs);
fixedPoint operator*(const fixedPoint& lhs, const fixedPoint& rhs);

#endif
