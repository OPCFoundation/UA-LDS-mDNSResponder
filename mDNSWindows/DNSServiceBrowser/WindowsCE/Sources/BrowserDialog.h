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

#if !defined(AFX_BROWSERDIALOG_H__DECC5C82_C1C6_4630_B8D5_E1DDE570A061__INCLUDED_)
#define AFX_BROWSERDIALOG_H__DECC5C82_C1C6_4630_B8D5_E1DDE570A061__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include	"afxtempl.h"
#include	"Resource.h"

#include	"DNSServices.h"

//===========================================================================================================================
//	BrowserDialog
//===========================================================================================================================

class	BrowserDialog : public CDialog
{
	public:
		
		BrowserDialog( CWnd *inParent = NULL );
		
		//{{AFX_DATA(BrowserDialog)
		enum { IDD = IDD_APPLICATION_DIALOG };
		CListCtrl	mBrowserList;
		//}}AFX_DATA

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(BrowserDialog)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		//}}AFX_VIRTUAL
		
		static void
			OnBrowserCallBack( 
				void *					inContext, 
				DNSBrowserRef			inRef, 
				DNSStatus				inStatusCode,  
				const DNSBrowserEvent *	inEvent );
		
	protected:
		
		struct	BrowserEntry
		{
			CString		name;
			CString		ip;
			CString		text;
		};
		
		HICON										mIcon;
		DNSBrowserRef								mBrowser;
		CArray < BrowserEntry, BrowserEntry >		mBrowserEntries;
		
		// Generated message map functions
		//{{AFX_MSG(BrowserDialog)
		virtual BOOL OnInitDialog();
		afx_msg void OnBrowserListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg LONG OnServiceAdd( WPARAM inWParam, LPARAM inLParam );
		afx_msg LONG OnServiceRemove( WPARAM inWParam, LPARAM inLParam );
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERDIALOG_H__DECC5C82_C1C6_4630_B8D5_E1DDE570A061__INCLUDED_)
