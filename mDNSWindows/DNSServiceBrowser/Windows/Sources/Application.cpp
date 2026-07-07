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

#include	<assert.h>

#include	"StdAfx.h"

#include	"DNSServices.h"

#include	"Application.h"

#include	"ChooserDialog.h"

#include	"stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================================================================================
//	Message Map
//===========================================================================================================================

BEGIN_MESSAGE_MAP(Application, CWinApp)
	//{{AFX_MSG_MAP(Application)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//===========================================================================================================================
//	Globals
//===========================================================================================================================

Application		gApp;

//===========================================================================================================================
//	Application
//===========================================================================================================================

Application::Application( void )
{
	//
}

//===========================================================================================================================
//	InitInstance
//===========================================================================================================================

BOOL	Application::InitInstance()
{
	DNSStatus		err;
	
	// Standard MFC initialization.

#if( !defined( AFX_DEPRECATED ) )
	#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif

	InitCommonControls();
	
	// Set up DNS Services.
	
	err = DNSServicesInitialize( 0, 512 );
	assert( err == kDNSNoErr );
	
	// Create the chooser dialog.
	
	ChooserDialog *		dialog;
	
	m_pMainWnd = NULL;
	dialog = new ChooserDialog;
	dialog->Create( IDD_CHOOSER_DIALOG );
	m_pMainWnd = dialog;
	dialog->ShowWindow( SW_SHOW );
	
	return( true );
}

//===========================================================================================================================
//	ExitInstance
//===========================================================================================================================

int	Application::ExitInstance( void )
{
	// Clean up DNS Services.
	
	DNSServicesFinalize();
	return( 0 );
}
