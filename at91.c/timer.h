/************************************************************************/
/* Programmable Interval Timer                                          */
/*                                                                      */
/* timer.h                                                              */
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

#ifndef TIMER_H_
#define TIMER_H_

/*
 * Includes
 */
#include <board.h>
#include <aic/aic.h>
#include <pit/pit.h>
#include "hook.h"

/*
 * Definitions
 */
#define TIMER_PERIOD        1000

/*
 * Typedefs
 */

/**
 * TIMER_HOOK_HANDLER
 * manages the timer hook handler chain
 */
typedef HOOK_HANDLER TIMER_HOOK_HANDLER;

/*
 * Globals
 */
extern volatile unsigned long jiffies;
extern TIMER_HOOK_HANDLER *timer_first_hook;

/*
 * Prototypes
 */

/**
 * timer_handler
 * programmable interval timer interrupt handler
 */
void timer_handler(void);

/**
 * configure_timer
 * programmable interval timer configuration
 */
void configure_timer(void);

/**
 * wait
 * delay routine based on programmable interval timer
 */
void wait (unsigned long ms);

/**
 * timer_add_hook
 * adds a timer interrupt hook
 */
void timer_add_hook(TIMER_HOOK_HANDLER *hook, void (*handler)());

/**
 * timer_remove_hook
 * removes a timer interrupt hook
 */
void timer_remove_hook(TIMER_HOOK_HANDLER *hook);



#endif /* TIMER_H_ */

