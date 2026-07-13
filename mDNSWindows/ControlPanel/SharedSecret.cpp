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

// SharedSecret.cpp : implementation file
//


#include <Secret.h>
#include "stdafx.h"
#include "SharedSecret.h"
#include <WinServices.h>

#include <DebugServices.h>


// SharedSecret dialog

IMPLEMENT_DYNAMIC(CSharedSecret, CDialog)


//---------------------------------------------------------------------------------------------------------------------------
//	CSharedSecret::CSharedSecret
//---------------------------------------------------------------------------------------------------------------------------

CSharedSecret::CSharedSecret(CWnd* pParent /*=NULL*/)
	: CDialog(CSharedSecret::IDD, pParent)
	, m_key(_T(""))
	, m_secret(_T(""))
{
}


//---------------------------------------------------------------------------------------------------------------------------
//	CSharedSecret::~CSharedSecret
//---------------------------------------------------------------------------------------------------------------------------

CSharedSecret::~CSharedSecret()
{
}


//---------------------------------------------------------------------------------------------------------------------------
//	CSharedSecret::DoDataExchange
//---------------------------------------------------------------------------------------------------------------------------

void CSharedSecret::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEY, m_key );
	DDX_Text(pDX, IDC_SECRET, m_secret );
}


BEGIN_MESSAGE_MAP(CSharedSecret, CDialog)
END_MESSAGE_MAP()


//---------------------------------------------------------------------------------------------------------------------------
//	CSharedSecret::Load
//---------------------------------------------------------------------------------------------------------------------------

void
CSharedSecret::Load( CString zone )
{
	char	zoneUTF8[ 256 ];
	char	outDomain[ 256 ];
	char	outKey[ 256 ];
	char	outSecret[ 256 ];

	StringObjectToUTF8String( zone, zoneUTF8, sizeof( zoneUTF8 ) );

	if ( LsaGetSecret( zoneUTF8, outDomain, sizeof( outDomain ) / sizeof( TCHAR ), outKey, sizeof( outKey ) / sizeof( TCHAR ), outSecret, sizeof( outSecret ) / sizeof( TCHAR ) ) )
	{
		m_key		= outKey;
		m_secret	= outSecret;
	}
	else
	{
		m_key = zone;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
//	CSharedSecret::Commit
//---------------------------------------------------------------------------------------------------------------------------

void
CSharedSecret::Commit( CString zone )
{
	char	zoneUTF8[ 256 ];
	char	keyUTF8[ 256 ];
	char	secretUTF8[ 256 ];

	StringObjectToUTF8String( zone, zoneUTF8, sizeof( zoneUTF8 ) );
	StringObjectToUTF8String( m_key, keyUTF8, sizeof( keyUTF8 ) );
	StringObjectToUTF8String( m_secret, secretUTF8, sizeof( secretUTF8 ) );

	LsaSetSecret( zoneUTF8, keyUTF8, secretUTF8 );
}
