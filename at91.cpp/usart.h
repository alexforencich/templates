/************************************************************************/
/* USART Driver                                                         */
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

#ifndef USART_H_
#define USART_H_

/*
 * Includes
 */
#include <stdint.h>

#include <board.h>
#include <aic/aic.h>

#include "iostream.h"

/*
 * Definitions
 */
#define USART_INT_PRIOR                 5

#define USART0_IND 0
#define USART1_IND 1

#define USART_INVALID_IND -1

#define MAX_USART_IND 1

#define USART0_TXD_MSK  (1 << 0)
#define USART0_RXD_MSK  (1 << 1)
#define USART0_RTS_MSK  (1 << 3)
#define USART0_CTS_MSK  (1 << 4)
#define USART0_TXRX     (USART0_TXD_MSK | USART0_RXD_MSK)
#define USART0_RTSCTS   (USART0_RTS_MSK | USART0_CTS_MSK)

#define USART1_TXD_MSK  (1 << 0)
#define USART1_RXD_MSK  (1 << 1)
#define USART1_RTS_MSK  (1 << 3)
#define USART1_CTS_MSK  (1 << 4)
#define USART1_TXRX     (USART1_TXD_MSK | USART1_RXD_MSK)
#define USART1_RTSCTS   (USART1_RTS_MSK | USART1_CTS_MSK)

#define USART_TX_QUEUE_EMPTY 0x10
#define USART_TX_QUEUE_FULL 0x20
#define USART_RX_QUEUE_EMPTY 0x40
#define USART_RX_QUEUE_FULL 0x80
#define USART_RUNNING 0x01

/*
 * Typedefs
 */

/*
 * Globals
 */
extern volatile unsigned int usart_transmit_total_bytes;
extern volatile unsigned int usart_receive_total_bytes;

extern unsigned int usart_transmitter_is_present;

/*
 * Prototypes
 */

// Usart class
class Usart : public IOStream
{
private:
        // Per object data
        AT91PS_USART usart;
        
        char usart_ind;
        char *txbuf;
        size_t txbuf_size;
        size_t txbuf_head;
        size_t txbuf_tail;
        size_t txbuf_next_tail;
        size_t txbuf_xfer_size;
        char *rxbuf;
        size_t rxbuf_size;
        size_t rxbuf_head;
        size_t rxbuf_tail;
        
        uint8_t nonblocking;
        
        volatile char flags;
        
        // Static data
        static Usart *usart_list[MAX_USART_IND+1];
        
        // Private methods
        void recv();
        void xmit();
        
        // Private static methods
        static char which_usart(AT91PS_USART _usart);
        static AT91PS_USART get_usart(char _usart);
        static AT91PS_PIO get_pio(char _usart);
        static int get_id(char _usart);
        //static char get_txpin(char _usart);
        
public:
        // Public variables
        
        // Public methods
        Usart(AT91PS_USART _usart);
        ~Usart();
        
        void set_tx_buffer(char *_txbuf, size_t _txbuf_size);
        void set_rx_buffer(char *_rxbuf, size_t _rxbuf_size);
        
        void set_nonblocking(uint8_t nb);
        
        void begin(long baud, char mode = 0);
        void end();
        
        void put(char c);
        
        size_t available();
        char get();
        int peek(size_t index = 0);
        int ungetc(int c);
        
//         void setup_stream(FILE *stream);
        
        // Static methods
//         static int put(char c, FILE *stream);
//         static int get(FILE *stream);
        
         static inline void handle_interrupts(char _usart);
         static void handle_interrupts(Usart *_usart);
         
         static void usart0_isr();
#if MAX_USART_IND > 1
         static void usart1_isr();
#endif
         
};


#endif /* USART_H_ */
