#ifndef FIXED_POINT_MATHS_H
#define FIXED_POINT_MATHS_H

/*
 *    Fixed Point Maths
 *
 *    Used for complex fixed point maths in the gte
 *
 */ 

#include <Memory.h>
#include <string>

// NOTE: input to fixed point operations MUST be 32 bit to get accurate results
// operations on unknown data (which could be up to 64 bits) could result in errors
// to avoid this issue convert back before doing more fixed point operations

// represents a fixed point value
class fixedPoint
{
      /*** DATA ***/
      s_doubleword number;
      unsigned frac_range;
      
      // internal constructor used for return values
      fixedPoint(s_doubleword value, unsigned frac_bits) :
            number(value), frac_range(frac_bits) {}

public:
      /*** CONSTRUCTORS ***/
      // zero constructor
      fixedPoint() :
            number(0), frac_range(0) {}

      fixedPoint(word value, unsigned frac_bits) :
            number(s_word(value)), frac_range(frac_bits) {}

      fixedPoint(halfword value, unsigned frac_bits) :
            number(s_halfword(value)), frac_range(frac_bits) {}

      // byte can be constructed signed or unsigned
      fixedPoint(byte value, unsigned frac_bits, bool signed_byte = true)
      {
            frac_range = frac_bits;
            if (signed_byte)
                  number = s_byte(value);
            else
                  number = value;
      }
      
      /*** STATUS & RETURN ***/

      // check: value > number > -value-1
      // only check negative side if "is_signed" = true
      // if it is, saturate the number and return true
      bool checkSaturation(s_word value, bool positive);

      // check the number is not using more than "bits"
      // if it is return true
      bool checkOverflow(unsigned bits) const;

      // positive = false, negative = true
      bool checkSign() const;

      // set fraction to the new bits
      // this might not be needed: add/sub may just need to change how they shift bits
      void truncateFraction(unsigned new_frac_bits);

      // return as 32 bit value
      word getAsWord(unsigned int_bits, unsigned frac_bits);

      /*** OPERATIONS ***/
      static fixedPoint multiply(fixedPoint a, fixedPoint b);
      //static fixedPoint divide(fixedPoint a, fixedPoint b);
      static fixedPoint add(fixedPoint a, fixedPoint b);
      static fixedPoint subtract(fixedPoint a, fixedPoint b);

      /*** OPERATORS ***/
      friend fixedPoint operator+(const fixedPoint& lhs, const fixedPoint& rhs);
      friend fixedPoint operator-(const fixedPoint& lhs, const fixedPoint& rhs);
      friend fixedPoint operator*(const fixedPoint& lhs, const fixedPoint& rhs);

      /*** DEBUG ***/
      std::string hexStr() const; // return int.frac in hex
};

fixedPoint operator+(const fixedPoint& lhs, const fixedPoint& rhs);
fixedPoint operator-(const fixedPoint& lhs, const fixedPoint& rhs);
fixedPoint operator*(const fixedPoint& lhs, const fixedPoint& rhs);

#endif
