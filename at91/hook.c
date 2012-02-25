/************************************************************************/
/* Hook Hander Chain                                                    */
/*                                                                      */
/* hook.c                                                               */
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
#include "hook.h"

/*
 * Globals
 */

/*
 * Methods
 */

/**
 * hook_add
 * adds a hook chain link
 */
void hook_add(HOOK_HANDLER **first_hook, HOOK_HANDLER *hook, void (*handler)())
{
        hook->hook_handler = handler;
        hook->prev_handler = *first_hook;
        hook->next_handler = (void *)0;
        if (*first_hook != (void *)0)
        {
                (*first_hook)->next_handler = hook;
        }
        *first_hook = hook;
}

/**
 * hook_remove
 * removes a hook chain link
 */
void hook_remove(HOOK_HANDLER **first_hook, HOOK_HANDLER *hook)
{
        if (hook->prev_handler != (void *)0)
                hook->prev_handler->next_handler = hook->next_handler;
        if (hook->next_handler != (void *)0)
                hook->next_handler->prev_handler = hook->prev_handler;
        if (hook == *first_hook)
                *first_hook = hook->prev_handler;
}

/**
 * hook_call_chain
 * calls a complete hook chain
 */
void hook_call_chain(HOOK_HANDLER *first_hook)
{
        HOOK_HANDLER *hook = first_hook;
        
        while (hook != (void *)0)
        {
                if (hook->hook_handler != (void *)0)
                        hook->hook_handler();
                hook = hook->prev_handler;
        }
}


