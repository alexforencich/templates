#ifndef ALPHANUM__HPP
#define ALPHANUM__HPP

/*
The Alphanum Algorithm is an improved sorting algorithm for strings
containing numbers.  Instead of sorting numbers in ASCII order like a
standard sort, this algorithm sorts numbers in numeric order.

The Alphanum Algorithm is discussed at http://www.DaveKoelle.com

This implementation is Copyright (c) 2008 Dirk Jagdmann <doj@cubic.org>.
It is a cleanroom implementation of the algorithm and not derived by
other's works. In contrast to the versions written by Dave Koelle this
source code is distributed with the libpng/zlib license.

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you
       must not claim that you wrote the original software. If you use
       this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and
       must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
       distribution. */

/* $Header: /code/doj/alphanum.hpp,v 1.3 2008/01/28 23:06:47 doj Exp $ */

namespace doj
{
  /**
     Compare left and right with the same semantics as strcmp(), but with the
     "Alphanum Algorithm" which produces more human-friendly
     results. The classes lT and rT must implement "std::ostream
     operator<< (std::ostream&, const Ty&)".

     @return negative if left<right, 0 if left==right, positive if left>right.
  */
  template <typename lT, typename rT>
  int alphanum_comp(const lT& left, const rT& right);

  /**
     Compare l and r with the same semantics as strcmp(), but with
     the "Alphanum Algorithm" which produces more human-friendly
     results.

     @return negative if l<r, 0 if l==r, positive if l>r.
  */
  template <>
  int alphanum_comp<std::string>(const std::string& l, const std::string& r);

  ////////////////////////////////////////////////////////////////////////////

  // now follow a lot of overloaded alphanum_comp() functions to get a
  // direct call to alphanum_impl() upon the various combinations of c
  // and c++ strings.

  /**
     Compare l and r with the same semantics as strcmp(), but with
     the "Alphanum Algorithm" which produces more human-friendly
     results.

     @return negative if l<r, 0 if l==r, positive if l>r.
  */
  int alphanum_comp(char* l, char* r);

  int alphanum_comp(const char* l, const char* r);

  int alphanum_comp(char* l, const char* r);

  int alphanum_comp(const char* l, char* r);

  int alphanum_comp(const std::string& l, char* r);

  int alphanum_comp(char* l, const std::string& r);

  int alphanum_comp(const std::string& l, const char* r);

  int alphanum_comp(const char* l, const std::string& r);

  ////////////////////////////////////////////////////////////////////////////

  /**
     Functor class to compare two objects with the "Alphanum
     Algorithm". If the objects are no std::string, they must
     implement "std::ostream operator<< (std::ostream&, const Ty&)".
  */
  template<class Ty>
  struct alphanum_less : public std::binary_function<Ty, Ty, bool>
  {
    bool operator()(const Ty& left, const Ty& right) const
    {
      return alphanum_comp(left, right) < 0;
    }
  };
}


#endif // ALPHANUM__HPP