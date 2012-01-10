/************************************************************************/
/* XMEGA I2C Driver                                                     */
/*                                                                      */
/* i2c.cpp                                                              */
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

#include "i2c.h"


// Statics
I2c *I2c::i2c_list[MAX_TWI_IND];


char __attribute__ ((noinline)) I2c::which_twi(TWI_t *_twi)
{
#if MAX_TWI_IND >= TWIC_IND
        if ((uintptr_t)_twi == (uintptr_t)&TWIC)
                return TWIC_IND;
#endif
#if MAX_TWI_IND >= TWIE_IND
        if ((uintptr_t)_twi == (uintptr_t)&TWIE)
                return TWIE_IND;
#endif
#if MAX_TWI_IND >= TWID_IND
        if ((uintptr_t)_twi == (uintptr_t)&TWID)
                return TWID_IND;
#endif
#if MAX_TWI_IND >= TWIF_IND
        if ((uintptr_t)_twi == (uintptr_t)&TWIF)
                return TWIF_IND;
#endif
        return 0;
}


TWI_t * __attribute__ ((noinline)) I2c::get_twi(char _twi)
{
        switch (_twi)
        {
#if MAX_TWI_IND >= TWIC_IND
                case TWIC_IND:
                        return &TWIC;
#endif
#if MAX_TWI_IND >= TWIE_IND
                case TWIE_IND:
                        return &TWIE;
#endif
#if MAX_TWI_IND >= TWID_IND
                case TWID_IND:
                        return &TWID;
#endif
#if MAX_TWI_IND >= TWIF_IND
                case TWIF_IND:
                        return &TWIF;
#endif
                default:
                        return 0;
        }
}


PORT_t * __attribute__ ((noinline)) I2c::get_port(char _twi)
{
        switch (_twi)
        {
#if MAX_TWI_IND >= TWIC_IND
                case TWIC_IND:
                        return &PORTC;
#endif
#if MAX_TWI_IND >= TWIE_IND
                case TWIE_IND:
                        return &PORTE;
#endif
#if MAX_TWI_IND >= TWID_IND
                case TWID_IND:
                        return &PORTD;
#endif
#if MAX_TWI_IND >= TWIF_IND
                case TWIF_IND:
                        return &PORTF;
#endif
                default:
                        return 0;
        }
}

I2c::I2c(TWI_t *_twi) :
        twi(_twi),
        flags(0)
{
        twi_ind = which_twi(twi);
        i2c_list[twi_ind-1] = this;
}


I2c::~I2c()
{
        end();
        i2c_list[twi_ind-1] = 0;
}


void __attribute__ ((noinline)) I2c::begin(uint32_t baud)
{
        flags = I2C_MODE_MASTER;
        
        twi->MASTER.CTRLA = TWI_MASTER_INTLVL_OFF_gc | TWI_MASTER_ENABLE_bm;
        twi->MASTER.CTRLB = TWI_MASTER_TIMEOUT_DISABLED_gc;
        twi->MASTER.BAUD = (F_CPU / (2 * baud)) - 5;
        twi->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}


void __attribute__ ((noinline)) I2c::end()
{
        twi->MASTER.CTRLA = 0;
        
        flags = 0;
}


void I2c::start_write(uint8_t addr)
{
        if (flags & I2C_STATE_ACTIVE)
        {
                twi->MASTER.CTRLC = TWI_MASTER_CMD_REPSTART_gc;
        }
        
        twi->MASTER.ADDR = (addr << 1) | 0x00;
        
        flags |= I2C_STATE_ACTIVE;
        
        I2C_WAIT_WRITE_MASTER();
}


void I2c::start_read(uint8_t addr)
{
        if (flags & I2C_STATE_ACTIVE)
        {
                twi->MASTER.CTRLC = TWI_MASTER_CMD_REPSTART_gc;
        }
        
        twi->MASTER.ADDR = (addr << 1) | 0x01;
        
        flags |= I2C_STATE_ACTIVE;
        
        I2C_WAIT_READ_MASTER();
}


void I2c::stop()
{
        if (flags & I2C_STATE_ACTIVE)
        {
                twi->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc | TWI_MASTER_ACKACT_bm;
                flags &= ~I2C_STATE_ACTIVE;
        }
}


void I2c::put(char c)
{
        if (flags & I2C_STATE_ACTIVE)
        {
                twi->MASTER.DATA = c;
                twi->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
                I2C_WAIT_WRITE_MASTER();
        }
}


char I2c::get()
{
        char c = 0;
        if (flags & I2C_STATE_ACTIVE)
        {
                if (!(twi->MASTER.STATUS & TWI_MASTER_RIF_bm))
                {
                        twi->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
                        I2C_WAIT_READ_MASTER();
                }
                c = twi->MASTER.DATA;
        }
        return c;
}


void I2c::setup_stream(FILE *stream)
{
        fdev_setup_stream(stream, put, get, _FDEV_SETUP_RW);
        fdev_set_udata(stream, this);
}


// static
int I2c::put(char c, FILE *stream)
{
        I2c *u;
        u = (I2c *)fdev_get_udata(stream);
        if (u != 0)
        {
                u->put(c);
                return 0;
        }
        return _FDEV_ERR;
}


// static
int I2c::get(FILE *stream)
{
        I2c *u;
        u = (I2c *)fdev_get_udata(stream);
        if (u != 0)
        {
                return u->get();
        }
        return _FDEV_ERR;
}

