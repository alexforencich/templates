/************************************************************************/
/* Template                                                             */
/*                                                                      */
/* main.cpp                                                             */
/*                                                                      */
/* Alex Forencich <alex@alexforencich.com>                              */
/*                                                                      */
/* Copyright (c) 2012 Alex Forencich                                    */
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
#include "main.h"

/*
 * Definitions
 */

/*
 * Typedefs
 */

/*
 * Globals
 */

#define USART_TX_BUF_SIZE 64
#define USART_RX_BUF_SIZE 64
char usart_txbuf[USART_TX_BUF_SIZE];
char usart_rxbuf[USART_RX_BUF_SIZE];
Usart usart(AT91C_BASE_US0);

/*
 * Prototypes
 */

/*
 * Methods
 */

// exception handlers
// hang
extern "C" void __data_abort(void)
{
        while (1);
}

extern "C" void __prefetch_abort(void)
{
        while (1);
}

extern "C" void __undef(void)
{
        while (1);
}

extern "C" void __swi(void)
{
        while (1);
}

extern "C" void spuriousInterruptHandler(void)
{
        // do nothing, but actually return
        // otherwise, processor will hang randomly for no apparent reason
        // Note: could cause problems, but not hanging is much more productive
        // than entering a placeholder interrupt handler
}

// system initialization
void init()
{
        // set up PIT for jiffies count
        configure_timer();
        
        // Spurious interrupt handler
        // overrides default 'black hole' handler,
        // necessary when something generates an interrupt storm
        AT91C_BASE_AIC->AIC_SPU = (unsigned int) spuriousInterruptHandler;
        
        // set up LED IO pins
        AT91C_BASE_PMC->PMC_PCER = (1 << PIN_LED_ID);
        PIN_LED_PIO->PIO_PER = PIN_LED_MASK;
        PIN_LED_PIO->PIO_PPUDR = PIN_LED_MASK;
        PIN_LED_PIO->PIO_OER = PIN_LED_OESET;
        PIN_LED_PIO->PIO_ODR = PIN_LED_OECLR;
        PIN_LED_PIO->PIO_CODR = PIN_LED_MASK;

        // set up USART
        usart.set_tx_buffer(usart_txbuf, USART_TX_BUF_SIZE);
        usart.set_rx_buffer(usart_rxbuf, USART_RX_BUF_SIZE);
        usart.begin(115200);
        
}

// Main Method
int main()
{
        // initialize everything
        init();
        
        PIN_LED_PIO->PIO_SODR = PIN_LED_1_MASK;
        
        usart.write_string("Startup\n");
        
        while (1)
        {
                PIN_LED_PIO->PIO_SODR = PIN_LED_1_MASK;
                wait(500);
                PIN_LED_PIO->PIO_CODR = PIN_LED_1_MASK;
                wait(500);
        }
        
        return 0;
}




