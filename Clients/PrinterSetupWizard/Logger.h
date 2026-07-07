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

#ifndef _Logger_h
#define _Logger_h

#include <fstream>
#include <string>


class Logger : public std::ofstream
{
public:

Logger();
~Logger();

std::string
currentTime();
};


#define require_noerr_with_log( LOG, MESSAGE, ERR, LABEL )  \
    do                      \
    {                       \
        int_least32_t localErr;         \
        localErr = (int_least32_t)( ERR );  \
        if( localErr != 0 )             \
        {                   \
            log << log.currentTime() << " [ERROR] " << MESSAGE << " returned " << ERR << std::endl;                     \
            log << log.currentTime() << " [WHERE] " << "\"" << __FILE__ << "\", \"" << __FUNCTION__ << "\", line " << __LINE__ << std::endl << std::endl;   \
            goto LABEL;         \
        }                   \
    } while( 0 )


#define require_action_with_log( LOG, X, LABEL, ACTION )    \
    do                      \
    {                       \
        if( !( X ) )                \
        {                   \
            log << log.currentTime() << " [ERROR] " << # X << std::endl; \
            log << log.currentTime() << " [WHERE] " << "\"" << __FILE__ << "\", \"" << __FUNCTION__ << "\", line " << __LINE__ << std::endl << std::endl;   \
            { ACTION; }         \
            goto LABEL;         \
        }                   \
    } while( 0 )

#endif
