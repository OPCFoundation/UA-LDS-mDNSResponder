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

#pragma once

#include "stdafx.h"
#include "resource.h"

#include <DebugServices.h>
#include <list>
#include "afxcmn.h"

#include "afxwin.h"





//---------------------------------------------------------------------------------------------------------------------------
//	CServicesPage
//---------------------------------------------------------------------------------------------------------------------------

class CServicesPage : public CPropertyPage
{
public:
	CServicesPage();
	~CServicesPage();

protected:

	//{{AFX_DATA(CServicesPage)
	enum { IDD = IDR_APPLET_PAGE5 };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CServicesPage)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	DECLARE_DYNCREATE(CServicesPage)

	//{{AFX_MSG(CServicesPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	
	typedef std::list<CString> StringList;

	afx_msg BOOL
	OnSetActive();
	
	afx_msg void
	OnOK();
	
	void
	SetModified( BOOL bChanged = TRUE );
	
	void
	Commit();

	BOOL			m_modified;

public:
private:

	CButton m_SMBCheckBox;
	CButton m_powerManagementCheckBox;

public:


	afx_msg void OnBnClickedAdvertiseSMB();
	afx_msg void OnBnClickedPowerManagement();
};


//---------------------------------------------------------------------------------------------------------------------------
//	CPowerManagementWarning
//---------------------------------------------------------------------------------------------------------------------------

class CPowerManagementWarning : public CDialog
{
	DECLARE_DYNAMIC(CPowerManagementWarning)

public:

	CPowerManagementWarning(CWnd* pParent = NULL);   // standard constructor

	virtual ~CPowerManagementWarning();

// Dialog Data

	enum { IDD = IDR_POWER_MANAGEMENT_WARNING };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:

	CStatic m_energySaverIcon;
};
