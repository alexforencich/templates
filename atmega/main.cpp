/************************************************************************/
/* Template                                                             */
/*                                                                      */
/* main.cpp                                                             */
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

#include "main.h"

// USART

#define USART_TX_BUF_SIZE 64
#define USART_RX_BUF_SIZE 64
char usart_txbuf[USART_TX_BUF_SIZE];
char usart_rxbuf[USART_RX_BUF_SIZE];
CREATE_USART(usart, USART0_INDEX);
FILE usart_stream;

// I2C

I2c i2c;

// Timer

unsigned long jiffies = 0;

// Timer tick ISR (1 kHz)
#ifdef __AVR_XMEGA__
ISR(TCC0_OVF_vect)
#else // __AVR_XMEGA__
ISR(TIMER1_CAPT_vect)
#endif // __AVR_XMEGA__
{
        // Timers
        jiffies++;
	
        if (jiffies % 50 == 0)
                LED_PORT ^= LED_PIN_bm;
        
}

int64_t get_jiffies_us()
{
#ifdef __AVR_XMEGA__
        uint16_t t = TCC0.CNT;
#else // __AVR_XMEGA__
        uint16_t t = TCNT1;
#endif // __AVR_XMEGA__
        uint32_t tj = jiffies;
#ifdef __AVR_XMEGA__
        if (TCC0.INTFLAGS & TC0_OVFIF_bm) t += TCC0.PER;
#else // __AVR_XMEGA__
        if (TIFR1 & _BV(ICF1)) t += ICR1;
#endif // __AVR_XMEGA__
        return ((int64_t)tj * 1000) + (t >> 4);
}

int64_t get_jiffies_ns()
{
#ifdef __AVR_XMEGA__
        uint16_t t = TCC0.CNT;
#else // __AVR_XMEGA__
        uint16_t t = TCNT1;
#endif // __AVR_XMEGA__
        uint32_t tj = jiffies;
#ifdef __AVR_XMEGA__
        if (TCC0.INTFLAGS & TC0_OVFIF_bm) t += TCC0.PER;
#else // __AVR_XMEGA__
        if (TIFR1 & _BV(ICF1)) t += ICR1;
#endif // __AVR_XMEGA__
        return ((int64_t)tj * 1000000) + (((uint32_t)t * 125) >> 1);
}

// Init everything
void init(void)
{
        // Init pins
        LED_PORT &= ~LED_PIN_bm;
        LED_DDR |= LED_PIN_bm;
        
        // UARTs
        usart.set_tx_buffer(usart_txbuf, USART_TX_BUF_SIZE);
        usart.set_rx_buffer(usart_rxbuf, USART_RX_BUF_SIZE);
        usart.begin(USART_BAUD_RATE);
        usart.setup_stream(&usart_stream);
        
        // TCC
#ifdef __AVR_XMEGA__
        // tick rate 32 MHz = timer resolution 31.25 ns
        // rollover rate 1 kHz = tick period 1 kHz
        TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
        TCC0.CTRLB = 0;
        TCC0.CTRLC = 0;
        TCC0.CTRLD = 0;
        TCC0.CTRLE = 0;
        TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
        TCC0.INTCTRLB = 0;
        TCC0.CNT = 0;
        TCC0.PER = 32000;
#else // __AVR_XMEGA__
        // tick rate 16 MHz = timer resolution 62.5 ns
        // rollover rate 1 kHz = tick period 1 kHz
        TCCR1A = 0;
        TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
        TCCR1C = 0;
        TCNT1 = 0;
        ICR1 = 16000;
        TIMSK1 = _BV(ICIE1);
#endif // __AVR_XMEGA__
        
        // I2C
        i2c.begin(400000L);
        
        // SPI
        //spi.begin(SPI_MODE_2_gc, SPI_PRESCALER_DIV4_gc, 1);
        
        // CS line
        //SPI_CS_PORT.OUTSET = SPI_CS_DEV_PIN_bm;
        //SPI_CS_PORT.DIRSET = SPI_CS_DEV_PIN_bm;
        
        sei();
}

int main(void)
{
        _delay_ms(50);
        
        init();
        
        LED_PORT |= LED_PIN_bm;
        
        usart.write_string("Hello World\n");
        
        fprintf_P(&usart_stream, PSTR("x = %d"), 5);
        
        while (1)
        {
                // main loop
                
        }
        
}


