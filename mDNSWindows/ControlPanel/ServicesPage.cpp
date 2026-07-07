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

#include "ServicesPage.h"
#include "resource.h"

#include "ControlPanelExe.h"
#include "ConfigPropertySheet.h"

#include <WinServices.h>
    
#define MAX_KEY_LENGTH 255


IMPLEMENT_DYNCREATE(CServicesPage, CPropertyPage)


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::CServicesPage
//---------------------------------------------------------------------------------------------------------------------------

CServicesPage::CServicesPage()
:
	CPropertyPage(CServicesPage::IDD)
{
	//{{AFX_DATA_INIT(CServicesPage)
	//}}AFX_DATA_INIT
}


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::~CServicesPage
//---------------------------------------------------------------------------------------------------------------------------

CServicesPage::~CServicesPage()
{
}


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::DoDataExchange
//---------------------------------------------------------------------------------------------------------------------------

void CServicesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServicesPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ADVERTISE_SMB, m_SMBCheckBox);
	DDX_Control(pDX, IDC_POWER_MANAGEMENT, m_powerManagementCheckBox);
}

BEGIN_MESSAGE_MAP(CServicesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CServicesPage)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_ADVERTISE_SMB, &CServicesPage::OnBnClickedAdvertiseSMB)
	ON_BN_CLICKED(IDC_POWER_MANAGEMENT, &CServicesPage::OnBnClickedPowerManagement)

END_MESSAGE_MAP()


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::SetModified
//---------------------------------------------------------------------------------------------------------------------------

void CServicesPage::SetModified( BOOL bChanged )
{
	m_modified = bChanged;

	CPropertyPage::SetModified( bChanged );
}


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::OnSetActive
//---------------------------------------------------------------------------------------------------------------------------

BOOL
CServicesPage::OnSetActive()
{
	CConfigPropertySheet	*	psheet;
	HKEY						key = NULL;
	DWORD						dwSize;
	DWORD						enabled;
	DWORD						err;
	BOOL						b = CPropertyPage::OnSetActive();

	psheet = reinterpret_cast<CConfigPropertySheet*>(GetParent());
	require_quiet( psheet, exit );

	m_SMBCheckBox.SetCheck( 0 );

	// Now populate the browse domain box

	err = RegCreateKeyEx( HKEY_LOCAL_MACHINE, kServiceParametersNode L"\\Services\\SMB", 0,
		                  NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &key, NULL );
	require_noerr( err, exit );

	dwSize = sizeof( DWORD );
	err = RegQueryValueEx( key, L"Advertise", NULL, NULL, (LPBYTE) &enabled, &dwSize );
	require_noerr( err, exit );

	m_SMBCheckBox.SetCheck( enabled );

	RegCloseKey( key );
	key = NULL;

	m_powerManagementCheckBox.SetCheck( 0 );

	// Now populate the browse domain box

	err = RegCreateKeyEx( HKEY_LOCAL_MACHINE, kServiceParametersNode L"\\Power Management", 0,
		                  NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &key, NULL );
	require_noerr( err, exit );

	dwSize = sizeof( DWORD );
	err = RegQueryValueEx( key, L"Enabled", NULL, NULL, (LPBYTE) &enabled, &dwSize );
	require_noerr( err, exit );

	m_powerManagementCheckBox.SetCheck( enabled );
 
exit:

	if ( key )
	{
		RegCloseKey( key );
	}

	return b;
}


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::OnOK
//---------------------------------------------------------------------------------------------------------------------------

void
CServicesPage::OnOK()
{
	if ( m_modified )
	{
		Commit();
	}
}



//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::Commit
//---------------------------------------------------------------------------------------------------------------------------

void
CServicesPage::Commit()
{
	HKEY		key		= NULL;
	DWORD		enabled;
	DWORD		err;

	err = RegCreateKeyEx( HKEY_LOCAL_MACHINE, kServiceParametersNode L"\\Services\\SMB", 0,
	                   	NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &key, NULL );
	require_noerr( err, exit );

	enabled = m_SMBCheckBox.GetCheck();
	err = RegSetValueEx( key, L"Advertise", NULL, REG_DWORD, (LPBYTE) &enabled, sizeof( enabled ) );
	require_noerr( err, exit );

	RegCloseKey( key );
	key = NULL;

	err = RegCreateKeyEx( HKEY_LOCAL_MACHINE, kServiceParametersNode L"\\Power Management", 0,
		                  NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &key, NULL );
	require_noerr( err, exit );

	enabled = m_powerManagementCheckBox.GetCheck();
	err = RegSetValueEx( key, L"Enabled", NULL, REG_DWORD, (LPBYTE) &enabled, sizeof( enabled ) );
	require_noerr( err, exit );
	
exit:

	if ( key )
	{
		RegCloseKey( key );
	}
}


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::OnBnClickedAdvertiseSMB
//---------------------------------------------------------------------------------------------------------------------------

void CServicesPage::OnBnClickedAdvertiseSMB()
{
	SetModified( TRUE );
}


//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage::OnBnClickedPowerManagement
//---------------------------------------------------------------------------------------------------------------------------

void CServicesPage::OnBnClickedPowerManagement()
{
	SetModified( TRUE );

	if ( m_powerManagementCheckBox.GetCheck() )
	{
		CPowerManagementWarning dlg( GetParent() );

		dlg.DoModal();
	}
}


// CPowerManagementWarning dialog

IMPLEMENT_DYNAMIC(CPowerManagementWarning, CDialog)
CPowerManagementWarning::CPowerManagementWarning(CWnd* pParent /*=NULL*/)
	: CDialog(CPowerManagementWarning::IDD, pParent)
{
}

CPowerManagementWarning::~CPowerManagementWarning()
{
}

void CPowerManagementWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENERGY_SAVER, m_energySaverIcon);
}


BOOL
CPowerManagementWarning::OnInitDialog()
{	
	BOOL b = CDialog::OnInitDialog();

	const HICON hIcon = ( HICON ) ::LoadImage( GetNonLocalizedResources(), MAKEINTRESOURCE( IDI_ENERGY_SAVER ), IMAGE_ICON, 0, 0, 0);
	
	if ( hIcon )
	{
		m_energySaverIcon.SetIcon( hIcon );
	}

	return b;
}


void
CPowerManagementWarning::OnOK()
{
	CDialog::OnOK();
}


BEGIN_MESSAGE_MAP(CPowerManagementWarning, CDialog)
END_MESSAGE_MAP()
