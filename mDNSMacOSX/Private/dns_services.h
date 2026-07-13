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

#ifndef _DNS_SERVICES_H
#define _DNS_SERVICES_H

#include <dispatch/dispatch.h>

// DNSXConnRef: Opaque internal data type
typedef struct _DNSXConnRef_t *DNSXConnRef;

typedef enum
{
    kDNSX_NoError                   =  0,
    kDNSX_UnknownErr                = -65537,   /* 0xFFFE FFFF */
    kDNSX_NoMem                     = -65539,
    kDNSX_BadParam                  = -65540,
    kDNSX_DaemonNotRunning          = -65563,   /* Background daemon not running */
    kDNSX_DictError                 = -65565,   /* Dictionary Error */
    kDNSX_Engaged                   = -65566,   /* DNS Proxy is in use by another client */
    kDNSX_Timeout                   = -65568    
} DNSXErrorType;

// A max of 5 input interfaces can be processed at one time
#define MaxInputIf 5
#define IfIndex uint64_t
#define kDNSIfindexAny 0

// Enable DNS Proxy with an appropriate parameter defined below
typedef enum
{
    kDNSProxyEnable = 1
    // Other values reserved for future use
} DNSProxyParameters;

/*********************************************************************************************
*
*  Enable DNS Proxy Functionality
*
*********************************************************************************************/

/* DNSXEnableProxy : Turns ON the DNS Proxy (Details below)
 *
 * DNSXEnableProxyReply() parameters:
 *
 * connRef:                  The DNSXConnRef initialized by DNSXEnableProxy().
 *
 * errCode:                  Will be kDNSX_NoError on success, otherwise will indicate the 
 *                           failure that occurred.  Other parameters are undefined if 
 *                           errCode is nonzero.
 *
 */

typedef void (*DNSXEnableProxyReply)
(
    DNSXConnRef           connRef,
    DNSXErrorType         errCode 
);

/* DNSXEnableProxy
 * 
 * Enables the DNS Proxy functionality which will remain ON until the client terminates explictly (or exits/crashes).
 * Client can turn it OFF by passing the returned DNSXConnRef to DNSXRefDeAlloc()
 * 
 * DNSXEnableProxy() Parameters:
 *
 * connRef:                   A pointer to DNSXConnRef that is initialized to NULL when called for the first  
 *                            time. If the call succeeds it will be initialized to a non-NULL value.
 *                            Client terminates the DNS Proxy by passing this DNSXConnRef to DNSXRefDeAlloc().
 *
 * proxyparam:                Enable DNS Proxy functionality with parameters that are described in
 *                            DNSProxyParameters above.
 *
 * inIfindexArr[MaxInputIf]:  List of input interfaces from which the DNS queries will be accepted and
 *                            forwarded to the output interface specified below. The daemon processes
 *                            MaxInputIf entries in the list. For eg. if one has less than MaxInputIfs
 *                            values, just initialize the other values to be 0. Note: This field needs to
 *                            be initialized by the client.
 *
 * outIfindex:                Output interface on which the query will be forwarded.
 *                            Passing kDNSIfindexAny causes DNS Queries to be sent on the primary interface.
 *
 * clientq:                   Queue the client wants to schedule the callBack on (Note: Must not be NULL)
 *
 * callBack:                  CallBack function for the client that indicates success or failure.
 *                            Note: callback may be invoked more than once, For eg. if enabling DNS Proxy
 *                            first succeeds and the daemon possibly crashes sometime later. 
 *
 * return value:              Returns kDNSX_NoError when no error otherwise returns an error code indicating
 *                            the error that occurred. Note: A return value of kDNSX_NoError does not mean 
 *                            that DNS Proxy was successfully enabled. The callBack may asynchronously
 *                            return an error (such as kDNSX_DaemonNotRunning/ kDNSX_Engaged)
 *
 */

DNSXErrorType DNSXEnableProxy
(
    DNSXConnRef              *connRef,
    DNSProxyParameters       proxyparam,
    IfIndex                  inIfindexArr[MaxInputIf],
    IfIndex                  outIfindex,
    dispatch_queue_t         clientq,
    DNSXEnableProxyReply     callBack
);

/* DNSXRefDeAlloc()
 *
 * Terminate a connection with the daemon and free memory associated with the DNSXConnRef.
 * Used to Disable DNS Proxy on that connection.
 *
 * connRef:        A DNSXConnRef initialized by any of the DNSX*() calls.
 *
 */
void DNSXRefDeAlloc(DNSXConnRef connRef);

#endif  /* _DNS_SERVICES_H */
