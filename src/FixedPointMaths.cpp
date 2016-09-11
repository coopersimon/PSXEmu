#include <FixedPointMaths.h>

fixedPoint::fixedPoint(word value, unsigned int_bits, unsigned frac_bits)
{
      int_range = int_bits;
      frac_range = frac_bits;
      
      sign = (1 << (int_bits + frac_bits)) & value;

      number = value & (mask >> (32 - int_bits - frac_bits));
}

fixedPoint fixedPoint::multiply(fixedPoint a, fixedPoint b)
{
      bool new_sign = (a.sign != b.sign);
      doubleword new_number = a.number * b.number;
      unsigned new_int_range = a.int_range + b.int_range;
      unsigned new_frac_range = a.frac_range + b.frac_range;
      return fixedPoint(new_sign, new_number, new_int_range, new_frac_range);
}

fixedPoint fixedPoint::add(fixedPoint a, fixedPoint b)
{
      // first get a and b on the same decimal point
      s_doubleword a_number = a.number;
      s_doubleword b_number = b.number;
      unsigned new_frac_range;

      if (a.frac_range > b.frac_range)
      {
            new_frac_range = a.frac_range;
            b_number = b.number << (new_frac_range - b.frac_range);
      }
      else
      {
            new_frac_range = b.frac_range;
            a_number = a.number << (new_frac_range - a.frac_range);
      }

      // get integer range
      unsigned new_int_range = (a.int_range > b.int_range) ?
            (a.int_range + 1) : (b.int_range + 1);

      // convert to signed numbers
      a_number = a.sign ? (-a_number) : a_number;
      b_number = b.sign ? (-b_number) : b_number;

      // add
      s_doubleword new_number = a_number + b_number;

      // check sign and convert back
      if (new_number >= 0)
      {
            return fixedPoint(false, doubleword(new_number), new_int_range, new_frac_range);
      }
      else
      {
            new_number = (-new_number);
            return fixedPoint(true, doubleword(new_number), new_int_range, new_frac_range);
      }
}

fixedPoint fixedPoint::subtract(fixedPoint a, fixedPoint b)
{
      // first get a and b on the same decimal point
      s_doubleword a_number = a.number;
      s_doubleword b_number = b.number;
      unsigned new_frac_range;

      if (a.frac_range > b.frac_range)
      {
            new_frac_range = a.frac_range;
            b_number = b.number << (new_frac_range - b.frac_range);
      }
      else
      {
            new_frac_range = b.frac_range;
            a_number = a.number << (new_frac_range - a.frac_range);
      }

      // get integer range
      unsigned new_int_range = (a.int_range > b.int_range) ?
            (a.int_range) : (b.int_range);

      // convert to signed numbers
      a_number = a.sign ? (-a_number) : a_number;
      b_number = b.sign ? (-b_number) : b_number;

      // add
      s_doubleword new_number = a_number - b_number;

      // check sign and convert back
      if (new_number >= 0)
      {
            return fixedPoint(false, doubleword(new_number), new_int_range, new_frac_range);
      }
      else
      {
            new_number = (-new_number);
            return fixedPoint(true, doubleword(new_number), new_int_range, new_frac_range);
      }
}

word fixedPoint::getAsWord(unsigned int_bits, unsigned frac_bits) const
{
      // extract integer and fraction parts
      word integer = number >> frac_range;
      integer &= mask >> (32 - int_bits);
      integer <<= frac_bits;

      word fraction = number >> (frac_range - frac_bits);
      fraction &= mask >> (32 - frac_bits);
      
      // get sign bit in correct position
      if (sign)
      {
            word sign_bit = 1 << (int_bits + frac_bits);
            return sign_bit | integer | fraction;
      }
      else
            return integer | fraction;
}

fixedPoint operator+(const fixedPoint& lhs, const fixedPoint& rhs)
{
      // first get a and b on the same decimal point
      s_doubleword lhs_number = lhs.number;
      s_doubleword rhs_number = rhs.number;
      unsigned new_frac_range;

      if (lhs.frac_range > rhs.frac_range)
      {
            new_frac_range = lhs.frac_range;
            rhs_number = rhs.number << (new_frac_range - rhs.frac_range);
      }
      else
      {
            new_frac_range = rhs.frac_range;
            lhs_number = lhs.number << (new_frac_range - lhs.frac_range);
      }

      // get integer range
      unsigned new_int_range = (lhs.int_range > rhs.int_range) ?
            (lhs.int_range + 1) : (rhs.int_range + 1);

      // convert to signed numbers
      lhs_number = lhs.sign ? (-lhs_number) : lhs_number;
      rhs_number = rhs.sign ? (-rhs_number) : rhs_number;

      // add
      s_doubleword new_number = lhs_number + rhs_number;

      // check sign and convert back
      if (new_number >= 0)
      {
            return fixedPoint(false, doubleword(new_number), new_int_range, new_frac_range);
      }
      else
      {
            new_number = (-new_number);
            return fixedPoint(true, doubleword(new_number), new_int_range, new_frac_range);
      }
}

fixedPoint operator-(const fixedPoint& lhs, const fixedPoint& rhs)
{
      // first get a and b on the same decimal point
      s_doubleword lhs_number = lhs.number;
      s_doubleword rhs_number = rhs.number;
      unsigned new_frac_range;

      if (lhs.frac_range > rhs.frac_range)
      {
            new_frac_range = lhs.frac_range;
            rhs_number = rhs.number << (new_frac_range - rhs.frac_range);
      }
      else
      {
            new_frac_range = rhs.frac_range;
            lhs_number = lhs.number << (new_frac_range - lhs.frac_range);
      }

      // get integer range
      unsigned new_int_range = (lhs.int_range > rhs.int_range) ?
            (lhs.int_range + 1) : (rhs.int_range + 1);

      // convert to signed numbers
      lhs_number = lhs.sign ? (-lhs_number) : lhs_number;
      rhs_number = rhs.sign ? (-rhs_number) : rhs_number;

      // add
      s_doubleword new_number = lhs_number - rhs_number;

      // check sign and convert back
      if (new_number >= 0)
      {
            return fixedPoint(false, doubleword(new_number), new_int_range, new_frac_range);
      }
      else
      {
            new_number = (-new_number);
            return fixedPoint(true, doubleword(new_number), new_int_range, new_frac_range);
      }
}

fixedPoint operator*(const fixedPoint& lhs, const fixedPoint& rhs)
{
      bool new_sign = (lhs.sign != rhs.sign);
      doubleword new_number = lhs.number * rhs.number;
      unsigned new_int_range = lhs.int_range + rhs.int_range;
      unsigned new_frac_range = lhs.frac_range + rhs.frac_range;
      return fixedPoint(new_sign, new_number, new_int_range, new_frac_range);
}
