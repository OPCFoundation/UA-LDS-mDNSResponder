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

#include <vector>

#include <dns_sd.h>





#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)

#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."

#endif







// CTXTRecord



class ATL_NO_VTABLE CTXTRecord :

	public CComObjectRootEx<CComSingleThreadModel>,

	public CComCoClass<CTXTRecord, &CLSID_TXTRecord>,

	public IDispatchImpl<ITXTRecord, &IID_ITXTRecord, &LIBID_Bonjour, /*wMajor =*/ 1, /*wMinor =*/ 0>

{

public:

	CTXTRecord()

	:

		m_allocated( FALSE )

	{

	}



DECLARE_REGISTRY_RESOURCEID(IDR_TXTRECORD)





BEGIN_COM_MAP(CTXTRecord)

	COM_INTERFACE_ENTRY(ITXTRecord)

	COM_INTERFACE_ENTRY(IDispatch)

END_COM_MAP()







	DECLARE_PROTECT_FINAL_CONSTRUCT()



	HRESULT FinalConstruct()

	{

		return S_OK;

	}



	void FinalRelease()

	{

		if ( m_allocated )

		{

			TXTRecordDeallocate( &m_tref );

		}

	}



public:



	STDMETHOD(SetValue)(BSTR key, VARIANT value);

	STDMETHOD(RemoveValue)(BSTR key);

	STDMETHOD(ContainsKey)(BSTR key, VARIANT_BOOL* retval);

	STDMETHOD(GetValueForKey)(BSTR key, VARIANT* value);

	STDMETHOD(GetCount)(ULONG* count);

	STDMETHOD(GetKeyAtIndex)(ULONG index, BSTR* retval);

	STDMETHOD(GetValueAtIndex)(ULONG index, VARIANT* retval);



private:



	typedef std::vector< BYTE > ByteArray;

	ByteArray		m_byteArray;

	BOOL			m_allocated;

	TXTRecordRef	m_tref;



public:



	uint16_t

	GetLen()

	{

		return TXTRecordGetLength( &m_tref );

	}



	const void*

	GetBytes()

	{

		return TXTRecordGetBytesPtr( &m_tref );

	}



	void

	SetBytes

		(

		const unsigned char	*	bytes,

		uint16_t				len

		);

};



OBJECT_ENTRY_AUTO(__uuidof(TXTRecord), CTXTRecord)
