#ifndef FIFO_H
#define FIFO_H

/*
 *    FIFO.h
 *
 *    Declares a generic FIFO for use in GPU and MDEC
 *    First in first out memory device
 */

// includes
#include <list>


// FIFO
template <typename T>
class FIFOImpl
{
      std::list<T> data;
      unsigned fifo_size;

public:
      FIFOImpl(unsigned size_in) : fifo_size(size_in) {}

      // writes data to back of FIFO
      void write(T in)
      {
            // if FIFO is full
            if (data.size() == fifo_size)
            {
                  data.push_back(in);
                  data.pop_front();
            }
            else
            {
                  data.push_back(in);
            }
      }

      // reads from front of FIFO, and deletes front.
      T read()
      {
            // if FIFO is empty
            if (data.empty())
            {
                  return 0;
            }
            else
            {
                  T out = data.front();
                  data.pop_front();
                  return out;
            }
      }

      bool empty()
      {
            return data.empty();
      }

      bool full()
      {
            return data.size() == fifo_size;
      }

      void clear()
      {
            data.clear();
      }
};

#endif
