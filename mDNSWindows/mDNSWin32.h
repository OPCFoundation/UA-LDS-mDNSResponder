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

#ifndef	__MDNS_WIN32__
#define	__MDNS_WIN32__

#include	"CommonServices.h"

#if( !defined( _WIN32_WCE ) )
	#include	<mswsock.h>
#endif

#include	"mDNSEmbeddedAPI.h"
#include	"uDNS.h"

#ifdef	__cplusplus
	extern "C" {
#endif


typedef void ( *TCPUserCallback )();

struct TCPSocket_struct
{
	TCPSocketFlags				flags;		// MUST BE FIRST FIELD -- mDNSCore expects every TCPSocket_struct to begin with TCPSocketFlags flags
	SOCKET						fd;
	BOOL						connected;
	TCPUserCallback				userCallback;
	void					*	userContext;
	BOOL						closed;
	mDNS					*	m;
};


struct UDPSocket_struct
{
	mDNSIPPort					port; 		// MUST BE FIRST FIELD -- mDNSCoreReceive expects every UDPSocket_struct to begin with mDNSIPPort port
	mDNSAddr					addr;		// This is initialized by our code. If we don't get the 
											// dstAddr from WSARecvMsg we use this value instead.
	SOCKET						fd;
	LPFN_WSARECVMSG				recvMsgPtr;
	DNSMessage					packet;
	struct mDNSInterfaceData	*ifd;
	UDPSocket					*next;
	mDNS						*m;
};


//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		mDNSInterfaceData

	@abstract	Structure containing interface-specific data.
*/

typedef struct	mDNSInterfaceData	mDNSInterfaceData;
struct	mDNSInterfaceData
{
	char						name[ 128 ];
	uint32_t					index;
	uint32_t					scopeID;
	struct UDPSocket_struct		sock;
	NetworkInterfaceInfo		interfaceInfo;
	mDNSBool					hostRegistered;
	mDNSInterfaceData		*	next;
};


//---------------------------------------------------------------------------------------------------------------------------
/*!	@typedef	ReportStatusFunc
*/
typedef void		(*ReportStatusFunc)(int inType, const char *inFormat, ...);


//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		mDNS_PlatformSupport_struct

	@abstract	Structure containing platform-specific data.
*/

struct	mDNS_PlatformSupport_struct
{
	HANDLE						mainThread;
	HANDLE						checkFileSharesTimer;
	mDNSs32						checkFileSharesTimeout;
	ReportStatusFunc			reportStatusFunc;
	time_t						nextDHCPLeaseExpires;
	char						nbname[ 32 ];
	char						nbdomain[ 32 ];
	mDNSBool					smbFileSharing;
	mDNSBool					smbPrintSharing;
	ServiceRecordSet			smbSRS;
	AuthRecord					smbSubTypes[ 2 ];
	mDNSBool					registeredLoopback4;
	int							interfaceCount;
	mDNSInterfaceData *			interfaceList;
	mDNSInterfaceData *			inactiveInterfaceList;
	struct UDPSocket_struct		unicastSock4;
	struct UDPSocket_struct		unicastSock6;
	DWORD						osMajorVersion;
	DWORD						osMinorVersion;
};

//---------------------------------------------------------------------------------------------------------------------------
/*!	@struct		ifaddrs

	@abstract	Interface information
*/

struct ifaddrs
{
	struct ifaddrs *	ifa_next;
	char *				ifa_name;
	u_int				ifa_flags;
	struct sockaddr	*	ifa_addr;
	struct sockaddr	*	ifa_netmask;
	struct sockaddr	*	ifa_broadaddr;
	struct sockaddr	*	ifa_dstaddr;
	BYTE				ifa_physaddr[6];
	BOOL				ifa_dhcpEnabled;
	time_t				ifa_dhcpLeaseExpires;
	mDNSu8				ifa_womp;
	void *				ifa_data;
	
	struct
	{
		uint32_t		index;
	
	}	ifa_extra;
};


extern void		InterfaceListDidChange( mDNS * const inMDNS );
extern void		ComputerDescriptionDidChange( mDNS * const inMDNS );
extern void		TCPIPConfigDidChange( mDNS * const inMDNS );
extern void		DynDNSConfigDidChange( mDNS * const inMDNS );
extern void		FileSharingDidChange( mDNS * const inMDNS );
extern void		FirewallDidChange( mDNS * const inMDNS );
extern mStatus  TCPAddSocket( mDNS * const inMDNS, TCPSocket *sock );
extern mStatus	SetupInterfaceList( mDNS * const inMDNS );
extern mStatus	TearDownInterfaceList( mDNS * const inMDNS );
extern BOOL		IsWOMPEnabled();
extern void     DispatchSocketEvents( mDNS * const inMDNS );


#ifdef	__cplusplus
	}
#endif

#endif	// __MDNS_WIN32__
