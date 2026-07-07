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
//	CBrowsingPage
//---------------------------------------------------------------------------------------------------------------------------

class CBrowsingPage : public CPropertyPage
{
public:
	CBrowsingPage();
	~CBrowsingPage();

protected:

	//{{AFX_DATA(CBrowsingPage)
	enum { IDD = IDR_APPLET_PAGE3 };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBrowsingPage)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	DECLARE_DYNCREATE(CBrowsingPage)

	//{{AFX_MSG(CBrowsingPage)
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

	static int CALLBACK 

	SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);



	CListCtrl	m_browseListCtrl;

	bool		m_initialized;

	bool		m_firstTime;



public:



	afx_msg void OnBnClickedAddBrowseDomain();

	afx_msg void OnBnClickedRemoveBrowseDomain();

	afx_msg void OnLvnItemchangedBrowseList(NMHDR *pNMHDR, LRESULT *pResult);

	CButton m_removeButton;

};





//---------------------------------------------------------------------------------------------------------------------------
//	CAddBrowseDomain
//---------------------------------------------------------------------------------------------------------------------------


class CAddBrowseDomain : public CDialog

{

	DECLARE_DYNAMIC(CAddBrowseDomain)



public:

	CAddBrowseDomain(CWnd* pParent = NULL);   // standard constructor

	virtual ~CAddBrowseDomain();



// Dialog Data

	enum { IDD = IDR_ADD_BROWSE_DOMAIN };



protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:

	CComboBox	m_comboBox;

	CString		m_text;

};
