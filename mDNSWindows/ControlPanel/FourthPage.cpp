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

#include "FourthPage.h"
#include "resource.h"

#include "ConfigPropertySheet.h"
#include "SharedSecret.h"

#include <WinServices.h>
    
#define MAX_KEY_LENGTH 255


IMPLEMENT_DYNCREATE(CFourthPage, CPropertyPage)


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::CFourthPage
//---------------------------------------------------------------------------------------------------------------------------

CFourthPage::CFourthPage()
:
	CPropertyPage(CFourthPage::IDD)
{
	//{{AFX_DATA_INIT(CFourthPage)
	//}}AFX_DATA_INIT
}


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::~CFourthPage
//---------------------------------------------------------------------------------------------------------------------------

CFourthPage::~CFourthPage()
{
}


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::DoDataExchange
//---------------------------------------------------------------------------------------------------------------------------

void CFourthPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFourthPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_POWER_MANAGEMENT, m_checkBox);
}

BEGIN_MESSAGE_MAP(CFourthPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFourthPage)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_POWER_MANAGEMENT, &CFourthPage::OnBnClickedPowerManagement)

END_MESSAGE_MAP()


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::SetModified
//---------------------------------------------------------------------------------------------------------------------------

void CFourthPage::SetModified( BOOL bChanged )
{
	m_modified = bChanged;

	CPropertyPage::SetModified( bChanged );
}


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::OnSetActive
//---------------------------------------------------------------------------------------------------------------------------

BOOL
CFourthPage::OnSetActive()
{
	CConfigPropertySheet	*	psheet;
	HKEY						key = NULL;
	DWORD						dwSize;
	DWORD						enabled;
	DWORD						err;
	BOOL						b = CPropertyPage::OnSetActive();

	psheet = reinterpret_cast<CConfigPropertySheet*>(GetParent());
	require_quiet( psheet, exit );

	m_checkBox.SetCheck( 0 );

	// Now populate the browse domain box

	err = RegCreateKeyEx( HKEY_LOCAL_MACHINE, kServiceParametersNode L"\\Power Management", 0,
		                  NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &key, NULL );
	require_noerr( err, exit );

	dwSize = sizeof( DWORD );
	err = RegQueryValueEx( key, L"Enabled", NULL, NULL, (LPBYTE) &enabled, &dwSize );
	require_noerr( err, exit );

	m_checkBox.SetCheck( enabled );
 
exit:

	if ( key )
	{
		RegCloseKey( key );
	}

	return b;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::OnOK
//---------------------------------------------------------------------------------------------------------------------------

void
CFourthPage::OnOK()
{
	if ( m_modified )
	{
		Commit();
	}
}



//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::Commit
//---------------------------------------------------------------------------------------------------------------------------

void
CFourthPage::Commit()
{
	HKEY		key		= NULL;
	DWORD		enabled;
	DWORD		err;

	err = RegCreateKeyEx( HKEY_LOCAL_MACHINE, kServiceParametersNode L"\\Power Management", 0,
		                  NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &key, NULL );
	require_noerr( err, exit );

	enabled = m_checkBox.GetCheck();
	err = RegSetValueEx( key, L"Enabled", NULL, REG_DWORD, (LPBYTE) &enabled, sizeof( enabled ) );
	require_noerr( err, exit );
	
exit:

	if ( key )
	{
		RegCloseKey( key );
	}
}


//---------------------------------------------------------------------------------------------------------------------------
//	CFourthPage::OnBnClickedRemoveBrowseDomain
//---------------------------------------------------------------------------------------------------------------------------



void CFourthPage::OnBnClickedPowerManagement()

{

	char buf[ 256 ];



	sprintf( buf, "check box: %d", m_checkBox.GetCheck() );

	OutputDebugStringA( buf );

	// TODO: Add your control notification handler code here



	SetModified( TRUE );

}
