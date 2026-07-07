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

#include "ConfigPropertySheet.h"
#include <WinServices.h>
extern "C"
{
#include <ClientCommon.h>
}
#include <process.h>

// Custom events

#define WM_DATAREADY		( WM_USER + 0x100 )


IMPLEMENT_DYNCREATE(CConfigPropertySheet, CPropertySheet)


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::CConfigPropertySheet
//---------------------------------------------------------------------------------------------------------------------------

CConfigPropertySheet::CConfigPropertySheet()
:
	CPropertySheet(),
	m_browseDomainsRef( NULL ),
	m_thread( NULL ),
	m_threadExited( NULL )
{
	AddPage(&m_firstPage );
	AddPage(&m_secondPage);
	AddPage(&m_thirdPage);

	InitializeCriticalSection( &m_lock );
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::~CConfigPropertySheet
//---------------------------------------------------------------------------------------------------------------------------

CConfigPropertySheet::~CConfigPropertySheet()
{
	DeleteCriticalSection( &m_lock );
}


BEGIN_MESSAGE_MAP(CConfigPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CConfigPropertySheet)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_DATAREADY, OnDataReady )
END_MESSAGE_MAP()


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::OnInitDialog
//---------------------------------------------------------------------------------------------------------------------------

BOOL
CConfigPropertySheet::OnInitDialog()
{
	OSStatus err;

	BOOL b = CPropertySheet::OnInitDialog();

	err = SetupBrowsing();
	require_noerr( err, exit );

exit:

	return b;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::OnCommand
//---------------------------------------------------------------------------------------------------------------------------

BOOL
CConfigPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
   // Check if OK or Cancel was hit

   if ( ( wParam == ID_WIZFINISH ) || ( wParam == IDOK ) || ( wParam == IDCANCEL ) ) 
   {
      OnEndDialog();
   }

   return CPropertySheet::OnCommand(wParam, lParam);
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::OnDataReady
//---------------------------------------------------------------------------------------------------------------------------

LRESULT
CConfigPropertySheet::OnDataReady(WPARAM inWParam, LPARAM inLParam)
{
	if (WSAGETSELECTERROR(inLParam) && !(HIWORD(inLParam)))
	{
		dlog( kDebugLevelError, "OnSocket: window error\n" );
	}
	else
	{
		SOCKET sock = (SOCKET) inWParam;

		if ( m_browseDomainsRef && DNSServiceRefSockFD( m_browseDomainsRef ) == (int) sock )
		{
			DNSServiceProcessResult( m_browseDomainsRef );
		}
	}

	return 0;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::OnEndDialog
//---------------------------------------------------------------------------------------------------------------------------

void
CConfigPropertySheet::OnEndDialog()
{
	OSStatus err;

	err = TearDownBrowsing();
	check_noerr( err );
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::SetupBrowsing
//---------------------------------------------------------------------------------------------------------------------------

OSStatus
CConfigPropertySheet::SetupBrowsing()
{
	OSStatus err;

	// Start browsing for browse domains

	err = DNSServiceEnumerateDomains( &m_browseDomainsRef, kDNSServiceFlagsBrowseDomains, 0, BrowseDomainsReply, this );
	require_noerr( err, exit );

	err = WSAAsyncSelect( DNSServiceRefSockFD( m_browseDomainsRef ), m_hWnd, WM_DATAREADY, FD_READ|FD_CLOSE );
	require_noerr( err, exit );

exit:

	if ( err )
	{
		TearDownBrowsing();
	}

	return err;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::TearDownBrowsing
//---------------------------------------------------------------------------------------------------------------------------

OSStatus
CConfigPropertySheet::TearDownBrowsing()
{
	OSStatus err = kNoErr;

	if ( m_browseDomainsRef )
	{
		err = WSAAsyncSelect( DNSServiceRefSockFD( m_browseDomainsRef ), m_hWnd, 0, 0 );
		check_noerr( err );

		DNSServiceRefDeallocate( m_browseDomainsRef );
	
		m_browseDomainsRef = NULL;
	}

	return err;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::DecodeDomainName
//---------------------------------------------------------------------------------------------------------------------------

OSStatus
CConfigPropertySheet::DecodeDomainName( const char * raw, CString & decoded )
{
	char nextLabel[128] = "\0";
	char decodedDomainString[kDNSServiceMaxDomainName];
    char * buffer = (char *) raw;
    int labels = 0, i;
    char text[64];
	const char *label[128];
	OSStatus	err;
    
	// Initialize

	decodedDomainString[0] = '\0';

    // Count the labels

	while ( *buffer )
	{
		label[labels++] = buffer;
		buffer = (char *) GetNextLabel(buffer, text);
    }
        
    buffer = (char*) raw;

    for (i = 0; i < labels; i++)
	{
		buffer = (char *)GetNextLabel(buffer, nextLabel);
        strcat(decodedDomainString, nextLabel);
        strcat(decodedDomainString, ".");
    }
    
    // Remove trailing dot from domain name.
    
	decodedDomainString[ strlen( decodedDomainString ) - 1 ] = '\0';

	// Convert to Unicode

	err = UTF8StringToStringObject( decodedDomainString, decoded );

	return err;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CConfigPropertySheet::BrowseDomainsReply
//---------------------------------------------------------------------------------------------------------------------------

void DNSSD_API
CConfigPropertySheet::BrowseDomainsReply
							(
							DNSServiceRef			sdRef,
							DNSServiceFlags			flags,
							uint32_t				interfaceIndex,
							DNSServiceErrorType		errorCode,
							const char			*	replyDomain,
							void				*	context
							)
{
	CConfigPropertySheet	*	self = reinterpret_cast<CConfigPropertySheet*>(context);
	CString						decoded;
	OSStatus					err;

	DEBUG_UNUSED( sdRef );
	DEBUG_UNUSED( interfaceIndex );

	if ( errorCode )
	{
		goto exit;
	}

	check( replyDomain );
	
	// Ignore local domains

	if ( strcmp( replyDomain, "local." ) == 0 )
	{
		goto exit;
	}

	err = self->DecodeDomainName( replyDomain, decoded );
	require_noerr( err, exit );

	// Remove trailing '.'

	decoded.TrimRight( '.' );

	if ( flags & kDNSServiceFlagsAdd )
	{
		self->m_browseDomains.push_back( decoded );
	}
	else
	{
		self->m_browseDomains.remove( decoded );
	}

exit:

	return;
}
