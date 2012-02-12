/************************************************************************/
/* IStream Input Stream                                                 */
/*                                                                      */
/* istream.cpp                                                          */
/*                                                                      */
/* Alex Forencich <alex@alexforencich.com>                              */
/*                                                                      */
/* Copyright (c) 2011 Alex Forencich                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files(the "Software"), to deal in the Software without restriction,  */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

#include "istream.h"


size_t IStream::available()
{
        return 0;
}


char IStream::get()
{
        return 0;
}


int IStream::peek(size_t index)
{
        return EOF;
}


void IStream::read_string(char *dest)
{
        char last;
        do
        {
                last = this->get();
                *(dest++) = last;
        }
        while (last != 0 && last != '\r' && last != '\n');
        *(dest++) = 0;
}


size_t IStream::read(void *dest, size_t num)
{
        size_t j = num;
        char *ptr2 = (char *)dest;
        if (num == 0 || ptr2 == 0)
                return 0;
        while (num--)
        {
                *(ptr2++) = this->get();
        }
        return j;
}


