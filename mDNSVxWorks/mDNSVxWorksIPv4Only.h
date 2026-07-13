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

#ifndef __MDNS_VXWORKS__
#define __MDNS_VXWORKS__

#include    "vxWorks.h"
#include    "semLib.h"

#include    "mDNSEmbeddedAPI.h"

#ifdef  __cplusplus
extern "C" {
#endif

// Forward Declarations

typedef struct  MDNSInterfaceItem MDNSInterfaceItem;

//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		mDNS_PlatformSupport_struct

    @abstract	Structure containing platform-specific data.
 */

struct  mDNS_PlatformSupport_struct
{
    SEM_ID lockID;
    SEM_ID readyEvent;
    mStatus taskInitErr;
    SEM_ID quitEvent;
    MDNSInterfaceItem *     interfaceList;
    int commandPipe;
    int task;
    mDNSBool quit;
    long configID;
    int rescheduled;
};

//---------------------------------------------------------------------------------------------------------------------------
/*!	@function	mDNSReconfigure

    @abstract	Tell mDNS that the configuration has changed. Call when IP address changes, link goes up after being down, etc.

    @discussion

    VxWorks does not provide a generic mechanism for getting notified when network interfaces change so this routines
    provides a way for BSP-specific code to signal mDNS that something has changed and it should re-build its interfaces.
 */

void    mDNSReconfigure( void );

//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		ifaddrs

    @abstract	Interface information
 */

struct ifaddrs
{
    struct ifaddrs *    ifa_next;
    char *              ifa_name;
    u_int ifa_flags;
    struct sockaddr *   ifa_addr;
    struct sockaddr *   ifa_netmask;
    struct sockaddr *   ifa_dstaddr;
    void *              ifa_data;
};

//---------------------------------------------------------------------------------------------------------------------------
/*!	@function	getifaddrs

    @abstract	Builds a linked list of interfaces. Caller must free using freeifaddrs if successful.
 */

int getifaddrs( struct ifaddrs **outAddrs );

//---------------------------------------------------------------------------------------------------------------------------
/*!	@function	freeifaddrs

    @abstract	Frees a linked list of interfaces built with getifaddrs.
 */

void    freeifaddrs( struct ifaddrs *inAddrs );

//---------------------------------------------------------------------------------------------------------------------------
/*!	@function	sock_pton

    @abstract	Converts a 'p'resentation address string into a 'n'umeric sockaddr structure.

    @result		0 if successful or an error code on failure.
 */

int sock_pton( const char *inString, int inFamily, void *outAddr, size_t inAddrSize, size_t *outAddrSize );

//---------------------------------------------------------------------------------------------------------------------------
/*!	@function	sock_ntop

    @abstract	Converts a 'n'umeric sockaddr structure into a 'p'resentation address string.

    @result		Ptr to 'p'resentation address string buffer if successful or NULL on failure.
 */

char *  sock_ntop( const void *inAddr, size_t inAddrSize, char *inBuffer, size_t inBufferSize );

#ifdef  __cplusplus
}
#endif

#endif  // __MDNS_VXWORKS__
