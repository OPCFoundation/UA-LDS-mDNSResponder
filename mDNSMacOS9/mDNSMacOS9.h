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

// ***************************************************************************
// Classic Mac (Open Transport) structures

//#include <Files.h>	// OpenTransport.h requires this
#include <OpenTransport.h>
#include <OpenTptInternet.h>
#include <OpenTptClient.h>

typedef enum
{
    mOT_Closed = 0,     // We got kOTProviderIsClosed message
    mOT_Reset,          // We got xOTStackWasLoaded message
    mOT_Start,          // We've called OTAsyncOpenEndpoint
    mOT_ReusePort,      // Have just done kReusePortOption
    mOT_RcvDestAddr,    // Have just done kRcvDestAddrOption
    mOT_SetUTTL,        // Have just done kSetUnicastTTLOption
    mOT_SetMTTL,        // Have just done kSetMulticastTTLOption
    mOT_LLScope,        // Have just done kAddLinkMulticastOption
//	mOT_AdminScope,		// Have just done kAddAdminMulticastOption
    mOT_Bind,           // We've just called OTBind
    mOT_Ready           // Got T_BINDCOMPLETE; Interface is registered and active
} mOT_State;

typedef struct { TOptionHeader h; mDNSv4Addr multicastGroupAddress; mDNSv4Addr InterfaceAddress; } TIPAddMulticastOption;
typedef struct { TOptionHeader h; UInt8 val; } TSetByteOption;
typedef struct { TOptionHeader h; UInt32 flag; } TSetBooleanOption;

// TOptionBlock is a union of various types.
// What they all have in common is that they all start with a TOptionHeader.
typedef union  { TOptionHeader h; TIPAddMulticastOption m; TSetByteOption i; TSetBooleanOption b; } TOptionBlock;

struct mDNS_PlatformSupport_struct
{
    EndpointRef ep;
    UInt32 mOTstate;                // mOT_State enum
    TOptionBlock optBlock;
    TOptMgmt optReq;
    long OTTimerTask;
    UInt32 nesting;
    NetworkInterfaceInfo interface;
};
