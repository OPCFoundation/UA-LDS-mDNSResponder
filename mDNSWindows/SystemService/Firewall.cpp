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

// <rdar://problem/4278931> Doesn't compile correctly with latest Platform SDK

#if !defined(_WIN32_DCOM)
#	define _WIN32_DCOM 
#endif


#include "Firewall.h"
#include <windows.h>
#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>


static const int kMaxTries			= 30;
static const int kRetrySleepPeriod	= 1 * 1000; // 1 second


static OSStatus
mDNSFirewallInitialize(OUT INetFwProfile ** fwProfile)
{
	INetFwMgr		*	fwMgr		= NULL;
	INetFwPolicy	*	fwPolicy	= NULL;
	int					numRetries	= 0;
	HRESULT				err			= kNoErr;
    
	_ASSERT(fwProfile != NULL);

    *fwProfile = NULL;

	// Use COM to get a reference to the firewall settings manager.  This
	// call will fail on anything other than XP SP2

	err = CoCreateInstance( __uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwMgr), (void**)&fwMgr );
	require(SUCCEEDED(err) && ( fwMgr != NULL ), exit);

	// Use the reference to get the local firewall policy

	err = fwMgr->get_LocalPolicy(&fwPolicy);
	require(SUCCEEDED(err) && ( fwPolicy != NULL ), exit);

	// Use the reference to get the extant profile. Empirical evidence
	// suggests that there is the potential for a race condition when a system
	// service whose startup type is automatic calls this method.
	// This is true even when the service declares itself to be dependent
	// on the firewall service. Re-trying the method will succeed within
	// a few seconds.

	do
	{
    	err = fwPolicy->get_CurrentProfile(fwProfile);

		if (err)
		{
			Sleep(kRetrySleepPeriod);
		}
	}
	while (err && (numRetries++ < kMaxTries));

	require(SUCCEEDED(err), exit);

	err = kNoErr;

exit:

	// Release temporary COM objects

    if (fwPolicy != NULL)
    {
        fwPolicy->Release();
    }

    if (fwMgr != NULL)
    {
        fwMgr->Release();
    }

    return err;
}


static void
mDNSFirewallCleanup
			(
			IN INetFwProfile	*	fwProfile
			)
{
	// Call Release on the COM reference.

    if (fwProfile != NULL)
    {
        fwProfile->Release();
    }
}


static OSStatus
mDNSFirewallAppIsEnabled
			(
			IN INetFwProfile	*	fwProfile,
			IN const wchar_t	*	fwProcessImageFileName,
			OUT BOOL			*	fwAppEnabled    
			)
{
	BSTR							fwBstrProcessImageFileName = NULL;
	VARIANT_BOOL					fwEnabled;
	INetFwAuthorizedApplication	*	fwApp	= NULL;
	INetFwAuthorizedApplications*	fwApps	= NULL;
	OSStatus						err		= kNoErr;
    
	_ASSERT(fwProfile != NULL);
	_ASSERT(fwProcessImageFileName != NULL);
	_ASSERT(fwAppEnabled != NULL);

    *fwAppEnabled = FALSE;

	// Get the list of authorized applications

	err = fwProfile->get_AuthorizedApplications(&fwApps);
	require(SUCCEEDED(err) && ( fwApps != NULL ), exit);

    fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
	require_action( ( fwProcessImageFileName != NULL ) && ( SysStringLen(fwBstrProcessImageFileName) > 0 ), exit, err = kNoMemoryErr);

	// Look for us

    err = fwApps->Item(fwBstrProcessImageFileName, &fwApp);
	
    if (SUCCEEDED(err) && ( fwApp != NULL ) )
    {
        // It's listed, but is it enabled?

		err = fwApp->get_Enabled(&fwEnabled);
		require(SUCCEEDED(err), exit);

        if (fwEnabled != VARIANT_FALSE)
        {
			// Yes, it's enabled

            *fwAppEnabled = TRUE;
		}
	}

	err = kNoErr;

exit:

	// Deallocate the BSTR

	if ( fwBstrProcessImageFileName != NULL )
	{
		SysFreeString(fwBstrProcessImageFileName);
	}

	// Release the COM objects

    if (fwApp != NULL)
    {
        fwApp->Release();
    }

    if (fwApps != NULL)
    {
        fwApps->Release();
    }

    return err;
}


static OSStatus
mDNSFirewallAddApp
			(
            IN INetFwProfile	*	fwProfile,
            IN const wchar_t	*	fwProcessImageFileName,
            IN const wchar_t	*	fwName
            )
{
	BOOL							fwAppEnabled;
	BSTR							fwBstrName = NULL;
	BSTR							fwBstrProcessImageFileName = NULL;
	INetFwAuthorizedApplication	*	fwApp = NULL;
	INetFwAuthorizedApplications*	fwApps = NULL;
	OSStatus						err = S_OK;
    
	_ASSERT(fwProfile != NULL);
    _ASSERT(fwProcessImageFileName != NULL);
    _ASSERT(fwName != NULL);

    // First check to see if the application is already authorized.
	err = mDNSFirewallAppIsEnabled( fwProfile, fwProcessImageFileName, &fwAppEnabled );
	require_noerr(err, exit);

	// Only add the application if it isn't enabled

	if (!fwAppEnabled)
	{
		// Get the list of authorized applications

        err = fwProfile->get_AuthorizedApplications(&fwApps);
		require(SUCCEEDED(err) && ( fwApps != NULL ), exit);

        // Create an instance of an authorized application.

		err = CoCreateInstance( __uuidof(NetFwAuthorizedApplication), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwAuthorizedApplication), (void**)&fwApp );
		require(SUCCEEDED(err) && ( fwApp != NULL ), exit);

        fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
		require_action(( fwProcessImageFileName != NULL ) && ( SysStringLen(fwBstrProcessImageFileName) > 0 ), exit, err = kNoMemoryErr);

		// Set the executable file name

		err = fwApp->put_ProcessImageFileName(fwBstrProcessImageFileName);
		require(SUCCEEDED(err), exit);

		fwBstrName = SysAllocString(fwName);
		require_action( ( fwBstrName != NULL ) && ( SysStringLen(fwBstrName) > 0 ), exit, err = kNoMemoryErr);

		// Set the friendly name

        err = fwApp->put_Name(fwBstrName);
		require(SUCCEEDED(err), exit);

		// Now add the application

        err = fwApps->Add(fwApp);
		require(SUCCEEDED(err), exit);
	}

	err = kNoErr;

exit:

	// Deallocate the BSTR objects

	if ( fwBstrName != NULL )
	{
		SysFreeString(fwBstrName);
	}

	if ( fwBstrProcessImageFileName != NULL )
	{
		SysFreeString(fwBstrProcessImageFileName);
	}

    // Release the COM objects

    if (fwApp != NULL)
    {
        fwApp->Release();
    }

    if (fwApps != NULL)
    {
        fwApps->Release();
    }

    return err;
}





static OSStatus

mDNSFirewallIsFileAndPrintSharingEnabled

	(

	IN INetFwProfile	* fwProfile,

	OUT BOOL			* fwServiceEnabled

	)

{

    VARIANT_BOOL fwEnabled;

    INetFwService* fwService = NULL;

    INetFwServices* fwServices = NULL;

	OSStatus err = S_OK;



    _ASSERT(fwProfile != NULL);

    _ASSERT(fwServiceEnabled != NULL);



    *fwServiceEnabled = FALSE;



    // Retrieve the globally open ports collection.

    err = fwProfile->get_Services(&fwServices);

	require( SUCCEEDED( err ), exit );



    // Attempt to retrieve the globally open port.

    err = fwServices->Item(NET_FW_SERVICE_FILE_AND_PRINT, &fwService);

	require( SUCCEEDED( err ), exit );

	

	// Find out if the globally open port is enabled.

    err = fwService->get_Enabled(&fwEnabled);

	require( SUCCEEDED( err ), exit );

	if (fwEnabled != VARIANT_FALSE)

	{

		*fwServiceEnabled = TRUE;

	}



exit:



    // Release the globally open port.

    if (fwService != NULL)

    {

        fwService->Release();

    }



    // Release the globally open ports collection.

    if (fwServices != NULL)

    {

        fwServices->Release();

    }



    return err;

}


OSStatus
mDNSAddToFirewall
		(
		LPWSTR	executable,
		LPWSTR	name
		)
{
	INetFwProfile	*	fwProfile	= NULL;
	HRESULT				comInit		= E_FAIL;
	OSStatus			err			= kNoErr;

	// Initialize COM.

	comInit = CoInitializeEx( 0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );

	// Ignore this case. RPC_E_CHANGED_MODE means that COM has already been
	// initialized with a different mode.

	if (comInit != RPC_E_CHANGED_MODE)
	{
		err = comInit;
		require(SUCCEEDED(err), exit);
	}

	// Connect to the firewall

	err = mDNSFirewallInitialize(&fwProfile);
	require( SUCCEEDED( err ) && ( fwProfile != NULL ), exit);

	// Add us to the list of exempt programs

	err = mDNSFirewallAddApp( fwProfile, executable, name );
	require_noerr(err, exit);

exit:

	// Disconnect from the firewall

	if ( fwProfile != NULL )
	{
		mDNSFirewallCleanup(fwProfile);
	}

	// De-initialize COM

	if (SUCCEEDED(comInit))
    {
        CoUninitialize();
    }

	return err;
}


BOOL
mDNSIsFileAndPrintSharingEnabled( BOOL * retry )
{
	INetFwProfile	*	fwProfile					= NULL;
	HRESULT				comInit						= E_FAIL;
	BOOL				enabled						= FALSE;
	OSStatus			err							= kNoErr;

	// Initialize COM.

	*retry = FALSE;
	comInit = CoInitializeEx( 0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );

	// Ignore this case. RPC_E_CHANGED_MODE means that COM has already been
	// initialized with a different mode.

	if (comInit != RPC_E_CHANGED_MODE)
	{
		*retry = TRUE;
		err = comInit;
		require(SUCCEEDED(err), exit);
	}

	// Connect to the firewall

	err = mDNSFirewallInitialize(&fwProfile);
	require( SUCCEEDED( err ) && ( fwProfile != NULL ), exit);

	err = mDNSFirewallIsFileAndPrintSharingEnabled( fwProfile, &enabled );
	require_noerr( err, exit );

exit:

	// Disconnect from the firewall

	if ( fwProfile != NULL )
	{
		mDNSFirewallCleanup(fwProfile);
	}

	// De-initialize COM

	if (SUCCEEDED(comInit))
    {
        CoUninitialize();
    }

	return enabled;
}
