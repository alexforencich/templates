/************************************************************************/
/* XMEGA USART Driver                                                   */
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

#include "usart.h"


// Statics
Usart *Usart::usart_list[MAX_USART_IND];


char __attribute__ ((noinline)) Usart::which_usart(USART_t *_usart)
{
#if MAX_USART_IND >= USARTC0_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTC0)
                return USARTC0_IND;
#endif
#if MAX_USART_IND >= USARTC1_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTC1)
                return USARTC1_IND;
#endif
#if MAX_USART_IND >= USARTD0_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTD0)
                return USARTD0_IND;
#endif
#if MAX_USART_IND >= USARTD1_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTD1)
                return USARTD1_IND;
#endif
#if MAX_USART_IND >= USARTE0_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTE0)
                return USARTE0_IND;
#endif
#if MAX_USART_IND >= USARTE1_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTE1)
                return USARTE1_IND;
#endif
#if MAX_USART_IND >= USARTF0_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTF0)
                return USARTF0_IND;
#endif
#if MAX_USART_IND >= USARTF1_IND
        if ((uintptr_t)_usart == (uintptr_t)&USARTF1)
                return USARTF1_IND;
#endif
        return 0;
}


USART_t * __attribute__ ((noinline)) Usart::get_usart(char _usart)
{
        switch (_usart)
        {
#if MAX_USART_IND >= USARTC0_IND
                case USARTC0_IND:
                        return &USARTC0;
#endif
#if MAX_USART_IND >= USARTC1_IND
                case USARTC1_IND:
                        return &USARTC1;
#endif
#if MAX_USART_IND >= USARTD0_IND
                case USARTD0_IND:
                        return &USARTD0;
#endif
#if MAX_USART_IND >= USARTD1_IND
                case USARTD1_IND:
                        return &USARTD1;
#endif
#if MAX_USART_IND >= USARTE0_IND
                case USARTE0_IND:
                        return &USARTE0;
#endif
#if MAX_USART_IND >= USARTE1_IND
                case USARTE1_IND:
                        return &USARTE1;
#endif
#if MAX_USART_IND >= USARTF0_IND
                case USARTF0_IND:
                        return &USARTF0;
#endif
#if MAX_USART_IND >= USARTF1_IND
                case USARTF1_IND:
                        return &USARTF1;
#endif
                default:
                        return 0;
        }
}


PORT_t * __attribute__ ((noinline)) Usart::get_port(char _usart)
{
        switch (_usart)
        {
#if MAX_USART_IND >= USARTC0_IND
                case USARTC0_IND:
                        return &PORTC;
#endif
#if MAX_USART_IND >= USARTC1_IND
                case USARTC1_IND:
                        return &PORTC;
#endif
#if MAX_USART_IND >= USARTD0_IND
                case USARTD0_IND:
                        return &PORTD;
#endif
#if MAX_USART_IND >= USARTD1_IND
                case USARTD1_IND:
                        return &PORTD;
#endif
#if MAX_USART_IND >= USARTE0_IND
                case USARTE0_IND:
                        return &PORTE;
#endif
#if MAX_USART_IND >= USARTE1_IND
                case USARTE1_IND:
                        return &PORTE;
#endif
#if MAX_USART_IND >= USARTF0_IND
                case USARTF0_IND:
                        return &PORTF;
#endif
#if MAX_USART_IND >= USARTF1_IND
                case USARTF1_IND:
                        return &PORTF;
#endif
                default:
                        return 0;
        }
}


char __attribute__ ((noinline)) Usart::get_txpin(char _usart)
{
        switch (_usart)
        {
#if MAX_USART_IND >= USARTC0_IND
                case USARTC0_IND:
                        return 3;
#endif
#if MAX_USART_IND >= USARTC1_IND
                case USARTC1_IND:
                        return 7;
#endif
#if MAX_USART_IND >= USARTD0_IND
                case USARTD0_IND:
                        return 3;
#endif
#if MAX_USART_IND >= USARTD1_IND
                case USARTD1_IND:
                        return 7;
#endif
#if MAX_USART_IND >= USARTE0_IND
                case USARTE0_IND:
                        return 3;
#endif
#if MAX_USART_IND >= USARTE1_IND
                case USARTE1_IND:
                        return 7;
#endif
#if MAX_USART_IND >= USARTF0_IND
                case USARTF0_IND:
                        return 3;
#endif
#if MAX_USART_IND >= USARTF1_IND
                case USARTF1_IND:
                        return 7;
#endif
                default:
                        return 0;
        }
}


Usart::Usart(USART_t *_usart) :
        usart(_usart),
        txbuf(0),
        txbuf_size(0),
        txbuf_head(0),
        txbuf_tail(0),
        rxbuf(0),
        rxbuf_size(0),
        rxbuf_head(0),
        rxbuf_tail(0),
        flags(USART_TX_QUEUE_FULL | USART_RX_QUEUE_FULL)
{
        usart_ind = which_usart(_usart);
        usart_list[usart_ind-1] = this;
        
}


Usart::~Usart()
{
        end();
        usart_list[usart_ind-1] = 0;
}


void Usart::set_tx_buffer(char *_txbuf, size_t _txbuf_size)
{
        txbuf = _txbuf;
        txbuf_size = _txbuf_size;
        flags &= ~USART_TX_QUEUE_FULL;
        flags |= USART_TX_QUEUE_EMPTY;
}


void Usart::set_rx_buffer(char *_rxbuf, size_t _rxbuf_size)
{
        rxbuf = _rxbuf;
        rxbuf_size = _rxbuf_size;
        flags &= ~USART_RX_QUEUE_FULL;
        flags |= USART_RX_QUEUE_EMPTY;
}


void __attribute__ ((noinline)) Usart::begin(long baud)
{
        unsigned char pin;
        unsigned char pinmask;
        PORT_t *port;
        unsigned int bsel;
        char bscale;
        char clk2x;
        
        pin = get_txpin(usart_ind);
        port = get_port(usart_ind);
        pinmask = 1 << pin;
        port->DIRSET = pinmask;
        port->DIRCLR = pinmask >> 1;
        
        if ((F_CPU == 2000000L) && (baud == 19200))
        {
                bsel = 353;
                bscale = -6;
                clk2x = 0;
        }
        else if ((F_CPU == 2000000L) && (baud == 38400))
        {
                bsel = 144;
                bscale = -6;
                clk2x = 0;
        }
        else if ((F_CPU == 2000000L) && (baud == 57600))
        {
                bsel = 75;
                bscale = -6;
                clk2x = 0;
        }
        else if ((F_CPU == 2000000L) && (baud == 115200))
        {
                bsel = 5;
                bscale = -6;
                clk2x = 0;
        }
        else if ((F_CPU == 32000000L) && (baud == 19200))
        {
                bsel = 3301;
                bscale = -5;
                clk2x = 0;
        }
        else if ((F_CPU == 32000000L) && (baud == 38400))
        {
                bsel = 3269;
                bscale = -6;
                clk2x = 0;
        }
        else if ((F_CPU == 32000000L) && (baud == 57600))
        {
                bsel = 2158;
                bscale = -6;
                clk2x = 0;
        }
        else if ((F_CPU == 32000000L) && (baud == 115200))
        {
                bsel = 1047;
                bscale = -6;
                clk2x = 0;
        }
        else if (F_CPU == 32000000L)
        {
                bsel = ((F_CPU) / ((uint32_t)baud * 16) - 1);
                bscale = 0;
                clk2x = 0;
        }
        else
        {
                bsel = ((F_CPU) / ((uint32_t)baud * 8) - 1);
                bscale = 0;
                clk2x = 1;
        }
        
        usart->BAUDCTRLA = (bsel & USART_BSEL_gm);
        usart->BAUDCTRLB = ((bscale << USART_BSCALE_gp) & USART_BSCALE_gm) | ((bsel >> 8) & 0x0f);
        
        if (clk2x)
        {
                usart->CTRLB = USART_RXEN_bm | USART_CLK2X_bm | USART_TXEN_bm;
        }
        else
        {
                usart->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
        }
        
        usart->CTRLA = USART_RXCINTLVL_MED_gc;
        flags |= USART_RUNNING;
}


void __attribute__ ((noinline)) Usart::end()
{
        usart->CTRLA = 0;
        usart->CTRLB = 0;
        flags &= ~USART_RUNNING;
}


void Usart::recv()
{
        char tmp;
        if (usart->STATUS & USART_RXCIF_bm)
        {
                tmp = usart->DATA;
                if (!(flags & USART_RX_QUEUE_FULL))
                {
                        rxbuf[rxbuf_head++] = tmp;
                        flags &= ~USART_RX_QUEUE_EMPTY;
                        if (rxbuf_head >= rxbuf_size)
                                rxbuf_head = 0;
                        if (rxbuf_head == rxbuf_tail)
                                flags |= USART_RX_QUEUE_FULL;
                }
        }
}


void Usart::xmit()
{
        if (!(flags & USART_TX_QUEUE_EMPTY))
        {
                usart->DATA = txbuf[txbuf_tail++];
                flags &= ~USART_TX_QUEUE_FULL;
                if (txbuf_tail >= txbuf_size)
                        txbuf_tail = 0;
                if (txbuf_head == txbuf_tail)
                        flags |= USART_TX_QUEUE_EMPTY;
        }
        if (flags & USART_TX_QUEUE_EMPTY)
        {
                usart->CTRLA &= ~USART_DREINTLVL_gm;
        }
}


void (Usart::putc)(char c)
{
        uint8_t saved_status = 0;
        
        if (!(flags & USART_RUNNING))
                return;
        
        while (flags & USART_TX_QUEUE_FULL) { };
        
        saved_status = SREG;
        cli();
        
        txbuf[txbuf_head++] = c;
        flags &= ~USART_TX_QUEUE_EMPTY;
        if (txbuf_head >= txbuf_size)
                txbuf_head = 0;
        if (txbuf_head == txbuf_tail)
                flags |= USART_TX_QUEUE_FULL;
        
        usart->CTRLA |= USART_DREINTLVL_MED_gc;
        
        SREG = saved_status;
}


void Usart::puts(const char *str)
{
        while (*str)
        {
                (putc)(*(str++));
        }
}


int Usart::write(const void *ptr, int num)
{
        int j = num;
        const char *ptr2 = (const char *)ptr;
        if (num == 0 || ptr2 == 0)
                return 0;
        while (num--)
        {
                (putc)(*(ptr2++));
        }
        return j;
}


int Usart::available()
{
        int cnt = rxbuf_head - rxbuf_tail;
        if (cnt < 0 || flags & USART_RX_QUEUE_FULL)
                cnt += rxbuf_size;
        return cnt;
}


char (Usart::getc)()
{
        uint8_t saved_status = 0;
        char c;
        
        if (!(flags & USART_RUNNING))
                return 0;
        
        while (flags & USART_RX_QUEUE_EMPTY) { };
        
        saved_status = SREG;
        cli();
        
        c = rxbuf[rxbuf_tail++];
        flags &= ~USART_RX_QUEUE_FULL;
        if (rxbuf_tail >= rxbuf_size)
                rxbuf_tail = 0;
        if (rxbuf_head == rxbuf_tail)
                flags |= USART_RX_QUEUE_EMPTY;
        
        SREG = saved_status;
        
        return c;
}


int Usart::ungetc(int c)
{
        uint8_t saved_status = 0;
        
        if (c == EOF || flags & USART_RX_QUEUE_FULL)
                return EOF;
        
        saved_status = SREG;
        cli();
        
        rxbuf[rxbuf_head++] = c;
        flags &= ~USART_RX_QUEUE_EMPTY;
        if (rxbuf_head >= rxbuf_size)
                rxbuf_head = 0;
        if (rxbuf_head == rxbuf_tail)
                flags |= USART_RX_QUEUE_FULL;
        
        SREG = saved_status;
        
        return c;
}


void Usart::gets(char *dest)
{
        do
        {
                *(dest++) = (getc)();
        }
        while (*(dest-1) != 0 && *(dest-1) != '\n');
}


int Usart::read(void *dest, int num)
{
        int j = num;
        char *ptr2 = (char *)dest;
        if (num == 0 || ptr2 == 0)
                return 0;
        while (num--)
        {
                *(ptr2++) = (getc)();
        }
        return j;
}


void Usart::setup_stream(FILE *stream)
{
        fdev_setup_stream(stream, put, get, _FDEV_SETUP_RW);
        fdev_set_udata(stream, this);
}


// static
int Usart::put(char c, FILE *stream)
{
        Usart *u;
        u = (Usart *)fdev_get_udata(stream);
        if (u != 0)
        {
                (u->putc)(c);
                return 0;
        }
        return _FDEV_ERR;
}


// static
int Usart::get(FILE *stream)
{
        Usart *u;
        u = (Usart *)fdev_get_udata(stream);
        if (u != 0)
        {
                return (u->getc)();
        }
        return _FDEV_ERR;
}


// static
inline void Usart::handle_interrupts(char _usart)
{
        Usart *u = usart_list[_usart-1];
        if (u)
        {
                USART_t *dev = get_usart(_usart);
                if (dev->STATUS & USART_DREIF_bm)
                        u->xmit();
                if (dev->STATUS & USART_RXCIF_bm)
                        u->recv();
        }
}


// static
void Usart::handle_interrupts(Usart *_usart)
{
        if (_usart)
        {
                USART_t *dev = _usart->usart;
                if (dev->STATUS & USART_DREIF_bm)
                        _usart->xmit();
                if (dev->STATUS & USART_RXCIF_bm)
                        _usart->recv();
        }
}


#ifdef USART_CREATE_ALL_ISR
// ISR
#ifdef USARTC0
ISR(USARTC0_DRE_vect)
{
        Usart::handle_interrupts(USARTC0_IND);
}
ISR(USARTC0_RXC_vect, ISR_ALIASOF(USARTC0_DRE_vect));
#endif
#ifdef USARTC1
ISR(USARTC1_DRE_vect)
{
        Usart::handle_interrupts(USARTC1_IND);
}
ISR(USARTC1_RXC_vect, ISR_ALIASOF(USARTC1_DRE_vect));
#endif
#ifdef USARTD0
ISR(USARTD0_DRE_vect)
{
        Usart::handle_interrupts(USARTD0_IND);
}
ISR(USARTD0_RXC_vect, ISR_ALIASOF(USARTD0_DRE_vect));
#endif
#ifdef USARTD1
ISR(USARTD1_DRE_vect)
{
        Usart::handle_interrupts(USARTD1_IND);
}
ISR(USARTD1_RXC_vect, ISR_ALIASOF(USARTD1_DRE_vect));
#endif
#ifdef USARTE0
ISR(USARTE0_DRE_vect)
{
        Usart::handle_interrupts(USARTE0_IND);
}
ISR(USARTE0_RXC_vect, ISR_ALIASOF(USARTE0_DRE_vect));
#endif
#ifdef USARTE1
ISR(USARTE1_DRE_vect)
{
        Usart::handle_interrupts(USARTE1_IND);
}
ISR(USARTE1_RXC_vect, ISR_ALIASOF(USARTE1_DRE_vect));
#endif
#ifdef USARTF0
ISR(USARTF0_DRE_vect)
{
        Usart::handle_interrupts(USARTF0_IND);
}
ISR(USARTF0_RXC_vect, ISR_ALIASOF(USARTF0_DRE_vect));
#endif
#ifdef USARTF1
ISR(USARTF1_DRE_vect)
{
        Usart::handle_interrupts(USARTF1_IND);
}
ISR(USARTF1_RXC_vect, ISR_ALIASOF(USARTF1_DRE_vect));
#endif

#endif // USART_CREATE_ALL_ISR



