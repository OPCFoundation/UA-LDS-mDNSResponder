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

#include "mDNSDebug.h"

#include <stdio.h>

#if defined(WIN32) || defined(EFI32) || defined(EFI64) || defined(EFIX64)
// Need to add Windows/EFI syslog support here
#define LOG_PID 0x01
#define LOG_CONS 0x02
#define LOG_PERROR 0x20
#else
#include <syslog.h>
#endif

#include "mDNSEmbeddedAPI.h"

mDNSexport int mDNS_LoggingEnabled       = 0;
mDNSexport int mDNS_PacketLoggingEnabled = 0;
mDNSexport int mDNS_McastLoggingEnabled  = 0;
mDNSexport int mDNS_McastTracingEnabled  = 0; 

#if MDNS_DEBUGMSGS
mDNSexport int mDNS_DebugMode = mDNStrue;
#else
mDNSexport int mDNS_DebugMode = mDNSfalse;
#endif

// Note, this uses mDNS_vsnprintf instead of standard "vsnprintf", because mDNS_vsnprintf knows
// how to print special data types like IP addresses and length-prefixed domain names
#if MDNS_DEBUGMSGS > 1
mDNSexport void verbosedebugf_(const char *format, ...)
{
    char buffer[512];
    va_list ptr;
    va_start(ptr,format);
    buffer[mDNS_vsnprintf(buffer, sizeof(buffer), format, ptr)] = 0;
    va_end(ptr);
    mDNSPlatformWriteDebugMsg(buffer);
}
#endif

// Log message with default "mDNSResponder" ident string at the start
mDNSlocal void LogMsgWithLevelv(mDNSLogLevel_t logLevel, const char *format, va_list ptr)
{
    char buffer[512];
    buffer[mDNS_vsnprintf((char *)buffer, sizeof(buffer), format, ptr)] = 0;
    mDNSPlatformWriteLogMsg(ProgramName, buffer, logLevel);
}

#define LOG_HELPER_BODY(L) \
    { \
        va_list ptr; \
        va_start(ptr,format); \
        LogMsgWithLevelv(L, format, ptr); \
        va_end(ptr); \
    }

// see mDNSDebug.h
#if !MDNS_HAS_VA_ARG_MACROS
void LogMsg_(const char *format, ...)       LOG_HELPER_BODY(MDNS_LOG_MSG)
void LogOperation_(const char *format, ...) LOG_HELPER_BODY(MDNS_LOG_OPERATION)
void LogSPS_(const char *format, ...)       LOG_HELPER_BODY(MDNS_LOG_SPS)
void LogInfo_(const char *format, ...)      LOG_HELPER_BODY(MDNS_LOG_INFO)
#endif

#if MDNS_DEBUGMSGS
void debugf_(const char *format, ...)       LOG_HELPER_BODY(MDNS_LOG_DEBUG)
#endif

// Log message with default "mDNSResponder" ident string at the start
mDNSexport void LogMsgWithLevel(mDNSLogLevel_t logLevel, const char *format, ...)
LOG_HELPER_BODY(logLevel)
