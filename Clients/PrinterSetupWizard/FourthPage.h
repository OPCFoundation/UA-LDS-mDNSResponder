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
#include "afxwin.h"


// CFourthPage dialog

class CFourthPage : public CPropertyPage
{
DECLARE_DYNAMIC(CFourthPage)

public:
CFourthPage();
virtual ~CFourthPage();

// Dialog Data
enum { IDD = IDD_FOURTH_PAGE };

virtual BOOL OnSetActive();
virtual BOOL OnKillActive();

BOOL StartActivityIndicator();
BOOL StopActivityIndicator();

protected:
virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support

DECLARE_MESSAGE_MAP()

private:

OSStatus    OnInitPage();
CFont m_largeFont;
bool m_initialized;


public:
CStatic m_goodbye;
private:
CStatic m_printerNameCtrl;
CStatic m_printerManufacturerCtrl;
CStatic m_printerModelCtrl;
CStatic m_printerProtocolCtrl;
CStatic m_printerDefault;
};
