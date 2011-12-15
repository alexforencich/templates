/************************************************************************/
/* XMEGA EEPROM Driver                                                  */
/*                                                                      */
/* eeprom.h                                                             */
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

#ifndef __EEPROM_H
#define __EEPROM_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>



#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE 32
#endif

// AVR1008 fix
//#define USE_AVR1008_EEPROM

// EEPROM class
class EEPROM
{
private:
        // Per object data
        
        // Static data
        static uint16_t current_address;
        
        // Private methods
        
        // Private static methods
        
public:
        // Public variables
        
        // Public methods
        EEPROM();
        ~EEPROM();
        
        static uint8_t read_byte(uint16_t addr);
        static void write_byte(uint16_t addr, uint8_t byte);
        static uint16_t read_block(uint16_t addr, uint8_t *dest, uint16_t len);
        static uint16_t write_block(uint16_t addr, const uint8_t *src, uint16_t len);
        
        static void erase_page(uint16_t addr);
        static void erase_all();
        
        // Note: extra parentheses needed to prevent
        // avr-libc stdio macro expansion
        static void (putc)(char c);
        static void puts(const char *str);
        static int write(const void *ptr, int num);
        
        static char (getc)();
        static void gets(char *dest);
        static int read(void *dest, int num);
        
        static void seek(uint16_t addr);
        static uint16_t getpos();
        
        static void setup_stream(FILE *stream);
        
        // Static methods
        static int put(char c, FILE *stream);
        static int get(FILE *stream);
};

// Prototypes


#endif // __EEPROM_H



