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

#include "resource.h"       // main symbols



#include "DLLX.h"

#include "_IDNSSDEvents_CP.H"





#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)

#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."

#endif







// CDNSSDEventManager



class ATL_NO_VTABLE CDNSSDEventManager :

	public CComObjectRootEx<CComSingleThreadModel>,

	public CComCoClass<CDNSSDEventManager, &CLSID_DNSSDEventManager>,

	public IConnectionPointContainerImpl<CDNSSDEventManager>,

	public CProxy_IDNSSDEvents<CDNSSDEventManager>,

	public IDispatchImpl<IDNSSDEventManager, &IID_IDNSSDEventManager, &LIBID_Bonjour, /*wMajor =*/ 1, /*wMinor =*/ 0>

{

public:

	CDNSSDEventManager()

	{

	}



DECLARE_REGISTRY_RESOURCEID(IDR_DNSSDEVENTMANAGER)





BEGIN_COM_MAP(CDNSSDEventManager)

	COM_INTERFACE_ENTRY(IDNSSDEventManager)

	COM_INTERFACE_ENTRY(IDispatch)

	COM_INTERFACE_ENTRY(IConnectionPointContainer)

END_COM_MAP()



BEGIN_CONNECTION_POINT_MAP(CDNSSDEventManager)

	CONNECTION_POINT_ENTRY(__uuidof(_IDNSSDEvents))

END_CONNECTION_POINT_MAP()





	DECLARE_PROTECT_FINAL_CONSTRUCT()



	HRESULT FinalConstruct()

	{

		return S_OK;

	}



	void FinalRelease()

	{

	}



public:



};



OBJECT_ENTRY_AUTO(__uuidof(DNSSDEventManager), CDNSSDEventManager)
