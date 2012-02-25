/************************************************************************/
/* Board Definition File                                                */
/*                                                                      */
/* board.h                                                              */
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

#ifndef BOARD_H_
#define BOARD_H_

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam7s256)
    #include "at91sam7s256/AT91SAM7S256.h"
#else
    #error Board does not support the specified chip.
#endif
#define __inline inline

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

// Name of the board.
#define BOARD_NAME              "Template"

// Frequency of the board main oscillator.
#define BOARD_MAINOSC           18432000

// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               47923200

// pin definitions
// LEDs
#define PIN_LED_1_MASK          (1 << 20)
#define PIN_LED_1_PIO           AT91C_BASE_PIOB
#define PIN_LED_1_ID            AT91C_ID_PIOB
#define PIN_LED_1_OE            1
#define PIN_LED_1_OESET         PIN_LED_1_MASK
#define PIN_LED_1_OECLR         0
#define PIN_LED_1_PER           PIN_LED_1_MASK
#define PIN_LED_1_ASR           0
#define PIN_LED_1_BSR           0
#define PIN_LED_1_PDR           0

#define PIN_LED_MASK            (PIN_LED_1_MASK)
#define PIN_LED_PIO             (PIN_LED_1_PIO)
#define PIN_LED_ID              (PIN_LED_1_ID)
#define PIN_LED_OE              (PIN_LED_1_OE)
#define PIN_LED_OESET           (PIN_LED_1_OESET)
#define PIN_LED_OECLR           (PIN_LED_1_OECLR)
#define PIN_LED_PER             (PIN_LED_1_PER)
#define PIN_LED_ASR             (PIN_LED_1_ASR)
#define PIN_LED_BSR             (PIN_LED_1_BSR)
#define PIN_LED_PDR             (PIN_LED_1_PDR)


#endif   // Board_h     
