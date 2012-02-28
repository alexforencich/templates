/************************************************************************/
/* XMEGA SPI Driver                                                     */
/*                                                                      */
/* spi.cpp                                                              */
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

#include "spi.h"


// Statics
Spi *Spi::spi_list[MAX_SPI_IND];


char __attribute__ ((noinline)) Spi::which_spi(SPI_t *_spi)
{
#if MAX_SPI_IND >= SPIC_IND
        if ((uintptr_t)_spi == (uintptr_t)&SPIC)
                return SPIC_IND;
#endif
#if MAX_SPI_IND >= SPID_IND
        if ((uintptr_t)_spi == (uintptr_t)&SPID)
                return SPID_IND;
#endif
#if MAX_SPI_IND >= SPIE_IND
        if ((uintptr_t)_spi == (uintptr_t)&SPIE)
                return SPIE_IND;
#endif
#if MAX_SPI_IND >= SPIF_IND
        if ((uintptr_t)_spi == (uintptr_t)&SPIF)
                return SPIF_IND;
#endif
        return 0;
}


SPI_t * __attribute__ ((noinline)) Spi::get_spi(char _spi)
{
        switch (_spi)
        {
#if MAX_SPI_IND >= SPIC_IND
                case SPIC_IND:
                        return &SPIC;
#endif
#if MAX_SPI_IND >= SPID_IND
                case SPID_IND:
                        return &SPID;
#endif
#if MAX_SPI_IND >= SPIE_IND
                case SPIE_IND:
                        return &SPIE;
#endif
#if MAX_SPI_IND >= SPIF_IND
                case SPIF_IND:
                        return &SPIF;
#endif
                default:
                        return 0;
        }
}


PORT_t * __attribute__ ((noinline)) Spi::get_port(char _spi)
{
        switch (_spi)
        {
#if MAX_SPI_IND >= SPIC_IND
                case SPIC_IND:
                        return &PORTC;
#endif
#if MAX_SPI_IND >= SPID_IND
                case SPID_IND:
                        return &PORTD;
#endif
#if MAX_SPI_IND >= SPIE_IND
                case SPIE_IND:
                        return &PORTE;
#endif
#if MAX_SPI_IND >= SPIF_IND
                case SPIF_IND:
                        return &PORTF;
#endif
                default:
                        return 0;
        }
}

Spi::Spi(SPI_t *_spi) :
        spi(_spi),
        flags(0)
{
        spi_ind = which_spi(spi);
        spi_list[spi_ind-1] = this;
}


Spi::~Spi()
{
        end();
        spi_list[spi_ind-1] = 0;
}


void __attribute__ ((noinline)) Spi::begin(uint8_t mode, uint8_t prescl, uint8_t clk2x)
{
        PORT_t *port = get_port(spi_ind);
        flags = SPI_MODE_MASTER;
        
        port->DIRSET = SPI_SS_PIN_bm | SPI_MOSI_PIN_bm | SPI_SCK_PIN_bm;
        port->DIRCLR = SPI_MISO_PIN_bm;
        port->OUTCLR = SPI_SS_PIN_bm;
        port->OUTCLR = SPI_MOSI_PIN_bm | SPI_SCK_PIN_bm;
        
        spi->CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | mode | prescl | (clk2x ? SPI_CLK2X_bm : 0);
        spi->INTCTRL = SPI_INTLVL_OFF_gc;
}


void __attribute__ ((noinline)) Spi::end()
{
        spi->CTRL = 0;
}


char Spi::transceive(char c)
{
        uint8_t saved_status = 0;
        
        saved_status = SREG;
        cli();
        
        spi->DATA = c;
        SPI_WAIT();
        return spi->DATA;
        
        SREG = saved_status;
}


void Spi::put(char c)
{
        uint8_t saved_status = 0;
        
        saved_status = SREG;
        cli();
        
        spi->DATA = c;
        SPI_WAIT();
        
        SREG = saved_status;
}


void Spi::write_word(uint16_t word)
{
        uint8_t saved_status = 0;
        SPI_t *spi2 = spi;
        
        union
        {
                uint16_t w;
                struct
                {
                        uint8_t b0;
                        uint8_t b1;
                } b;
        } word2;
        
        saved_status = SREG;
        cli();
        
        word2.w = word;
        
        spi2->DATA = word2.b.b1;
        asm volatile("nop\n\t"::);
        while (!(spi2->STATUS & SPI_IF_bm)) { };
        spi2->DATA = word2.b.b0;
        asm volatile("nop\n\t"::);
        while (!(spi2->STATUS & SPI_IF_bm)) { };
        
        SREG = saved_status;
}


void Spi::write_dword(uint32_t dword)
{
        uint8_t saved_status = 0;
        SPI_t *spi2 = spi;
        
        union
        {
                uint32_t d;
                struct
                {
                        uint8_t b0;
                        uint8_t b1;
                        uint8_t b2;
                        uint8_t b3;
                } b;
        } dword2;
        
        saved_status = SREG;
        cli();
        
        dword2.d = dword;
        
        spi2->DATA = dword2.b.b3;
        asm volatile("nop\n\t"::);
        while (!(spi2->STATUS & SPI_IF_bm)) { };
        spi2->DATA = dword2.b.b2;
        asm volatile("nop\n\t"::);
        while (!(spi2->STATUS & SPI_IF_bm)) { };
        spi2->DATA = dword2.b.b1;
        asm volatile("nop\n\t"::);
        while (!(spi2->STATUS & SPI_IF_bm)) { };
        spi2->DATA = dword2.b.b0;
        asm volatile("nop\n\t"::);
        while (!(spi2->STATUS & SPI_IF_bm)) { };
        
        SREG = saved_status;
}


char Spi::get()
{
        return transceive(0);
}


void Spi::setup_stream(FILE *stream)
{
        fdev_setup_stream(stream, put, get, _FDEV_SETUP_RW);
        fdev_set_udata(stream, this);
}


// static
int Spi::put(char c, FILE *stream)
{
        Spi *u;
        u = (Spi *)fdev_get_udata(stream);
        if (u != 0)
        {
                u->put(c);
                return 0;
        }
        return _FDEV_ERR;
}


// static
int Spi::get(FILE *stream)
{
        Spi *u;
        u = (Spi *)fdev_get_udata(stream);
        if (u != 0)
        {
                return u->get();
        }
        return _FDEV_ERR;
}

/*
// static
inline void Spi::handle_interrupts(char _spi)
{
        Usart *u = spi_list[_spi-1];
        if (u)
        {
                SPI_t *dev = get_spi(_spi);
                if (dev->STATUS & USART_DREIF_bm)
                        u->xmit();
                if (dev->STATUS & USART_RXCIF_bm)
                        u->recv();
        }
}


// static
void Spi::handle_interrupts(Usart *_spi)
{
        if (_spi)
        {
                SPI_t *dev = _spi->spi;
                if (dev->STATUS & USART_DREIF_bm)
                        _spi->xmit();
                if (dev->STATUS & USART_RXCIF_bm)
                        _spi->recv();
        }
}
*/





