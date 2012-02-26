/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation
 *
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
//      Headers
//------------------------------------------------------------------------------

#include "dbgu.h"
#include <stdarg.h>
#include <board.h>

//------------------------------------------------------------------------------
//      Internal functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Outputs a character on the DBGU line.
/// \param c  Character to send.
//------------------------------------------------------------------------------
static void DBGU_PutChar(unsigned char c)
{
    // Wait for the transmitter to be ready
    while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXEMPTY) == 0);
    
    // Send character
    AT91C_BASE_DBGU->DBGU_THR = c;
    
    // Wait for the transfer to complete
    while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXEMPTY) == 0);
}

//------------------------------------------------------------------------------
/// Outputs n times a character on the DBGU line.
/// \param c  Character to send.
/// \param n  Number of times the character shall be sent.
//------------------------------------------------------------------------------
static void DBGU_PutCharN(unsigned char c, signed int n)
{
    while (n > 0) {
        
        DBGU_PutChar(c);
        n--;
    }
}

//------------------------------------------------------------------------------
/// Outputs an unsigned integer through the DBGU line.
/// \param value  Unsigned integer to send.
/// \param width  Minimum size (in characters) that the integer should span.
/// \param filler  Filler character to output if the value size is too small.
//------------------------------------------------------------------------------
static void DBGU_PutUnsignedInteger(unsigned int value,
                                           signed int width,
                                           unsigned char filler)
{
    // Recursively outputs upper digits
    if ((value / 10) != 0) {
        
        DBGU_PutUnsignedInteger(value / 10, width - 1, filler);
    }
    else {
        
        // Fill remaining space
        DBGU_PutCharN(filler, width - 1);
    }
    
    // Output lowest digit
    DBGU_PutChar((char) ((value % 10) + '0'));
}

//------------------------------------------------------------------------------
/// Outputs a signed integer through the DBGU line.
/// \param value  Signed integer value to output.
/// \param width  Minimum width of the output (in number of characters).
/// \param filler  Filler character to send for padding the output (if necessary).
//------------------------------------------------------------------------------
static void DBGU_PutInteger(signed int value,
                                   signed int width,
                                   unsigned char filler)
{
    // Output sign if necessary
    if (value < 0) {

        DBGU_PutChar('-');
        value = -value;
    }

    // Output unsigned value
    DBGU_PutUnsignedInteger((unsigned int) value, width, filler);
}

//------------------------------------------------------------------------------
/// Outputs a string on the DBGU.
///
/// \param string  Null-terminated string to output.
//------------------------------------------------------------------------------
static void DBGU_PutString(char *string)
{
    while (*string != 0) {
        
        DBGU_PutChar(*string);
        string++;
    }
}

//------------------------------------------------------------------------------
/// Outputs an hexadecimal value on the DBGU.
///
/// \param value  Value to output in hexadecimal format.
/// \param width  Minimum width of output (in number of characters).
/// \param filler  Character used for padding the output (if necessary).
/// \param upperCase  If true, outputs the letter in upper-case; otherwise in
///                   lower-case.
//------------------------------------------------------------------------------
static void DBGU_PutHexadecimal(unsigned int  value,
                                       signed int    width,
                                       unsigned char filler,
                                       unsigned char upperCase)
{
    // Recursively output upper digits
    if ((value / 16) != 0) {
        
        DBGU_PutHexadecimal(value / 16, width - 1, filler, upperCase);
    }
    else {

        // Fill remaining space
        DBGU_PutCharN(filler, width - 1);
    }
    
    // Output lowest digit
    if ((value % 16) < 10) {
        
        // Number
        DBGU_PutChar((char) ((value % 16) + '0'));
    }
    else if (upperCase) {
        
        // Uppercase letter
        DBGU_PutChar((char) ((value % 16) - 10 + 'A'));
    }
    else {
        
        // Lowercase letter
        DBGU_PutChar((char) ((value % 16) - 10 + 'a'));
    }
}

//------------------------------------------------------------------------------
/// Outputs an octal value on the DBGU.
/// \param value  Value to output in octal format.
/// \param width  Minimum width of output (in number of characters).
/// \param filler  Filler character for padding the output when necessary.
//------------------------------------------------------------------------------
static void DBGU_PutOctal(unsigned int value,
                                 signed int width,
                                 char filler)
{
    // Recursively output upper digits
    if ((value / 8) > 0) {
        
        DBGU_PutOctal(value / 8, width - 1, filler);
    }
    else {
        
        // Fill remaining space
        DBGU_PutCharN(filler, width - 1);
    }
    
    // Output lower digit
    DBGU_PutChar((char) ((value % 8) + '0'));
}

#if defined(USE_FLOATING_POINT)
//------------------------------------------------------------------------------
/// Outputs a floating-point value on the DBGU, using the given precision.
/// The precision is the maximum number of decimal digits that shall be
/// output; if null, all of them are displayed.
/// \param value  Floating-point value to output.
/// \param precision  Number of decimal digits to output (0 = all digits).
//------------------------------------------------------------------------------
static void DBGU_PutDouble(double value, int precision)
{
    // Output the integer part
    int integer = (int) value;
    if ((value < 0) && (integer == 0)) {

        // The minus sign won't be output by DBGU_PutInteger if the integer part
        // if the value is null, so this is done here.
        DBGU_PutChar('-');
    }
    DBGU_PutInteger(integer, 0, 0);

    DBGU_PutChar('.');

    // Output the decimal part
    double decimal = value - integer;
    if (decimal < 0) {

        decimal = -decimal;
    }

    if (precision >= 0) {
    
        // Output a fixed number of decimal digits
        while (precision > 0) {
    
            decimal *= 10;
            integer = (unsigned int) decimal;
            DBGU_PutUnsignedInteger(decimal, 0, 0);
            decimal -= integer;
    
            precision--;
        }
    }
    else {

        // Output all decimal digits
        do {

            decimal *= 10;
            integer = (unsigned int) decimal;
            DBGU_PutUnsignedInteger(decimal, 0, 0);
            decimal -= integer;

        } while (decimal > 0);
    }
}
#endif //#if defined(USE_FLOATING_POINT)

//------------------------------------------------------------------------------
/// Returns 1 if the given character is the representation of a digit; otherwise
/// 0.
/// \param character  Character to examinate.
//------------------------------------------------------------------------------
static unsigned char DBGU_IsDigit(char character)
{
    if ((character >= '0') && (character <= '9')) {

        return 1;
    }
    else {

        return 0;
    }
}

//------------------------------------------------------------------------------
/// Parses a numerical value at the start of string and returns it. The
/// string pointer is updated to point right after the end of the value.
/// \param string  String to parse for a numerical value.
//------------------------------------------------------------------------------
static signed int DBGU_ParseDigits(const char **string)
{
    signed int value = 0;

    while (DBGU_IsDigit(**string)) {

        value = (10 * value) + (**string - '0');
        *string += 1;
    }

    return value;
}
            
//------------------------------------------------------------------------------
//      Exported functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Initializes the DBGU with the given parameters, and enables both the
/// transmitter and the receiver.
/// \param mode  Operating mode to configure (see <Modes>).
/// \param baudrate  Desired baudrate.
/// \param mck  Frequency of the system master clock.
//------------------------------------------------------------------------------
void DBGU_Configure(unsigned int mode,
                           unsigned int baudrate,
                           unsigned int mck)
{   
    // Reset & disable receiver and transmitter, disable interrupts
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX;
    AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF;
    
    // Configure baud rate
    AT91C_BASE_DBGU->DBGU_BRGR = mck / (baudrate * 16);
    
    // Configure mode register
    AT91C_BASE_DBGU->DBGU_MR = mode;
    
    // Disable DMA channel
    AT91C_BASE_DBGU->DBGU_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;

    // Enable receiver and transmitter
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
}

//------------------------------------------------------------------------------
/// Reads and returns a character from the DBGU.
//------------------------------------------------------------------------------
unsigned char DBGU_GetChar()
{
    while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) == 0);
    return AT91C_BASE_DBGU->DBGU_RHR;
}

//------------------------------------------------------------------------------
/// Outputs a formatted string on the DBGU. The format is similar to the
/// printf() method definition in stdio.h.
/// \param string  Formatted string to output.
/// \param ...  Additional parameters, depending on the formatted string.
//------------------------------------------------------------------------------
void DBGU_Printf(const char *string, ...)
{
    va_list       list;
    unsigned char isTokenBeingParsed = 0;
    signed int    width = 0;
    char          filler = ' ';
    signed int    precision = -1;
    
    // Start processing additional arguments
    va_start(list, string);
    
    // Loop through string
    while (*string != 0) {
   
        // Check if a token is being parsed
        if (isTokenBeingParsed) {
        
            switch (*string) {
                
                // Flags
                //------
                // '0'
                case '0':
                    filler = '0';
                    break;
                
                // TODO +
                // TODO -
                // TODO #
                 
                // '.': precision
                case '.':
                    string++;
                    precision = (signed int) DBGU_ParseDigits(&string);
                    string--;
                    break;
                
                // Width
                //------
                // Number
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    width = DBGU_ParseDigits(&string);
                    string--;
                    break;
                    
                // TODO *
                
                // Types
                //------
                // Signed integer
                case 'd':
                case 'i':
                    DBGU_PutInteger(va_arg(list, int), width, filler);
                    isTokenBeingParsed = 0;
                    break;
                
                // Unsigned integer
                case 'u':
                    DBGU_PutUnsignedInteger(va_arg(list, unsigned int),
                                            width, filler);
                    isTokenBeingParsed = 0;
                    break;
                    
                // Double in fixed-point notation
                // TODO

#if defined(USE_FLOATING_POINT)
                // Real value in standard form
                // Double in standard or exponentional notation
                case 'f':
                    // Float is promoted to double when passing through va_args
                    DBGU_PutDouble(va_arg(list, double), precision);
                    isTokenBeingParsed = 0;
                    break;
#endif
                
                // Hexadecimal value
                case 'x':
                    DBGU_PutHexadecimal(va_arg(list, unsigned int),
                                        width, filler, 0);
                    isTokenBeingParsed = 0;
                    break; 
                
                case 'X':
                    DBGU_PutHexadecimal(va_arg(list, unsigned int),
                                        width, filler, 1);
                    isTokenBeingParsed = 0;
                    break;
                    
                // Octal value
                case 'o':
                    DBGU_PutOctal(va_arg(list, unsigned int),
                                  width, filler);
                    isTokenBeingParsed = 0;
                    break;
                   
                // String
                case 's':
                    DBGU_PutString(va_arg(list, char *));
                    isTokenBeingParsed = 0;
                    break;
                    
                // Character
                case 'c':
                    // 'char' is promoted to 'int' when passing through va_arg
                    DBGU_PutChar(va_arg(list, unsigned int));
                    isTokenBeingParsed = 0;
                    break;
                    
                // %
                case '%':
                    DBGU_PutChar('%');
                    isTokenBeingParsed = 0;
                    break;
            }
            
            // Reset optional values if token has been consumed
            if (!isTokenBeingParsed) {
                
                width = 0;
                filler = ' ';
                precision = -1;
            }
        }
        // Check if this is a formatted parameter
        else if (*string == '%') {
            
            // Start token parsing
            isTokenBeingParsed = 1;
        }
        else {
            // Display character
            DBGU_PutChar(*string);
        }
        
        string++;
    }
}

