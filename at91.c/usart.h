/************************************************************************/
/* USART Driver                                                         */
/*                                                                      */
/* usart.c                                                              */
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
#include <board.h>
#include <aic/aic.h>

/*
 * Definitions
 */
#define USART_TRANSMIT_BUFFER_SIZE      128
#define USART_RECEIVE_BUFFER_SIZE       128

#define USART_INT_PRIOR                 5

/*
 * Typedefs
 */

/*
 * Globals
 */

// buffers
extern volatile char usart_transmit_buffer[USART_TRANSMIT_BUFFER_SIZE];
extern volatile unsigned int usart_transmit_buffer_front;
extern volatile unsigned int usart_transmit_buffer_next_front;
extern volatile unsigned int usart_transmit_buffer_xfer_size;
extern volatile unsigned int usart_transmit_buffer_back;
extern volatile unsigned int usart_transmit_buffer_length;

extern volatile char usart_receive_buffer[USART_RECEIVE_BUFFER_SIZE];
extern volatile unsigned int usart_receive_buffer_front;
extern volatile unsigned int usart_receive_buffer_back;
extern volatile unsigned int usart_receive_buffer_length;

extern volatile unsigned int usart_transmit_total_bytes;
extern volatile unsigned int usart_receive_total_bytes;

extern unsigned int usart_transmitter_is_present;

/*
 * Prototypes
 */

/**
 * usart_init
 * Initializes the USART module and its associated pins
 */
extern void usart_init(long baud);

/**
 * usart_isr
 * USART Interrupt Service Routine
 */
extern void usart_isr();

/**
 * usart_putc
 * sends a character over the  USART
 */
extern void usart_putc(char ch);

/**
 * usart_puts
 * sends a string over the  USART
 */
extern void usart_puts(const char *str);

/**
 * usart_write
 * writes num characters into the USART buffer
 * waits for space as needed
 * returns the number of bytes read
 */
extern int usart_write(const void *ptr, int num);

/**
 * usart_getc
 * fetches a character out of the USART buffer
 * waits for one to arrive if the buffer is empty
 */
extern char usart_getc();

/**
 * usart_gets
 * fetches characters out of the USART buffer until
 * either (num - 1) characters are fetched or a newline
 * is encountered.  Waits for more characters if needed.
 */
extern char *usart_gets(char *str, int num);

/**
 * usart_read
 * fetches num characters out of the USART buffer
 * returns number of characters read
 */
extern int usart_read(void *ptr, int num);

/**
 * usart_peek
 * looks at the next character in the USART buffer
 * waits for one to arrive if the buffer is empty
 */
extern char usart_peek();

/**
 * usart_ungetc
 * returns a character to the USART receive buffer
 */
extern void usart_ungetc(char ch);




#endif /* USART_H_ */
