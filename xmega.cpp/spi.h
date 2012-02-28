/************************************************************************/
/* XMEGA SPI Driver                                                     */
/*                                                                      */
/* spi.h                                                                */
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

#ifndef __SPI_H
#define __SPI_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

#include "iostream.h"

// Defines
#define SPIC_IND 1
#define SPID_IND 2
#define SPIE_IND 3
#define SPIF_IND 4

#if defined(SPIF)
#define MAX_SPI_IND 4
#elif defined(SPIE)
#define MAX_SPI_IND 3
#elif defined(SPID)
#define MAX_SPI_IND 2
#else
#define MAX_SPI_IND 1
#endif

#define SPI_MODE_MASTER         0x00
#define SPI_MODE_SLAVE          0x80

#define SPI_SS_PIN              4
#define SPI_MOSI_PIN            5
#define SPI_MISO_PIN            6
#define SPI_SCK_PIN             7
#define SPI_SS_PIN_bm           (1 << SPI_SS_PIN)
#define SPI_MOSI_PIN_bm         (1 << SPI_MOSI_PIN)
#define SPI_MISO_PIN_bm         (1 << SPI_MISO_PIN)
#define SPI_SCK_PIN_bm          (1 << SPI_SCK_PIN)

#define SPI_WAIT() while (!(spi->STATUS & SPI_IF_bm)) { }

// Spi class
class Spi : public IOStream
{
private:
        // Per object data
        SPI_t *spi;
        int spi_ind;
        
        char flags;
        
        // Static data
        static Spi *spi_list[MAX_SPI_IND];
        
        // Private methods
        
        // Private static methods
        static char which_spi(SPI_t *_spi);
        static SPI_t *get_spi(char _spi);
        static PORT_t *get_port(char _spi);
        
public:
        // Public variables
        
        // Public methods
        Spi(SPI_t *_spi);
        ~Spi();
        
        void begin(uint8_t mode, uint8_t prescl, uint8_t clk2x);
        void end();
        
        char transceive(char c);
        
        // Note: extra parentheses needed to prevent
        // avr-libc stdio macro expansion
        void put(char c);
        void write_word(uint16_t word);
        void write_dword(uint32_t dword);
        
        char get();
        
        void setup_stream(FILE *stream);
        
        // Static methods
        static int put(char c, FILE *stream);
        static int get(FILE *stream);
        
        //static inline void handle_interrupts(char _usart);
        //static void handle_interrupts(Usart *_usart);
};

// Prototypes


#endif // __SPI_H



