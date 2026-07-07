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

#include "resource.h"

#include "DLLX.h"

#include "dlldatax.h"

#include <DebugServices.h>





class CDLLComponentModule : public CAtlDllModuleT< CDLLComponentModule >

{

public :

	DECLARE_LIBID(LIBID_Bonjour)

	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DLLX, "{56608F9C-223B-4CB6-813D-85EDCCADFB4B}")

};



CDLLComponentModule _AtlModule;





#ifdef _MANAGED

#pragma managed(push, off)

#endif



// DLL Entry Point

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)

{

	debug_initialize( kDebugOutputTypeWindowsDebugger );
	debug_set_property( kDebugPropertyTagPrintLevel, kDebugLevelVerbose );



#ifdef _MERGE_PROXYSTUB

    if (!PrxDllMain(hInstance, dwReason, lpReserved))

        return FALSE;

#endif

	hInstance;

    return _AtlModule.DllMain(dwReason, lpReserved); 

}



#ifdef _MANAGED

#pragma managed(pop)

#endif









// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)

{

#ifdef _MERGE_PROXYSTUB

    HRESULT hr = PrxDllCanUnloadNow();

    if (hr != S_OK)

        return hr;

#endif

    return _AtlModule.DllCanUnloadNow();

}





// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)

{

#ifdef _MERGE_PROXYSTUB

    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)

        return S_OK;

#endif

    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);

}





// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)

{

    // registers object, typelib and all interfaces in typelib

    HRESULT hr = _AtlModule.DllRegisterServer();

#ifdef _MERGE_PROXYSTUB

    if (FAILED(hr))

        return hr;

    hr = PrxDllRegisterServer();

#endif

	return hr;

}





// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)

{

	HRESULT hr = _AtlModule.DllUnregisterServer();

#ifdef _MERGE_PROXYSTUB

    if (FAILED(hr))

        return hr;

    hr = PrxDllRegisterServer();

    if (FAILED(hr))

        return hr;

    hr = PrxDllUnregisterServer();

#endif

	return hr;

}
