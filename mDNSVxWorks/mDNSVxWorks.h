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

#ifndef __MDNS_VXWORKS_H__
#define __MDNS_VXWORKS_H__

#include    "vxWorks.h"
#include    "config.h"

#include    "semLib.h"

#include    "CommonServices.h"
#include    "DebugServices.h"

#ifdef  __cplusplus
extern "C" {
#endif

// Forward Declarations

typedef struct  NetworkInterfaceInfoVxWorks NetworkInterfaceInfoVxWorks;

//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		SocketSet

    @abstract	Data for IPv4 and IPv6 sockets.
 */

typedef struct  SocketSet SocketSet;
struct  SocketSet
{
    NetworkInterfaceInfoVxWorks *       info;
    SocketRef sockV4;
    SocketRef sockV6;
};

//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		NetworkInterfaceInfoVxWorks

    @abstract	Interface info for VxWorks.
 */

struct  NetworkInterfaceInfoVxWorks
{
    NetworkInterfaceInfo ifinfo;                    // MUST be the first element in this structure.
    NetworkInterfaceInfoVxWorks *       next;
    mDNSu32 exists;                                 // 1 = currently exists in getifaddrs list; 0 = doesn't.
                                                    // 2 = exists, but McastTxRx state changed.
    mDNSs32 lastSeen;                               // If exists == 0, last time this interface appeared in getifaddrs list.
    mDNSu32 scopeID;                                // Interface index / IPv6 scope ID.
    int family;                                     // Socket address family of the primary socket.
    mDNSBool multicast;
    SocketSet ss;
};

//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		mDNS_PlatformSupport_struct

    @abstract	Data for mDNS platform plugin.
 */

struct  mDNS_PlatformSupport_struct
{
    NetworkInterfaceInfoVxWorks *       interfaceList;
    SocketSet unicastSS;
    domainlabel userNiceLabel;
    domainlabel userHostLabel;

    SEM_ID lock;
    SEM_ID initEvent;
    mStatus initErr;
    SEM_ID quitEvent;
    int commandPipe;
    int taskID;
    mDNSBool quit;
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
/*!	@function	mDNSDeferIPv4

    @abstract	Tells mDNS whether to defer advertising of IPv4 interfaces.

    @discussion

    To workaround problems with clients getting a link-local IPv4 address before a DHCP address is acquired, this allows
    external code to defer advertising of IPv4 addresses until a DHCP lease has been acquired (or it times out).
 */

void    mDNSDeferIPv4( mDNSBool inDefer );

#ifdef  __cplusplus
}
#endif

#endif  // __MDNS_VXWORKS_H__
