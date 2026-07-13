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

#include "PosixCompat.h"
#include <DebugServices.h>


typedef PCHAR (WINAPI * if_indextoname_funcptr_t)(ULONG index, PCHAR name);
typedef ULONG (WINAPI * if_nametoindex_funcptr_t)(PCSTR name);


unsigned
if_nametoindex( const char * ifname )
{
	HMODULE library;
	unsigned index = 0;

	check( ifname );

	// Try and load the IP helper library dll
	if ((library = LoadLibrary(TEXT("Iphlpapi")) ) != NULL )
	{
		if_nametoindex_funcptr_t if_nametoindex_funcptr;

		// On Vista and above there is a Posix like implementation of if_nametoindex
		if ((if_nametoindex_funcptr = (if_nametoindex_funcptr_t) GetProcAddress(library, "if_nametoindex")) != NULL )
		{
			index = if_nametoindex_funcptr(ifname);
		}

		FreeLibrary(library);
	}

	return index;
}


char*
if_indextoname( unsigned ifindex, char * ifname )
{
	HMODULE library;
	char * name = NULL;

	check( ifname );
	*ifname = '\0';

	// Try and load the IP helper library dll
	if ((library = LoadLibrary(TEXT("Iphlpapi")) ) != NULL )
	{
		if_indextoname_funcptr_t if_indextoname_funcptr;

		// On Vista and above there is a Posix like implementation of if_indextoname
		if ((if_indextoname_funcptr = (if_indextoname_funcptr_t) GetProcAddress(library, "if_indextoname")) != NULL )
		{
			name = if_indextoname_funcptr(ifindex, ifname);
		}

		FreeLibrary(library);
	}

	return name;
}


int
inet_pton( int family, const char * addr, void * dst )
{
	struct sockaddr_storage ss;
	int sslen = sizeof( ss );

	ZeroMemory( &ss, sizeof( ss ) );
	ss.ss_family = family;

	if ( WSAStringToAddressA( ( LPSTR ) addr, family, NULL, ( struct sockaddr* ) &ss, &sslen ) == 0 )
	{
		if ( family == AF_INET ) { memcpy( dst, &( ( struct sockaddr_in* ) &ss)->sin_addr, sizeof( IN_ADDR ) ); return 1; }
		else if ( family == AF_INET6 ) { memcpy( dst, &( ( struct sockaddr_in6* ) &ss)->sin6_addr, sizeof( IN6_ADDR ) ); return 1; }
		else return 0;
	}
    else return 0;
}


int
gettimeofday( struct timeval * tv, struct timezone * tz )
{
#define EPOCHFILETIME (116444736000000000i64)

	if ( tv != NULL )
	{
		FILETIME        ft;
		LARGE_INTEGER   li;
		__int64         t;

		GetSystemTimeAsFileTime(&ft);
		li.LowPart  = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		t  = li.QuadPart;	/* In 100-nanosecond intervals */
		t -= EPOCHFILETIME;	/* Offset to the Epoch time */
		t /= 10;			/* In microseconds */
		tv->tv_sec  = ( long )( t / 1000000 );
		tv->tv_usec = ( long )( t % 1000000 );
	}

	return 0;
}


extern struct tm*
localtime_r( const time_t * clock, struct tm * result )
{
	localtime_s( result, clock );
	return result;
}
