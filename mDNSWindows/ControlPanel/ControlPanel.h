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

//---------------------------------------------------------------------------------------------------------------------------
//	CCPApplet
//---------------------------------------------------------------------------------------------------------------------------

class CCPApplet : public CCmdTarget
{
public:

	CCPApplet( UINT nResourceID, UINT nDescriptionID, CRuntimeClass* pUIClass );

	virtual ~CCPApplet();

protected:

	virtual LRESULT OnRun(CWnd* pParentWnd);
	virtual LRESULT OnStartParms(CWnd* pParentWnd, LPCTSTR lpszExtra);
	virtual LRESULT OnInquire(CPLINFO* pInfo);
	virtual LRESULT OnNewInquire(NEWCPLINFO* pInfo);
	virtual LRESULT OnSelect();
	virtual LRESULT OnStop();

	CRuntimeClass	*	m_uiClass;
	UINT				m_resourceId;
	UINT				m_descId;
	CString				m_name;
	int					m_pageNumber;
  
	friend class CCPApp;

	DECLARE_DYNAMIC(CCPApplet);
};


//---------------------------------------------------------------------------------------------------------------------------
//	CCPApp
//---------------------------------------------------------------------------------------------------------------------------

class CCPApp : public CWinApp
{
public:

	CCPApp();
	virtual ~CCPApp();

	void AddApplet( CCPApplet* pApplet );

protected:

	CList<CCPApplet*, CCPApplet*&> m_applets;

	friend LONG APIENTRY
	CPlApplet(HWND hWndCPl, UINT uMsg, LONG lParam1, LONG lParam2);

	virtual LRESULT OnCplMsg(HWND hWndCPl, UINT msg, LPARAM lp1, LPARAM lp2);
	virtual LRESULT OnInit();
	virtual LRESULT OnExit();

	DECLARE_DYNAMIC(CCPApp);
};


CCPApp * GetControlPanelApp();
