/************************************************************************/
/* Hook Hander Chain                                                    */
/*                                                                      */
/* hook.h                                                               */
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

#ifndef HOOK_H_
#define HOOK_H_

/*
 * Includes
 */

/*
 * Typedefs
 */
/**
 * HOOK_HANDLER
 * a link in a hook handler chain
 */
struct HOOK_HANDLER_ST {
        void (*hook_handler)();
        struct HOOK_HANDLER_ST *next_handler;
        struct HOOK_HANDLER_ST *prev_handler;
};

typedef struct HOOK_HANDLER_ST HOOK_HANDLER;

/*
 * Globals
 */

/*
 * Prototypes
 */

/**
 * hook_add
 * adds a hook chain link
 */
extern void hook_add(HOOK_HANDLER **first_hook, HOOK_HANDLER *hook, void (*handler)());

/**
 * hook_remove
 * removes a hook chain link
 */
extern void hook_remove(HOOK_HANDLER **first_hook, HOOK_HANDLER *hook);

/**
 * hook_call_chain
 * calls a complete hook chain
 */
extern void hook_call_chain(HOOK_HANDLER *first_hook);



#endif /* HOOK_H_ */
