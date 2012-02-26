/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution. 
 * 
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission. 
 * 
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "pio_it.h"
#include <aic/aic.h>
#include <board.h>
#include <aic/aic.h>

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
/// \internal Returns the current value of a register.
#define READ(peripheral, register)          (peripheral->register)
/// \internal Modifies the current value of a register.
#define WRITE(peripheral, register, value)  (peripheral->register = value)

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Configures a PIO controller to generate an interrupt when the level of one
/// or more pin(s) changes.
/// \note The interrupt is *not* enabled by this function; PIO_EnableIT must be
/// called for that.
/// \param pin  Pointer to a Pin instance describing one or more pins.
/// \param priority  Interrupt priority, between 0 (lowest) and 7.
/// \param handler  Pointer to the interrupt handler function.
//------------------------------------------------------------------------------
void PIO_ConfigureIT(const Pin *pin,
                            unsigned char priority,
                            void (*handler)( void ))
{
    AT91C_BASE_PMC->PMC_PCER = 1 << pin->id;
    AIC_ConfigureIT(pin->id, (priority & AT91C_AIC_PRIOR), handler);
}

//------------------------------------------------------------------------------
/// Enables interrupt generation when the level on one or more pin(s) changes.
/// \note PIO_ConfigureIT must be called before this function.
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
void PIO_EnableIT(const Pin *pin)
{
    AIC_EnableIT(pin->id);
    WRITE(pin->pio, PIO_IER, pin->mask);
}

//------------------------------------------------------------------------------
/// Disables interrupt generation for one or more pin(s).
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
void PIO_DisableIT(const Pin *pin)
{
    WRITE(pin->pio, PIO_IDR, pin->mask);
}

