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

#include "usart.h"


// Statics
Usart *Usart::usart_list[MAX_USART_IND+1];

#ifdef __AVR_XMEGA__

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
        return USART_INVALID_IND;
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

#endif // __AVR_XMEGA__

#ifdef __AVR_XMEGA__
Usart::Usart(USART_t *_usart) :
        usart(_usart),
#else // __AVR_XMEGA__
Usart::Usart(char _usart) :
        usart_ind(_usart),
#endif // __AVR_XMEGA__
        txbuf(0),
        txbuf_size(0),
        txbuf_head(0),
        txbuf_tail(0),
        rxbuf(0),
        rxbuf_size(0),
        rxbuf_head(0),
        rxbuf_tail(0),
#ifdef __AVR_XMEGA__
        rtsport(0),
        ctsport(0),
        rtspin_bm(0),
        ctspin_bm(0),
#endif // __AVR_XMEGA__
        nonblocking(0),
        flags(USART_TX_QUEUE_FULL | USART_RX_QUEUE_FULL)
{
#ifdef __AVR_XMEGA__
        usart_ind = which_usart(_usart);
#endif // __AVR_XMEGA__
        usart_list[(int)usart_ind] = this;
        
#ifndef __AVR_XMEGA__
        if (_usart == 0)
        {
                udr = &UDR0;
                ucsra = &UCSR0A;
                ucsrb = &UCSR0B;
                ucsrc = &UCSR0C;
                ubrr = &UBRR0;
        }
        #ifdef UDR1
        else if (_usart == 1)
        {
                udr = &UDR1;
                ucsra = &UCSR1A;
                ucsrb = &UCSR1B;
                ucsrc = &UCSR1C;
                ubrr = &UBRR1;
        }
        #endif // UDR1
#endif // __AVR_XMEGA__
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
        
        if (flags & USART_RUNNING)
        {
#ifdef __AVR_XMEGA__
                usart->CTRLA &= ~USART_RXCINTLVL_gm;
                usart->CTRLA |= USART_RXCINTLVL_MED_gc;
#else // __AVR_XMEGA__
                *ucsrb |= _BV(RXCIE0);
#endif // __AVR_XMEGA__
        }
}


#ifdef __AVR_XMEGA__
void Usart::set_rts_pin(PORT_t *_rtsport, int _rtspin)
{
        rtsport = _rtsport;
        rtspin_bm = 1 << _rtspin;
        rtsport->DIRSET = rtspin_bm;
        
}


void Usart::set_cts_pin(PORT_t *_ctsport, int _ctspin)
{
        ctsport = _ctsport;
        ctspin_bm = 1 << _ctspin;
        ctsport->DIRCLR = ctspin_bm;
}
#endif // __AVR_XMEGA__


void Usart::set_nonblocking(uint8_t nb)
{
        nonblocking = nb;
}


#ifdef __AVR_XMEGA__
void Usart::update_rts()
{
        if (rtsport == 0)
                return;
        if (txbuf_size == 0)
        {
                // no buffer, so just assert it
                rtsport->OUTCLR = rtspin_bm;
        }
        else
        {
                // define 'getting full' as 3/4
                if (available() > ((rxbuf_size >> 1) + (rxbuf_size >> 2)))
                {
                        rtsport->OUTSET = rtspin_bm;
                }
                else
                {
                        rtsport->OUTCLR = rtspin_bm;
                }
        }
}


void Usart::check_cts()
{
        if (ctsport == 0)
                return;
        if (ctsport->IN & ctspin_bm)
        {
                // deasserted, disable transmit
                usart->CTRLA &= ~USART_DREINTLVL_gm;
        }
        else
        {
                // asserted, enable transmit
                if (!(flags & USART_TX_QUEUE_EMPTY))
                        usart->CTRLA |= USART_DREINTLVL_MED_gc;
        }
}
#endif // __AVR_XMEGA__


void __attribute__ ((noinline)) Usart::begin(long baud, char _clk2x, char puen)
{
#ifdef __AVR_XMEGA__
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
        
        if (puen)
        {
                *(&(port->PIN0CTRL)+(pin-1)) = PORT_OPC_PULLUP_gc;
        }
        
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
        else if (_clk2x)
        {
                bsel = ((F_CPU) / ((uint32_t)baud * 8) - 1);
                bscale = 0;
                clk2x = 1;
        }
        else
        {
                bsel = ((F_CPU) / ((uint32_t)baud * 16) - 1);
                bscale = 0;
                clk2x = 0;
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
        
        if (rxbuf_size > 0)
        {
                usart->CTRLA = USART_RXCINTLVL_MED_gc;
        }
        else
        {
                usart->CTRLA = 0;
        }
        
        flags |= USART_RUNNING;
        
        update_rts();
#else // __AVR_XMEGA__
        if (_clk2x)
        {
                *ubrr = ((uint32_t)((F_CPU) + ((uint32_t)(baud) * 4UL)) / ((uint32_t)(baud) * 8UL) - 1);
                *ucsra = _BV(U2X0);
        }
        else
        {
                *ubrr = ((uint32_t)((F_CPU) + ((uint32_t)(baud) * 8UL)) / ((uint32_t)(baud) * 16UL) - 1);
                *ucsra = 0;
        }
        
        if (rxbuf_size > 0)
        {
                *ucsrb = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
        }
        else
        {
                *ucsrb = _BV(RXEN0) | _BV(TXEN0);
        }
        
        *ucsrc = _BV(UCSZ01) | _BV(UCSZ00);
        
        flags |= USART_RUNNING;
#endif // __AVR_XMEGA__
}


void __attribute__ ((noinline)) Usart::end()
{
#ifdef __AVR_XMEGA__
        usart->CTRLA = 0;
        usart->CTRLB = 0;
#else // __AVR_XMEGA__
        *ucsrb = 0;
#endif // __AVR_XMEGA__
        flags &= ~USART_RUNNING;
}


void Usart::recv()
{
        char tmp;
#ifdef __AVR_XMEGA__
        if (usart->STATUS & USART_RXCIF_bm)
#else // __AVR_XMEGA__
        if (*ucsra & _BV(RXC0))
#endif // __AVR_XMEGA__
        {
#ifdef __AVR_XMEGA__
                tmp = usart->DATA;
#else // __AVR_XMEGA__
                tmp = *udr;
#endif // __AVR_XMEGA__
                if (!(flags & USART_RX_QUEUE_FULL))
                {
                        rxbuf[rxbuf_head++] = tmp;
                        flags &= ~USART_RX_QUEUE_EMPTY;
                        if (rxbuf_head >= rxbuf_size)
                                rxbuf_head = 0;
                        if (rxbuf_head == rxbuf_tail)
                                flags |= USART_RX_QUEUE_FULL;
                }
#ifdef __AVR_XMEGA__
                update_rts();
#endif // __AVR_XMEGA__
        }
}


void Usart::xmit()
{
        if (!(flags & USART_TX_QUEUE_EMPTY))
        {
#ifdef __AVR_XMEGA__
                usart->DATA = txbuf[txbuf_tail++];
#else // __AVR_XMEGA__
                *udr = txbuf[txbuf_tail++];
#endif // __AVR_XMEGA__
                flags &= ~USART_TX_QUEUE_FULL;
                if (txbuf_tail >= txbuf_size)
                        txbuf_tail = 0;
                if (txbuf_head == txbuf_tail)
                        flags |= USART_TX_QUEUE_EMPTY;
        }
        if (flags & USART_TX_QUEUE_EMPTY)
        {
#ifdef __AVR_XMEGA__
                usart->CTRLA &= ~USART_DREINTLVL_gm;
#else // __AVR_XMEGA__
                *ucsrb &= ~_BV(UDRIE0);
#endif // __AVR_XMEGA__
        }
}


size_t Usart::free()
{
        int cnt = txbuf_tail - txbuf_head;
        if (cnt < 0 || flags & USART_TX_QUEUE_EMPTY)
                cnt += txbuf_size;
        return cnt;
}


void Usart::put(char c)
{
        uint8_t saved_status = 0;
        
        if (!(flags & USART_RUNNING) || (!(SREG & SREG_I) && (flags & USART_TX_QUEUE_FULL)))
                return;
        
        // blocking write if no buffer
        if (txbuf_size == 0)
        {
#ifdef __AVR_XMEGA__
                while (!(usart->STATUS & USART_DREIF_bm)) { };
                usart->DATA = c;
#else // __AVR_XMEGA__
                while (!(*ucsra & _BV(UDRE0))) { };
                *udr = c;
#endif // __AVR_XMEGA__
                return;
        }
        
        // return if nonblocking
        if ((flags & USART_TX_QUEUE_FULL) && nonblocking)
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
        
#ifdef __AVR_XMEGA__
        usart->CTRLA |= USART_DREINTLVL_MED_gc;
#else // __AVR_XMEGA__
        *ucsrb |= _BV(UDRIE0);
#endif // __AVR_XMEGA__
        
        SREG = saved_status;
}


size_t Usart::available()
{
        int cnt = rxbuf_head - rxbuf_tail;
        if (cnt < 0 || flags & USART_RX_QUEUE_FULL)
                cnt += rxbuf_size;
        return cnt;
}


char Usart::get()
{
        uint8_t saved_status = 0;
        char c;
        
        if (!(flags & USART_RUNNING) || (!(SREG & SREG_I) && (flags & USART_RX_QUEUE_EMPTY)))
                return 0;
        
        // blocking read if no buffer
        if (rxbuf_size == 0)
        {
#ifdef __AVR_XMEGA__
                while (!(usart->STATUS & USART_RXCIF_bm)) { };
                return usart->DATA;
#else // __AVR_XMEGA__
                while (!(*ucsra & _BV(RXC0))) { };
                return *udr;
#endif // __AVR_XMEGA__
        }
        
        // return if nonblocking
        if ((flags & USART_RX_QUEUE_EMPTY) && nonblocking)
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
        
#ifdef __AVR_XMEGA__
        update_rts();
#endif // __AVR_XMEGA__
        
        SREG = saved_status;
        
        return c;
}


int Usart::peek(size_t index)
{
        uint8_t saved_status = 0;
        char c;
        
        if (!(flags & USART_RUNNING) || (flags & USART_RX_QUEUE_EMPTY))
                return EOF;
        
        // return EOF if invalid index
        if (index >= rxbuf_size)
                return EOF;
        
        saved_status = SREG;
        cli();
        
        index += rxbuf_tail;
        if (index >= rxbuf_size)
                index -= rxbuf_size;
        
        c = rxbuf[index];
        
        SREG = saved_status;
        
        return c;
}


int Usart::ungetc(int c)
{
        uint8_t saved_status = 0;
        
        if (c == EOF || flags & USART_RX_QUEUE_FULL || (!(SREG & SREG_I) && (flags & USART_RX_QUEUE_FULL)))
                return EOF;
        
        // return EOF if no buffer
        if (rxbuf_size == 0)
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
                u->put(c);
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
                return u->get();
        }
        return _FDEV_ERR;
}


// static
inline void Usart::handle_interrupts(char _usart)
{
        Usart *u = usart_list[(int)_usart];
        if (u)
        {
#ifdef __AVR_XMEGA__
                USART_t *dev = get_usart(_usart);
                if (dev->STATUS & USART_DREIF_bm)
                        u->xmit();
                if (dev->STATUS & USART_RXCIF_bm)
                        u->recv();
#else // __AVR_XMEGA__
                if (*(u->ucsra) & _BV(UDRE0))
                        u->xmit();
                if (*(u->ucsra) & _BV(RXC0))
                        u->recv();
#endif // __AVR_XMEGA__
        }
}


// static
void Usart::handle_interrupts(Usart *_usart)
{
        if (_usart)
        {
#ifdef __AVR_XMEGA__
                USART_t *dev = _usart->usart;
                if (dev->STATUS & USART_DREIF_bm)
                        _usart->xmit();
                if (dev->STATUS & USART_RXCIF_bm)
                        _usart->recv();
#else // __AVR_XMEGA__
                if (*(_usart->ucsra) & _BV(UDRE0))
                        _usart->xmit();
                if (*(_usart->ucsra) & _BV(RXC0))
                        _usart->recv();
#endif // __AVR_XMEGA__
        }
}


#ifdef USART_CREATE_ALL_ISR
// ISR
#ifdef __AVR_XMEGA__

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

#else // __AVR_XMEGA__

#endif // __AVR_XMEGA__

#endif // USART_CREATE_ALL_ISR



