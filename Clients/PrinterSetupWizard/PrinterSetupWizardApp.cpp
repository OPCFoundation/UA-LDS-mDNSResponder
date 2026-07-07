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

#include "stdafx.h"
#include "PrinterSetupWizardApp.h"
#include "PrinterSetupWizardSheet.h"
#include "DebugServices.h"
#include "loclibrary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef HeapEnableTerminationOnCorruption
#	define HeapEnableTerminationOnCorruption (HEAP_INFORMATION_CLASS) 1
#endif


// Stash away pointers to our resource DLLs

static HINSTANCE g_nonLocalizedResources	= NULL;
static HINSTANCE g_localizedResources		= NULL;


HINSTANCE
GetNonLocalizedResources()
{
	return g_nonLocalizedResources;
}


HINSTANCE
GetLocalizedResources()
{
	return g_localizedResources;
}


// CPrinterSetupWizardApp

BEGIN_MESSAGE_MAP(CPrinterSetupWizardApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPrinterSetupWizardApp construction

CPrinterSetupWizardApp::CPrinterSetupWizardApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPrinterSetupWizardApp object

CPrinterSetupWizardApp theApp;


// CPrinterSetupWizardApp initialization

BOOL CPrinterSetupWizardApp::InitInstance()
{
	CString		errorMessage;
	CString		errorCaption;
	wchar_t		resource[MAX_PATH];
	int			res;
	OSStatus	err = kNoErr;

	HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, 0 );

	//
	// initialize the debugging framework
	//
	debug_initialize( kDebugOutputTypeWindowsDebugger, "PrinterSetupWizard", NULL );
	debug_set_property( kDebugPropertyTagPrintLevel, kDebugLevelTrace );

	// Before we load the resources, let's load the error string

	errorMessage.LoadString( IDS_REINSTALL );
	errorCaption.LoadString( IDS_REINSTALL_CAPTION );

	// Load Resources

	res = PathForResource( NULL, L"PrinterWizardResources.dll", resource, MAX_PATH );
	err = translate_errno( res != 0, kUnknownErr, kUnknownErr );
	require_noerr( err, exit );

	g_nonLocalizedResources = LoadLibrary( resource );
	translate_errno( g_nonLocalizedResources, GetLastError(), kUnknownErr );
	require_noerr( err, exit );

	res = PathForResource( NULL, L"PrinterWizardLocalized.dll", resource, MAX_PATH );
	err = translate_errno( res != 0, kUnknownErr, kUnknownErr );
	require_noerr( err, exit );

	g_localizedResources = LoadLibrary( resource );
	translate_errno( g_localizedResources, GetLastError(), kUnknownErr );
	require_noerr( err, exit );

	AfxSetResourceHandle( g_localizedResources );

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	{
		CPrinterSetupWizardSheet dlg(IDS_CAPTION);

		m_pMainWnd = &dlg;

		try
		{
			INT_PTR nResponse = dlg.DoModal();
		
			if (nResponse == IDOK)
			{
				// TODO: Place code here to handle when the dialog is
				//  dismissed with OK
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: Place code here to handle when the dialog is
				//  dismissed with Cancel
			}
		}
		catch (CPrinterSetupWizardSheet::WizardException & exc)
		{
			MessageBox(NULL, exc.text, exc.caption, MB_OK|MB_ICONEXCLAMATION);
		}
	}

exit:

	if ( err )
	{
		MessageBox( NULL, errorMessage, errorCaption, MB_ICONERROR | MB_OK );
	}

	if ( g_nonLocalizedResources )
	{
		FreeLibrary( g_nonLocalizedResources );
	}

	if ( g_localizedResources )
	{
		FreeLibrary( g_localizedResources );
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
