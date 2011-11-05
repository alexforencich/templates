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
CREATE_USART(usart, UART_DEVICE_PORT);
FILE usart_stream;

// SPI

Spi spi(&SPI_DEV);

// I2C

I2c i2c(&I2C_DEV);

// Timer

unsigned long jiffies = 0;

// Production signature row access
uint8_t SP_ReadCalibrationByte( uint8_t index )
{
        uint8_t result;
        NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
        result = pgm_read_byte(index);
        NVM_CMD = NVM_CMD_NO_OPERATION_gc;
        return result;
}

// User signature row access
uint8_t SP_ReadUserSigRow( uint8_t index )
{
        uint8_t result;
        NVM_CMD = NVM_CMD_READ_USER_SIG_ROW_gc;
        result = pgm_read_byte(index);
        NVM_CMD = NVM_CMD_NO_OPERATION_gc;
        return result;
}

// Timer tick ISR (1 kHz)
ISR(TCC0_OVF_vect)
{
        // Timers
        jiffies++;
	
        if (jiffies % 50 == 0)
                LED_PORT.OUTTGL = 0x03;
        
}

// Init everything
void init(void)
{
        // clock
        OSC.CTRL |= OSC_RC32MEN_bm; // turn on 32 MHz oscillator
        while (!(OSC.STATUS & OSC_RC32MRDY_bm)) { }; // wait for it to start
        CCP = CCP_IOREG_gc;
        CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // swtich osc
        DFLLRC32M.CTRL = DFLL_ENABLE_bm; // turn on DFLL
        
        // disable JTAG
        CCP = CCP_IOREG_gc;
        MCU.MCUCR = 1;
        
        // Init pins
        LED_PORT.OUTCLR = LED_USR_0_PIN_bm | LED_USR_1_PIN_bm | LED_USR_2_PIN_bm | LED_USR_3_PIN_bm |
                LED_USR_4_PIN_bm | LED_USR_5_PIN_bm | LED_USR_6_PIN_bm | LED_USR_7_PIN_bm;
        LED_PORT.DIRSET = LED_USR_0_PIN_bm | LED_USR_1_PIN_bm | LED_USR_2_PIN_bm | LED_USR_3_PIN_bm |
                LED_USR_4_PIN_bm | LED_USR_5_PIN_bm | LED_USR_6_PIN_bm | LED_USR_7_PIN_bm;
        
        // UARTs
        usart.set_tx_buffer(usart_txbuf, USART_TX_BUF_SIZE);
        usart.set_rx_buffer(usart_rxbuf, USART_RX_BUF_SIZE);
        usart.begin(UART_BAUD_RATE);
        usart.setup_stream(&usart_stream);
        
        // ADC setup
        ADCA.CTRLA = ADC_DMASEL_OFF_gc | ADC_FLUSH_bm;
        ADCA.CTRLB = ADC_CONMODE_bm | ADC_RESOLUTION_12BIT_gc;
        ADCA.REFCTRL = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm;
        ADCA.EVCTRL = ADC_SWEEP_0123_gc | ADC_EVSEL_0123_gc | ADC_EVACT_SWEEP_gc;
        ADCA.PRESCALER = ADC_PRESCALER_DIV64_gc;
        ADCA.CALL = SP_ReadCalibrationByte(PROD_SIGNATURES_START + ADCACAL0_offset);
        ADCA.CALH = SP_ReadCalibrationByte(PROD_SIGNATURES_START + ADCACAL1_offset);
        
        ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
        ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
        ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
        ADCA.CH0.RES = 0;
        
        ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
        ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
        ADCA.CH1.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
        ADCA.CH1.RES = 0;
        
        ADCA.CH2.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
        ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
        ADCA.CH2.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
        ADCA.CH2.RES = 0;
        
        ADCA.CH3.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
        ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;
        ADCA.CH3.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
        ADCA.CH3.RES = 0;
        
        //ADCA.CTRLA |= ADC_ENABLE_bm;
        //ADCA.CTRLB |= ADC_FREERUN_bm;
        
        // TCC
        TCC0.CTRLA = TC_CLKSEL_DIV256_gc;
        TCC0.CTRLB = 0;
        TCC0.CTRLC = 0;
        TCC0.CTRLD = 0;
        TCC0.CTRLE = 0;
        TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
        TCC0.INTCTRLB = 0;
        TCC0.CNT = 0;
        TCC0.PER = 125;
        
        // ADC trigger on TCC0 overflow
        //EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;
        //EVSYS.CH0CTRL = 0;
        
        // I2C
        i2c.begin(400000L);
        
        // SPI
        spi.begin(SPI_MODE_2_gc, SPI_PRESCALER_DIV4_gc, 1);
        
        // CS line
        SPI_CS_PORT.OUTSET = SPI_CS_DEV_PIN_bm;
        SPI_CS_PORT.DIRSET = SPI_CS_DEV_PIN_bm;
        
        // Interrupts
        PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm;
        
        sei();
}

int main(void)
{
        _delay_ms(50);
        
        init();
        
        LED_PORT.OUT = 0x01;
        
        usart.puts("Hello World\n");
        
        fprintf_P(&usart_stream, PSTR("x = %d"), 5);
        
        while (1)
        {
                // main loop
                LED_PORT.OUTTGL = 0x03;
                _delay_ms(50);
        }
        
}


