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

#include "PrinterSetupWizardSheet.h"
#include "CommonServices.h"
#include "UtilTypes.h"
#include "afxcmn.h"
#include "dns_sd.h"
#include "afxwin.h"
#include <map>

using namespace PrinterSetupWizard;

// CSecondPage dialog

class CSecondPage : public CPropertyPage
{
DECLARE_DYNAMIC(CSecondPage)

public:
CSecondPage();
virtual ~CSecondPage();

// Dialog Data
enum { IDD = IDD_SECOND_PAGE };

protected:

void         InitBrowseList();
virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
virtual BOOL OnSetActive();
virtual BOOL OnKillActive();

DECLARE_MESSAGE_MAP()

public:

HTREEITEM m_emptyListItem;
bool m_selectOkay;
CTreeCtrl m_browseList;
bool m_initialized;
bool m_waiting;

afx_msg void    OnTvnSelchangedBrowseList(NMHDR *pNMHDR, LRESULT *pResult);
afx_msg void    OnNmClickBrowseList(NMHDR * pNMHDR, LRESULT * pResult);
afx_msg void    OnTvnKeyDownBrowseList(NMHDR * pNMHDR, LRESULT * pResult );

OSStatus
OnAddPrinter(
    Printer     *   printer,
    bool moreComing);

OSStatus
OnRemovePrinter(
    Printer     *   printer,
    bool moreComing);

void
OnResolveService( Service * service );

private:

void
LoadTextAndDisableWindow( CString & text );

void
SetPrinterInformationState( BOOL state );

std::string m_selectedName;


private:



CStatic m_printerInformation;

CStatic m_descriptionLabel;

CStatic m_descriptionField;

CStatic m_locationLabel;

CStatic m_locationField;


bool m_gotChoice;
};
