/************************************************************************/
/* XMEGA USART Driver                                                   */
/*                                                                      */
/* usart.h                                                              */
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

#ifndef __USART_H
#define __USART_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

#include "iostream.h"

// token pasting
#define token_paste2_int(x, y) x ## y
#define token_paste2(x, y) token_paste2_int(x, y)
#define token_paste3_int(x, y, z) x ## y ## z
#define token_paste3(x, y, z) token_paste3_int(x, y, z)

// Defines
#define USARTC0_IND 1
#define USARTD0_IND 2
#define USARTE0_IND 3
#define USARTC1_IND 4
#define USARTD1_IND 5
#define USARTE1_IND 6
#define USARTF0_IND 7
#define USARTF1_IND 8

#if defined(USARTF1)
#define MAX_USART_IND 8
#elif defined(USARTC1)
#define MAX_USART_IND 5
#elif defined(USARTE0)
#define MAX_USART_IND 3
#else
#define MAX_USART_IND 2
#endif

#define USART_TX_QUEUE_EMPTY 0x10
#define USART_TX_QUEUE_FULL 0x20
#define USART_RX_QUEUE_EMPTY 0x40
#define USART_RX_QUEUE_FULL 0x80
#define USART_RUNNING 0x01

#define CREATE_USART(name, device_port) \
        Usart (name)(&token_paste2(USART, device_port)); \
        ISR(token_paste3(USART, device_port, _DRE_vect)) \
        { \
                Usart::handle_interrupts(&name); \
        } \
        ISR(token_paste3(USART, device_port, _RXC_vect), ISR_ALIASOF(token_paste3(USART, device_port, _DRE_vect)));

//#define USART_CREATE_ALL_ISR

// Usart class
class Usart : public IOStream
{
private:
        // Per object data
        USART_t *usart;
        int usart_ind;
        char *txbuf;
        size_t txbuf_size;
        size_t txbuf_head;
        size_t txbuf_tail;
        char *rxbuf;
        size_t rxbuf_size;
        size_t rxbuf_head;
        size_t rxbuf_tail;
        PORT_t *rtsport;
        PORT_t *ctsport;
        uint8_t rtspin_bm;
        uint8_t ctspin_bm;
        
        volatile char flags;
        
        // Static data
        static Usart *usart_list[MAX_USART_IND];
        
        // Private methods
        void recv();
        void xmit();
        
        void update_rts();
        
        // Private static methods
        static char which_usart(USART_t *_usart);
        static USART_t *get_usart(char _usart);
        static PORT_t *get_port(char _usart);
        static char get_txpin(char _usart);
        
public:
        // Public variables
        
        // Public methods
        Usart(USART_t *_usart);
        ~Usart();
        
        void set_tx_buffer(char *_txbuf, size_t _txbuf_size);
        void set_rx_buffer(char *_rxbuf, size_t _rxbuf_size);
        
        void set_rts_pin(PORT_t *_rtsport, int _rtspin);
        void set_cts_pin(PORT_t *_ctsport, int _ctspin);
        
        void check_cts();
        
        void begin(long baud);
        void end();
        
        void put(char c);
        
        size_t available();
        char get();
        int peekc(size_t index = 0);
        int ungetc(int c);
        
        void setup_stream(FILE *stream);
        
        // Static methods
        static int put(char c, FILE *stream);
        static int get(FILE *stream);
        
        static inline void handle_interrupts(char _usart);
        static void handle_interrupts(Usart *_usart);
};

// Prototypes


#endif // __USART_H



