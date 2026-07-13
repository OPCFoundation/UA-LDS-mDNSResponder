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

#ifndef _Poll_h
#define _Poll_h

#include	"CommonServices.h"
#include	<mswsock.h>
#include	"mDNSEmbeddedAPI.h"
#include	"uDNS.h"


#if defined(__cplusplus )
extern "C" {
#endif


typedef void ( CALLBACK *mDNSPollSocketCallback )( SOCKET socket, LPWSANETWORKEVENTS event, void *context );
typedef void ( CALLBACK *mDNSPollEventCallback )( HANDLE event, void *context );


extern mStatus
mDNSPollRegisterSocket( SOCKET socket, int networkEvents, mDNSPollSocketCallback callback, void *context );


extern void
mDNSPollUnregisterSocket( SOCKET socket );


extern mStatus
mDNSPollRegisterEvent( HANDLE event, mDNSPollEventCallback callback, void *context );


extern void
mDNSPollUnregisterEvent( HANDLE event );


extern mStatus
mDNSPoll( DWORD msec );


#if defined(__cplusplus)
}
#endif


#endif
