/************************************************************************/
/* Programmable Interval Timer                                          */
/*                                                                      */
/* timer.c                                                              */
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
#include "timer.h"

/*
 * Globals
 */
volatile unsigned long jiffies = 0;

// timer hook handler chain
TIMER_HOOK_HANDLER *timer_first_hook;

/*
 * Methods
 */

/**
 * timer_handler
 * programmable interval timer interrupt handler
 */
void timer_handler(void)
{
        unsigned long pivr = 0;
        unsigned long pisr = 0;

        // Read the PISR
        pisr = PIT_GetStatus() & AT91C_PITC_PITS;
        if (pisr != 0) {
                // Read the PIVR. It acknowledges the IT
                pivr = PIT_GetPIVR();

                // Add to jiffies PICNT: the number of occurrences of periodic intervals
                // since the last read of PIT_PIVR
                jiffies += (pivr >> 20);

                // call the hook if one is set
                hook_call_chain(timer_first_hook);
        }
}

/**
 * configure_timer
 * programmable interval timer configuration
 */
void configure_timer(void)
{
        // initialize timer hook
        timer_first_hook = (void *)0;

        // Initialize and enable the PIT
        PIT_Init(TIMER_PERIOD, BOARD_MCK / 1000000);

        // Disable the interrupt on the interrupt controller
        AIC_DisableIT(AT91C_ID_SYS);

        // Configure the AIC for PIT interrupts
        AIC_ConfigureIT(AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, timer_handler);

        // Enable the interrupt on the interrupt controller
        AIC_EnableIT(AT91C_ID_SYS);

        // Enable the interrupt on the pit
        PIT_EnableIT();

        // Enable the pit
        PIT_Enable();
}

/**
 * wait
 * delay routine based on programmable interval timer
 */
void wait (unsigned long ms)
{
        volatile unsigned long current_time = jiffies;
        unsigned long prev_jiffies;
        unsigned long target_time = current_time + ms;

        // Handle the counter overflow
        if (target_time < current_time) {
                prev_jiffies = current_time;
                while (prev_jiffies <= jiffies) {
                prev_jiffies = jiffies;
                }
        }
        // Loop until the target time is reached
        while (jiffies < target_time);
}

/**
 * timer_add_hook
 * adds a timer interrupt hook
 */
void timer_add_hook(TIMER_HOOK_HANDLER *hook, void (*handler)())
{
        hook_add(&timer_first_hook, hook, handler);
}

/**
 * timer_remove_hook
 * removes a timer interrupt hook
 */
void timer_remove_hook(TIMER_HOOK_HANDLER *hook)
{
        hook_remove(&timer_first_hook, hook);
}


