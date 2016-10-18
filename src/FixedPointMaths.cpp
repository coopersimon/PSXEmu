#include <FixedPointMaths.h>
#include <iostream>

fixedPoint fixedPoint::multiply(fixedPoint a, fixedPoint b)
{
      s_doubleword new_number = a.number * b.number;
      unsigned new_frac_range = a.frac_range + b.frac_range;
      return fixedPoint(new_number, new_frac_range);
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

      // add
      s_doubleword new_number = a_number + b_number;

      return fixedPoint(new_number, new_frac_range);
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

      // sub
      s_doubleword new_number = a_number - b_number;

      return fixedPoint(new_number, new_frac_range);
}

bool fixedPoint::checkSaturation(s_word value, bool not_signed)
{
      s_word number_32 = s_word(number);
      if (number_32 > value)
      {
            number = value;
            return true;
      }

      s_word negative_value = not_signed ? 0 : (-1 - value);

      if (number_32 < negative_value)
      {
            number = negative_value;
            return true;
      }

      return false;
}

bool fixedPoint::checkBits(unsigned bits) const
{
      doubleword masked_number = (number < 0) ?
            doubleword(-number) : doubleword(number);

      doubleword max_value = (number < 0) ?
            (1ll << bits) : (1ll << bits) - 1;

      return masked_number > max_value;
}

bool fixedPoint::checkSign() const
{
      return number < 0;
}

void fixedPoint::truncateFraction(unsigned new_frac_bits)
{
      number >>= (frac_range - new_frac_bits);
      frac_range = new_frac_bits;
}

word fixedPoint::getAsWord(unsigned int_bits, unsigned frac_bits)
{
      // extract integer and fraction parts
      word integer = number >> frac_range;
      integer &= 0xFFFFFFFF >> (32 - int_bits);
      integer <<= frac_bits;

      word fraction = number >> (frac_range - frac_bits);
      fraction &= 0xFFFFFFFF >> (32 - frac_bits);

      frac_range = frac_bits;

      return number = (integer | fraction);
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

      // add
      s_doubleword new_number = lhs_number + rhs_number;

      return fixedPoint(new_number, new_frac_range);
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

      // add
      s_doubleword new_number = lhs_number - rhs_number;

      return fixedPoint(new_number, new_frac_range);
}

fixedPoint operator*(const fixedPoint& lhs, const fixedPoint& rhs)
{
      s_doubleword new_number = lhs.number * rhs.number;
      unsigned new_frac_range = lhs.frac_range + rhs.frac_range;
      return fixedPoint(new_number, new_frac_range);
}
