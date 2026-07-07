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

#ifndef	__LOGIN_DIALOG__
#define	__LOGIN_DIALOG__

#pragma once

#include	"Resource.h"

//===========================================================================================================================
//	LoginDialog
//===========================================================================================================================

class	LoginDialog : public CDialog
{
	protected:
	
		CString		mUsername;
		CString		mPassword;
		
	public:
		
		enum { IDD = IDD_LOGIN };
		
		LoginDialog( CWnd *inParent = NULL );
		
		virtual BOOL	GetLogin( CString &outUsername, CString &outPassword );
	
	protected:

		virtual BOOL	OnInitDialog( void );
		virtual void	DoDataExchange( CDataExchange *inDX );
		virtual void	OnOK( void );
		
		DECLARE_MESSAGE_MAP()
};

#endif	// __LOGIN_DIALOG__
