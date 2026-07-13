/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
 *
 * OPC Foundation MIT License 1.00
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The complete license agreement can be found here:
 * http://opcfoundation.org/License/MIT/1.00/
 * ======================================================================*/

#include <ctype.h>
#include <stdio.h>          // For stdout, stderr

#include "ClientCommon.h"

const char *GetNextLabel(const char *cstr, char label[64])
{
    char *ptr = label;
    while (*cstr && *cstr != '.')               // While we have characters in the label...
    {
        char c = *cstr++;
        if (c == '\\' && *cstr)                 // If we have a backslash, and it's not the last character of the string
        {
            c = *cstr++;
            if (isdigit(cstr[-1]) && isdigit(cstr[0]) && isdigit(cstr[1]))
            {
                int v0 = cstr[-1] - '0';                        // then interpret as three-digit decimal
                int v1 = cstr[ 0] - '0';
                int v2 = cstr[ 1] - '0';
                int val = v0 * 100 + v1 * 10 + v2;
                // If valid three-digit decimal value, use it
                // Note that although ascii nuls are possible in DNS labels
                // we're building a C string here so we have no way to represent that
                if (val == 0) val = '-';
                if (val <= 255) { c = (char)val; cstr += 2; }
            }
        }
        *ptr++ = c;
        if (ptr >= label+64) { label[63] = 0; return(NULL); }   // Illegal label more than 63 bytes
    }
    *ptr = 0;                                                   // Null-terminate label text
    if (ptr == label) return(NULL);                             // Illegal empty label
    if (*cstr) cstr++;                                          // Skip over the trailing dot (if present)
    return(cstr);
}
