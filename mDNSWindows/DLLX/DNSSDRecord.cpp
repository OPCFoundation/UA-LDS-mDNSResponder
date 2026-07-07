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

#include "DNSSDRecord.h"

#include "StringServices.h"

#include <DebugServices.h>





// CDNSSDRecord



STDMETHODIMP CDNSSDRecord::Update(DNSSDFlags flags, VARIANT rdata, ULONG ttl)

{

	std::vector< BYTE >	byteArray;

	const void		*	byteArrayPtr	= NULL;

	DNSServiceErrorType	err				= 0;

	HRESULT				hr				= 0;

	BOOL				ok;



	// Convert the VARIANT

	ok = VariantToByteArray( &rdata, byteArray );

	require_action( ok, exit, err = kDNSServiceErr_Unknown );



	err = DNSServiceUpdateRecord( m_serviceObject->GetSubordRef(), m_rref, flags, ( uint16_t ) byteArray.size(), byteArray.size() > 0 ? &byteArray[ 0 ] : NULL, ttl );

	require_noerr( err, exit );



exit:



	return err;

}





STDMETHODIMP CDNSSDRecord::Remove(DNSSDFlags flags)

{

	DNSServiceErrorType	err = 0;



	err = DNSServiceRemoveRecord( m_serviceObject->GetSubordRef(), m_rref, flags );

	require_noerr( err, exit );



exit:



	return err;

}
