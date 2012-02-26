/************************************************************************/
/* USART Driver                                                         */
/*                                                                      */
/* usart.cpp                                                            */
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

volatile unsigned int usart_transmit_total_bytes = 0;
volatile unsigned int usart_receive_total_bytes = 0;

unsigned int usart_transmitter_is_present = 0;

/*
 * Methods
 */

// static
Usart *Usart::usart_list[MAX_USART_IND+1];


// static
char __attribute__ ((noinline)) Usart::which_usart(AT91PS_USART _usart)
{
#if MAX_USART_IND >= USART0_IND
        if ((uintptr_t)_usart == (uintptr_t)AT91C_BASE_US0)
                return USART0_IND;
#endif
#if MAX_USART_IND >= USART1_IND
        if ((uintptr_t)_usart == (uintptr_t)AT91C_BASE_US1)
                return USART1_IND;
#endif
        return USART_INVALID_IND;
}


// static
AT91PS_USART __attribute__ ((noinline)) Usart::get_usart(char _usart)
{
        switch (_usart)
        {
#if MAX_USART_IND >= USART0_IND
                case USART0_IND:
                        return AT91C_BASE_US0;
#endif
#if MAX_USART_IND >= USART1_IND
                case USART1_IND:
                        return AT91C_BASE_US0;
#endif
                default:
                        return 0;
        }
}


// static
AT91PS_PIO __attribute__ ((noinline)) Usart::get_pio(char _usart)
{
        switch (_usart)
        {
#if MAX_USART_IND >= USART0_IND
                case USART0_IND:
                        return AT91C_BASE_PIOA;
#endif
#if MAX_USART_IND >= USART1_IND
                case USART1_IND:
                        return AT91C_BASE_PIOA;
#endif
                default:
                        return 0;
        }
}


// static
int __attribute__ ((noinline)) Usart::get_id(char _usart)
{
        switch (_usart)
        {
#if MAX_USART_IND >= USART0_IND
                case USART0_IND:
                        return AT91C_ID_US0;
#endif
#if MAX_USART_IND >= USART1_IND
                case USART1_IND:
                        return AT91C_ID_US1;
#endif
                default:
                        return 0;
        }
}


Usart::Usart(AT91PS_USART _usart) :
        usart(_usart),
        txbuf(0),
        txbuf_size(0),
        txbuf_head(0),
        txbuf_tail(0),
        txbuf_next_tail(0),
        rxbuf(0),
        rxbuf_size(0),
        rxbuf_head(0),
        rxbuf_tail(0),
        nonblocking(0),
        flags(USART_TX_QUEUE_FULL | USART_RX_QUEUE_FULL)
{
        usart_ind = which_usart(_usart);
        usart_list[(int)usart_ind] = this;
}


Usart::~Usart()
{
        end();
        usart_list[(int)usart_ind] = 0;
}


void Usart::set_tx_buffer(char *_txbuf, size_t _txbuf_size)
{
        txbuf = _txbuf;
        txbuf_size = _txbuf_size;
        txbuf_head = 0;
        txbuf_tail = 0;
        flags &= ~USART_TX_QUEUE_FULL;
        flags |= USART_TX_QUEUE_EMPTY;
}


void Usart::set_rx_buffer(char *_rxbuf, size_t _rxbuf_size)
{
        rxbuf = _rxbuf;
        rxbuf_size = _rxbuf_size;
        rxbuf_head = 0;
        rxbuf_tail = 0;
        flags &= ~USART_RX_QUEUE_FULL;
        flags |= USART_RX_QUEUE_EMPTY;
        
//         if (flags & USART_RUNNING)
//         {
//                 usart->CTRLA &= ~USART_RXCINTLVL_gm;
//                 usart->CTRLA |= USART_RXCINTLVL_MED_gc;
//         }
}


/**
 * Usart::begin
 * Initializes the USART module and its associated pins
 */
void Usart::begin(long baud, char mode)
{
        long l;
        AT91PS_PIO pio;
        int id;
        
        // init IO
        switch (usart_ind)
        {
                case USART0_IND:
                        pio = AT91C_BASE_PIOA;
                        id = AT91C_ID_US0;
                        
                        // init pins
                        // clock
                        AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
                        
                        // disable PIO control over pins
                        pio->PIO_PDR = (USART0_TXRX | USART0_RTSCTS);
                        
                        // connect proper peripherals
                        pio->PIO_ASR = (USART0_TXRX | USART0_RTSCTS);
                        pio->PIO_BSR = 0;
                        
                        // enable pull-up on receive pin
                        pio->PIO_PPUER = USART0_RXD_MSK;
                        
                        break;
                case USART1_IND:
                        pio = AT91C_BASE_PIOA;
                        id = AT91C_ID_US1;
                        
                        // init pins
                        // clock
                        AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);
                        
                        // disable PIO control over pins
                        pio->PIO_PDR = (USART1_TXRX | USART1_RTSCTS);
                        
                        // connect proper peripherals
                        pio->PIO_ASR = (USART1_TXRX | USART1_RTSCTS);
                        pio->PIO_BSR = 0;
                        
                        // enable pull-up on receive pin
                        pio->PIO_PPUER = USART1_RXD_MSK;
                        
                        break;
        }
        
        // init USART module
        // enable clock
        AT91C_BASE_PMC->PMC_PCER = (1 << id);
        
        // reset module
        usart->US_CR = AT91C_US_RSTRX |         // reset receiver
                        AT91C_US_RSTTX |        // reset transmitter
                        AT91C_US_RXDIS |        // disable receiver
                        AT91C_US_TXDIS;         // disable transmitter
        
        // set mode
        usart->US_MR = AT91C_US_USMODE_HWHSH |          // hardware handshaking (CTS and RTS)
                        AT91C_US_CLKS_CLOCK |           // use MCK
                        AT91C_US_CHRL_8_BITS |          // 8 bit characters
                        AT91C_US_NBSTOP_1_BIT |         // one stop bit
                        AT91C_US_PAR_NONE;              // no parity
        
        // set up BRG
        // CD * 8 + FP = CLK / 2 * baud
        l = BOARD_MCK / (2 * baud + 1);
        usart->US_BRGR = ((l & 0x7) << 16) | ((l >> 3) & 0xffff);
        
        // set up interrupts
        // Disable the interrupt on the interrupt controller
        AIC_DisableIT(id);
        
        // Configure the AIC for PIT interrupts
        AIC_ConfigureIT(id, USART_INT_PRIOR, usart0_isr);
        
        // Enable the interrupt on the interrupt controller
        AIC_EnableIT(id);
        
        // enable USART peripheral interrupts
        // interrupts will be enabled as needed
        usart->US_IER = 0;
        
        // queue
        txbuf_tail = 0;
        txbuf_head = 0;
        txbuf_next_tail = 0;
        
        rxbuf_tail = 0;
        rxbuf_head = 0;
        
        // set up PDC for reception
        usart->US_RPR = ((unsigned long)rxbuf) + rxbuf_head;
        usart->US_RCR = 1;
        usart->US_PTCR = AT91C_PDC_RXTEN;
        usart->US_IER = AT91C_US_RXBUFF;
        
        // enable module and proper pins
        usart->US_CR = AT91C_US_RXEN | AT91C_US_TXEN;  // enable transmitter and receiver
        
        flags |= USART_RUNNING;
}


void Usart::end()
{
        usart->US_CR = 0; // disable it
        
        flags &= ~USART_RUNNING;
}


void Usart::recv()
{
        // disable PDC
        usart->US_PTCR = AT91C_PDC_RXTDIS;

        if (!(flags & USART_RX_QUEUE_FULL))
        {
                rxbuf_head++;
                flags &= ~USART_RX_QUEUE_EMPTY;
                usart_receive_total_bytes++;
                
                if (rxbuf_head >= rxbuf_size)
                        rxbuf_head = 0;
                if (rxbuf_head == rxbuf_tail)
                        flags |= USART_RX_QUEUE_FULL;
                
                //if (usart_receive_total_bytes > 1)
                //        usart_transmitter_is_present = 1;
                
                // is there space to store more?
                if (!(flags & USART_RX_QUEUE_FULL))
                {
                        usart->US_RPR = ((unsigned long)rxbuf) + rxbuf_head;
                        usart->US_RCR = 1;
                        usart->US_PTCR = AT91C_PDC_RXTEN;
                }
        }
        
        if (flags & USART_RX_QUEUE_FULL)
        {
                // disable USART interrupt if the buffer is full
                usart->US_IDR = AT91C_US_RXBUFF;
        }
}


void Usart::xmit()
{
        // disable PDC
        usart->US_PTCR = AT91C_PDC_TXTDIS;

        if (!(flags & USART_TX_QUEUE_EMPTY))
        {
                txbuf_tail = txbuf_next_tail;
                flags &= ~USART_TX_QUEUE_FULL;
                
                if (txbuf_head == txbuf_tail)
                        flags |= USART_TX_QUEUE_EMPTY;
                
                usart_transmit_total_bytes += txbuf_xfer_size;
                if (usart_transmit_total_bytes > 1)
                        usart_transmitter_is_present = 1;
                
                // is there another block?
                if (!(flags & USART_TX_QUEUE_EMPTY))
                {
                        txbuf_xfer_size = txbuf_head - txbuf_tail;
                        txbuf_next_tail = txbuf_tail + txbuf_xfer_size;
                        if (txbuf_tail > txbuf_head) {
                                txbuf_next_tail = 0;
                                txbuf_xfer_size = txbuf_size - txbuf_tail;
                        }
                        // if nothing is being sent, send it on its way...
                        usart->US_TPR = ((unsigned long)txbuf) + txbuf_tail;
                        usart->US_TCR = txbuf_xfer_size;

                        usart->US_PTCR = AT91C_PDC_TXTEN;

                }
        }
        
        if (flags & USART_TX_QUEUE_EMPTY)
        {
                // disable USART interrupt if there isn't any data waiting
                usart->US_IDR = AT91C_US_TXBUFE;
        }
}


/**
 * Usart::put
 * sends a character over the USART
 */
void Usart::put(char c)
{
        unsigned long mask;
        
        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && txbuf_xfer_size + usart_transmit_total_bytes -
                        usart->US_TCR < 2 && flags & USART_TX_QUEUE_FULL)
                return;
        
        // wait for space
        while (flags & USART_TX_QUEUE_FULL) {}
        
        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;
        
        // queue it up
        txbuf[txbuf_head++] = c;
        flags &= ~USART_TX_QUEUE_EMPTY;
        if (txbuf_head >= txbuf_size)
                txbuf_head = 0;
        if (txbuf_head == txbuf_tail)
                flags |= USART_TX_QUEUE_FULL;
        
        if (!(usart->US_PTSR & AT91C_PDC_TXTEN))
        {
                txbuf_xfer_size = txbuf_head - txbuf_tail;
                txbuf_next_tail = txbuf_tail + txbuf_xfer_size;
                if (txbuf_tail > txbuf_head) {
                        txbuf_next_tail = 0;
                        txbuf_xfer_size = txbuf_size - txbuf_tail;
                }
                // if nothing is being sent, send it on its way...
                usart->US_TPR = ((unsigned long)txbuf) + txbuf_tail;
                usart->US_TCR = txbuf_xfer_size;
                
                usart->US_PTCR = AT91C_PDC_TXTEN;

        }
        
        // turn on the interrupt
        usart->US_IER = AT91C_US_TXBUFE;
        
        AT91C_BASE_AIC->AIC_IECR = mask;
}


size_t Usart::available()
{
        int cnt = rxbuf_head - rxbuf_tail;
        if (cnt < 0 || flags & USART_RX_QUEUE_FULL)
                cnt += rxbuf_size;
        return cnt;
}


/**
 * Usart::get
 * fetches a character out of the USART buffer
 * waits for one to arrive if the buffer is empty
 */
char Usart::get()
{
        unsigned long mask;
        char ch;
        
        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && txbuf_xfer_size + usart_transmit_total_bytes -
                        usart->US_TCR < 2 && flags & USART_TX_QUEUE_FULL)
                return 0;
        
        // wait for char
        while (flags & USART_RX_QUEUE_EMPTY) {}
        
        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;
        
        // dequeue it
        ch = rxbuf[rxbuf_tail++];
        flags &= ~USART_RX_QUEUE_FULL;
        if (rxbuf_tail >= rxbuf_size)
                rxbuf_tail = 0;
        if (rxbuf_tail == rxbuf_head)
                flags |= USART_RX_QUEUE_EMPTY;
        
        if (!(usart->US_PTSR & AT91C_PDC_RXTEN) && !(flags & USART_RX_QUEUE_FULL))
        {
                usart->US_RPR = ((unsigned long)rxbuf) + rxbuf_head;
                usart->US_RCR = 1;
                usart->US_PTCR = AT91C_PDC_RXTEN;
        
                // turn on the interrupt
                usart->US_IER = AT91C_US_RXBUFF;
        }
        
        AT91C_BASE_AIC->AIC_IECR = mask;
        
        return ch;
}


int Usart::peek(size_t index)
{
        unsigned long mask;
        char ch;

        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && txbuf_xfer_size + usart_transmit_total_bytes -
                        usart->US_TCR < 2 && flags & USART_TX_QUEUE_FULL)
                return 0;

        // wait for char
        while (flags & USART_RX_QUEUE_EMPTY) {}

        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

        // dequeue it
        ch = rxbuf[rxbuf_tail];

        AT91C_BASE_AIC->AIC_IECR = mask;

        return ch;
}


/**
 * Usart::ungetc
 * returns a character to the USART receive buffer
 */
int Usart::ungetc(int c)
{
        unsigned long mask;

        // if there isn't a transmitter present, then don't wait up
        if (usart_transmitter_is_present == 0 && txbuf_xfer_size + usart_transmit_total_bytes -
                        usart->US_TCR < 2 && flags & USART_TX_QUEUE_FULL)
                return EOF;

        // if full, return
        if (flags & USART_RX_QUEUE_FULL)
                return EOF;

        // critical code: disable interrupts
        mask = AT91C_BASE_AIC->AIC_IMR;
        AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;

        // equeue it
        rxbuf[rxbuf_head++] = c;
        flags &= ~USART_RX_QUEUE_EMPTY;
        if (rxbuf_head >= rxbuf_size)
                rxbuf_head = 0;
        if (rxbuf_head == rxbuf_tail)
                flags |= USART_RX_QUEUE_FULL;

        if (!(usart->US_PTSR & AT91C_PDC_RXTEN) && flags & USART_RX_QUEUE_FULL)
        {
                // turn off the interrupt
                usart->US_IDR = AT91C_US_RXBUFF;
        }

        AT91C_BASE_AIC->AIC_IECR = mask;
        
        return c;
}


// static
inline void Usart::handle_interrupts(char _usart)
{
        Usart *u = usart_list[(int)_usart];
        handle_interrupts(u);
}


// static
void Usart::handle_interrupts(Usart *_usart)
{
        unsigned long usart_csr;
        
        if (_usart)
        {
                usart_csr = _usart->usart->US_CSR;

                if (usart_csr & AT91C_US_TXBUFE)
                        _usart->xmit();
                if (usart_csr & AT91C_US_RXBUFF)
                        _usart->recv();
        }
}


// static
void Usart::usart0_isr()
{
        handle_interrupts((char)USART0_IND);
}


#if MAX_USART_IND > 1
//static
void Usart::usart1_isr()
{
        handle_interrupts((char)USART1_IND);
}
#endif




