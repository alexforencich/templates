/************************************************************************/
/* I2C Driver                                                           */
/*                                                                      */
/* i2c.h                                                                */
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

#ifndef __I2C_H
#define __I2C_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

#include "iostream.h"

// Defines
#define TWIC_IND 0
#define TWIE_IND 1
#define TWID_IND 2
#define TWIF_IND 3

#if defined(TWIF)
#define MAX_TWI_IND 3
#else
#define MAX_TWI_IND 1
#endif

#define I2C_MODE_MASTER         0x00
#define I2C_MODE_SLAVE          0x80
#define I2C_STATE_IDLE          0x00
#define I2C_STATE_ACTIVE        0x01


#ifdef __AVR_XMEGA__
#define I2C_WAIT_WRITE_MASTER() while (!(twi->MASTER.STATUS & TWI_MASTER_WIF_bm)) { }
#define I2C_WAIT_READ_MASTER() while (!(twi->MASTER.STATUS & TWI_MASTER_RIF_bm)) { }
#else // __AVR_XMEGA__
#define I2C_WAIT_WRITE_MASTER() while (!(TWCR & _BV(TWINT))) { }
#define I2C_WAIT_READ_MASTER() I2C_WAIT_WRITE_MASTER()
#endif // __AVR_XMEGA__


// I2c class
class I2c : public IOStream
{
private:
        // Per object data
#ifdef __AVR_XMEGA__
        TWI_t *twi;
        int twi_ind;
#endif // __AVR_XMEGA__
        size_t request;
        
        char flags;
        
        // Static data
#ifdef __AVR_XMEGA__
        static I2c *i2c_list[MAX_TWI_IND-1];
#endif // __AVR_XMEGA__
        
        // Private methods
        
        // Private static methods
#ifdef __AVR_XMEGA__
        static char which_twi(TWI_t *_twi);
        static TWI_t *get_twi(char _twi);
        static PORT_t *get_port(char _twi);
#endif // __AVR_XMEGA__
        
public:
        // Public variables
        
        // Public methods
#ifdef __AVR_XMEGA__
        I2c(TWI_t *_twi);
#else // __AVR_XMEGA__
        I2c();
#endif // __AVR_XMEGA__
        ~I2c();
        
        void begin(uint32_t baud);
        void end();
        
        void start_write(uint8_t addr);
        void start_read(uint8_t addr);
        void start_raw(uint8_t addr);
        void stop();
        
        void set_request(size_t count);
        size_t available();
        
        void put(char c);
        
        char get();
        
        void setup_stream(FILE *stream);
        
        // Static methods
        static int put(char c, FILE *stream);
        static int get(FILE *stream);
        
        //static inline void handle_interrupts(char _i2c);
        //static void handle_interrupts(Usart *_i2c);
};

// Prototypes


#endif // __I2C_H

