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

/*
 * Includes
 */
#include "usart.h"

/*
 * Globals
 */

// buffers
volatile char usart_transmit_buffer[USART_TRANSMIT_BUFFER_SIZE];
volatile unsigned int usart_transmit_buffer_front = 0;
volatile unsigned int usart_transmit_buffer_next_front = 0;
volatile unsigned int usart_transmit_buffer_xfer_size = 0;
volatile unsigned int usart_transmit_buffer_back = 0;
volatile unsigned int usart_transmit_buffer_length = 0;

volatile char usart_receive_buffer[USART_RECEIVE_BUFFER_SIZE];
volatile unsigned int usart_receive_buffer_front = 0;
volatile unsigned int usart_receive_buffer_back = 0;
volatile unsigned int usart_receive_buffer_length = 0;

volatile unsigned int usart_transmit_total_bytes = 0;
volatile unsigned int usart_receive_total_bytes = 0;

unsigned int usart_transmitter_is_present = 0;

/*
 * Methods
 */

/**
 * usart_init
 * Initializes the USART module and its associated pins
 */
void usart_init(long baud)
{
        long l;
        
        // init pins
        // clock
        AT91C_BASE_PMC->PMC_PCER = (1 << PIN_USART_ID);
        
        // disable PIO control over pins
        PIN_USART_PIO->PIO_PDR = PIN_USART_PDR;
        
        // connect proper peripherals
        PIN_USART_PIO->PIO_ASR = PIN_USART_ASR;
        PIN_USART_PIO->PIO_BSR = PIN_USART_BSR;
        
        // enable pull-up on receive pin
        PIN_USART_PIO->PIO_PPUER = PIN_USART_RXD_MASK;
        
        // init USART module
        // enable clock
        AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_US0);
        
        // reset module
        AT91C_BASE_US0->US_CR = AT91C_US_RSTRX |        // reset receiver
                        AT91C_US_RSTTX |                // reset transmitter
                        AT91C_US_RXDIS |                // disable receiver
                        AT91C_US_TXDIS;                 // disable transmitter
        
        // set mode
        AT91C_BASE_US0->US_MR = AT91C_US_USMODE_HWHSH |         // hardware handshaking (CTS and RTS)
                        AT91C_US_CLKS_CLOCK |                   // use MCK
                        AT91C_US_CHRL_8_BITS |                  // 8 bit characters
                        AT91C_US_NBSTOP_1_BIT |                 // one stop bit
                        AT91C_US_PAR_NONE;                      // no parity
        
        // set up BRG
        // CD * 8 + FP = CLK / 2 * baud
        l = BOARD_MCK / (2 * baud + 1);
        AT91C_BASE_US0->US_BRGR = ((l & 0x7) << 16) | ((l >> 3) & 0xffff);
        //AT91C_BASE_US0->US_BRGR = 26;
        
        // set up interrupts
        // Disable the interrupt on the interrupt controller
        AIC_DisableIT(AT91C_ID_US0);
        
        // Configure the AIC for PIT interrupts
        AIC_ConfigureIT(AT91C_ID_US0, USART_INT_PRIOR, usart_isr);
        
        // Enable the interrupt on the interrupt controller
        AIC_EnableIT(AT91C_ID_US0);
        
        // enable USART peripheral interrupts
        // interrupts will be enabled as needed
        AT91C_BASE_US0->US_IER = 0;
        
        // queue
        usart_transmit_buffer_front = 0;
        usart_transmit_buffer_next_front = 0;
        usart_transmit_buffer_xfer_size = 0;
        usart_transmit_buffer_back = 0;
        usart_transmit_buffer_length = 0;
        
        usart_receive_buffer_front = 0;
        usart_receive_buffer_back = 0;
        usart_receive_buffer_length = 0;
        
        // set up PDC for reception
        AT91C_BASE_US0->US_RPR = ((unsigned long)&usart_receive_buffer) + usart_receive_buffer_back;
        AT91C_BASE_US0->US_RCR = 1;
        AT91C_BASE_US0->US_PTCR = AT91C_PDC_RXTEN;
        AT91C_BASE_US0->US_IER = AT91C_US_RXBUFF;
        
        // enable module and proper pins
        AT91C_BASE_US0->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;  // enable transmitter and receiver
}

/**
 * usart_isr
 * USART Interrupt Service Routine
 */
void usart_isr()
{
        unsigned long usart_csr;

        usart_csr = AT91C_BASE_US0->US_CSR;

        if (usart_csr & AT91C_US_TXBUFE)
        {
                // Transmit buffer empty interrupt

                // disable PDC
                AT91C_BASE_US0->US_PTCR = AT91C_PDC_TXTDIS;

                if (usart_transmit_buffer_length > 0)
                {
                        usart_transmit_buffer_front = usart_transmit_buffer_next_front;
                        usart_transmit_buffer_length -= usart_transmit_buffer_xfer_size;
                        usart_transmit_total_bytes += usart_transmit_buffer_xfer_size;
                        if (usart_transmit_total_bytes > 1)
                                usart_transmitter_is_present = 1;

                        // is there another block?
                        if (usart_transmit_buffer_length > 0)
                        {
                                usart_transmit_buffer_xfer_size = usart_transmit_buffer_length;
                                usart_transmit_buffer_next_front = usart_transmit_buffer_front + usart_transmit_buffer_xfer_size;
                                if (usart_transmit_buffer_next_front >= USART_TRANSMIT_BUFFER_SIZE) {
                                        usart_transmit_buffer_next_front = 0;
                                        usart_transmit_buffer_xfer_size = USART_TRANSMIT_BUFFER_SIZE - usart_transmit_buffer_front;
                                }
                                // if nothing is being sent, send it on its way...
                                AT91C_BASE_US0->US_TPR = ((unsigned long)&usart_transmit_buffer) + usart_transmit_buffer_front;
                                AT91C_BASE_US0->US_TCR = usart_transmit_buffer_xfer_size;

                                AT91C_BASE_US0->US_PTCR = AT91C_PDC_TXTEN;

                        }
                        else
                        {
                                // disable USART interrupt if there isn't any data waiting
                                AT91C_BASE_US0->US_IDR = AT91C_US_TXBUFE;
                        }

                }
                else
                {
                        // disable USART interrupt if there isn't any data waiting
                        AT91C_BASE_US0->US_IDR = AT91C_US_TXBUFE;
                }
        }

        if (usart_csr & AT91C_US_RXBUFF)
        {
                // Receive buffer full interrupt

                // disable PDC
                AT91C_BASE_US0->US_PTCR = AT91C_PDC_RXTDIS;

                if (usart_receive_buffer_length < USART_RECEIVE_BUFFER_SIZE - 1)
                {
                        usart_receive_buffer_back++;
                        usart_receive_buffer_length++;
                        usart_receive_total_bytes++;

                        if (usart_receive_buffer_back >= USART_RECEIVE_BUFFER_SIZE) {
                                usart_receive_buffer_back = 0;
                        }

                        //if (usart_receive_total_bytes > 1)
                        //        usart_transmitter_is_present = 1;

                        // is there space to store more?
                        if (usart_receive_buffer_length < USART_RECEIVE_BUFFER_SIZE - 1)
                        {
                                AT91C_BASE_US0->US_RPR = ((unsigned long)&usart_receive_buffer) + usart_receive_buffer_back;
                                AT91C_BASE_US0->US_RCR = 1;
                                AT91C_BASE_US0->US_PTCR = AT91C_PDC_RXTEN;
                        }
                        else
                        {
                                // disable USART interrupt if the buffer is full
                                AT91C_BASE_US0->US_IDR = AT91C_US_RXBUFF;
                        }

                }
                else
                {
                        // disable USART interrupt if the buffer is full
                        AT91C_BASE_US0->US_IDR = AT91C_US_RXBUFF;
                }
        }
        
}

/**
 * usart_putc
 * sends a character over the USART
 */
void usart_putc(char ch)
{
        unsigned long mask;

        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && usart_transmit_buffer_xfer_size + usart_transmit_total_bytes -
                        AT91C_BASE_US0->US_TCR < 2 && usart_transmit_buffer_length >= USART_TRANSMIT_BUFFER_SIZE)
                return;

        // wait for space
        while (usart_transmit_buffer_length >= USART_TRANSMIT_BUFFER_SIZE) {}

        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

        // queue it up
        usart_transmit_buffer[usart_transmit_buffer_back++] = ch;
        usart_transmit_buffer_length++;
        if (usart_transmit_buffer_back >= USART_TRANSMIT_BUFFER_SIZE) {
                usart_transmit_buffer_back = 0;
        }

        if (!(AT91C_BASE_US0->US_PTSR & AT91C_PDC_TXTEN))
        {
                usart_transmit_buffer_xfer_size = usart_transmit_buffer_length;
                usart_transmit_buffer_next_front = usart_transmit_buffer_front + usart_transmit_buffer_xfer_size;
                if (usart_transmit_buffer_next_front >= USART_TRANSMIT_BUFFER_SIZE) {
                        usart_transmit_buffer_next_front = 0;
                        usart_transmit_buffer_xfer_size = USART_TRANSMIT_BUFFER_SIZE - usart_transmit_buffer_front;
                }
                // if nothing is being sent, send it on its way...
                AT91C_BASE_US0->US_TPR = ((unsigned long)&usart_transmit_buffer) + usart_transmit_buffer_front;
                AT91C_BASE_US0->US_TCR = usart_transmit_buffer_xfer_size;

                AT91C_BASE_US0->US_PTCR = AT91C_PDC_TXTEN;

        }

        // turn on the interrupt
        AT91C_BASE_US0->US_IER = AT91C_US_TXBUFE;

        AT91C_BASE_AIC->AIC_IECR = mask;
}

/**
 * usart_puts
 * sends a string over the USART
 */
void usart_puts(const char *str)
{
        while (*str != 0)
        {
                usart_putc(*(str++));
        }
}

/**
 * usart_write
 * writes num characters into the USART buffer
 * waits for space as needed
 * returns the number of bytes read
 */
int usart_write(const void *ptr, int num)
{
        int n = num;
        const char *ptr2 = ptr;
        
        while (n-- > 0)
        {
                usart_putc(*(ptr2++));
        }
        
        return num;
}

/**
 * usart_getc
 * fetches a character out of the USART buffer
 * waits for one to arrive if the buffer is empty
 */
char usart_getc()
{
        unsigned long mask;
        char ch;

        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && usart_transmit_buffer_xfer_size + usart_transmit_total_bytes -
                        AT91C_BASE_US0->US_TCR < 2 && usart_transmit_buffer_length >= USART_TRANSMIT_BUFFER_SIZE)
                return 0;

        // wait for char
        while (usart_receive_buffer_length == 0) {}

        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

        // dequeue it
        ch = usart_receive_buffer[usart_receive_buffer_front++];
        usart_receive_buffer_length--;
        if (usart_receive_buffer_front >= USART_RECEIVE_BUFFER_SIZE) {
                usart_receive_buffer_front = 0;
        }

        if (!(AT91C_BASE_US0->US_PTSR & AT91C_PDC_RXTEN) && usart_receive_buffer_length < USART_RECEIVE_BUFFER_SIZE - 1)
        {
                AT91C_BASE_US0->US_RPR = ((unsigned long)&usart_receive_buffer) + usart_receive_buffer_back;
                AT91C_BASE_US0->US_RCR = 1;
                AT91C_BASE_US0->US_PTCR = AT91C_PDC_RXTEN;

                // turn on the interrupt
                AT91C_BASE_US0->US_IER = AT91C_US_RXBUFF;
        }

        AT91C_BASE_AIC->AIC_IECR = mask;

        return ch;
}

/**
 * usart_gets
 * fetches characters out of the USART buffer until
 * either (num - 1) characters are fetched or a newline
 * is encountered.  Waits for more characters if needed.
 * Returns null if there isn't a receiver present.
 */
char *usart_gets(char *str, int num)
{
        char last;
        
        if (num == 0)
                return str;
        
        do
        {
                last = usart_getc();
                *(str++) = last;
                num--;
        }
        while (last != 0 && last != '\r' && last != '\n' && num > 0);
        
        return str;
}

/**
 * usart_read
 * fetches num characters out of the USART buffer
 * returns number of characters read
 */
int usart_read(void *ptr, int num)
{
        int n = num;
        char *ptr2 = ptr;
        
        while (n-- > 0)
        {
                *(ptr2++) = usart_getc();
        }
        
        return num;
}

/**
 * usart_peek
 * looks at the next character in the USART buffer
 * waits for one to arrive if the buffer is empty
 */
char usart_peek()
{
        unsigned long mask;
        char ch;

        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && usart_transmit_buffer_xfer_size + usart_transmit_total_bytes -
                        AT91C_BASE_US0->US_TCR < 2 && usart_transmit_buffer_length >= USART_TRANSMIT_BUFFER_SIZE)
                return 0;

        // wait for char
        while (usart_receive_buffer_length == 0) {}

        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

        // dequeue it
        ch = usart_receive_buffer[usart_receive_buffer_front];

        AT91C_BASE_AIC->AIC_IECR = mask;

        return ch;
}

/**
 * usart_ungetc
 * returns a character to the USART receive buffer
 */
void usart_ungetc(char ch)
{
        unsigned long mask;

        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && usart_transmit_buffer_xfer_size + usart_transmit_total_bytes -
                        AT91C_BASE_US0->US_TCR < 2 && usart_transmit_buffer_length >= USART_TRANSMIT_BUFFER_SIZE)
                return;

        // wait for char
        while (usart_receive_buffer_length >= USART_RECEIVE_BUFFER_SIZE) {}

        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

        // equeue it
        usart_receive_buffer[usart_receive_buffer_back++] = ch;
        usart_receive_buffer_length++;
        if (usart_receive_buffer_back >= USART_RECEIVE_BUFFER_SIZE) {
                usart_receive_buffer_back = 0;
        }

        if (!(AT91C_BASE_US0->US_PTSR & AT91C_PDC_RXTEN) && usart_receive_buffer_length == USART_RECEIVE_BUFFER_SIZE - 1)
        {
                // turn off the interrupt
                AT91C_BASE_US0->US_IDR = AT91C_US_RXBUFF;
        }

        AT91C_BASE_AIC->AIC_IECR = mask;
}






