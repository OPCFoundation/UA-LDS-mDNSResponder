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

#include "StringServices.h"

#include <DebugServices.h>





extern BOOL

BSTRToUTF8

	(

	BSTR			inString,

	std::string	&	outString

	)

{

	USES_CONVERSION;

	

	char	*	utf8String	= NULL;

	OSStatus    err			= kNoErr;



	outString = "";

	if ( inString )

	{
		TCHAR	*	utf16String	= NULL;
		size_t      size		= 0;


		utf16String = OLE2T( inString );

		require_action( utf16String != NULL, exit, err = kUnknownErr );



		if ( wcslen( utf16String ) > 0 )

		{

			size = (size_t) WideCharToMultiByte( CP_UTF8, 0, utf16String, ( int ) wcslen( utf16String ), NULL, 0, NULL, NULL );

			err = translate_errno( size != 0, GetLastError(), kUnknownErr );

			require_noerr( err, exit );



			try

			{

				utf8String = new char[ size + 1 ];

			}

			catch ( ... )

			{

				utf8String = NULL;

			}



			require_action( utf8String != NULL, exit, err = kNoMemoryErr );

			size = (size_t) WideCharToMultiByte( CP_UTF8, 0, utf16String, ( int ) wcslen( utf16String ), utf8String, (int) size, NULL, NULL);

			err = translate_errno( size != 0, GetLastError(), kUnknownErr );

			require_noerr( err, exit );



			// have to add the trailing 0 because WideCharToMultiByte doesn't do it,

			// although it does return the correct size



			utf8String[size] = '\0';

			outString = utf8String;

		}
	}



exit:



	if ( utf8String != NULL )

	{

		delete [] utf8String;

	}



	return ( !err ) ? TRUE : FALSE;

}





extern BOOL

UTF8ToBSTR

	(

	const char	*	inString,

	CComBSTR	&	outString

	)

{

	wchar_t	*	unicode	= NULL;

	OSStatus	err		= 0;



	if ( inString )

	{
		int n;

		n = MultiByteToWideChar( CP_UTF8, 0, inString, -1, NULL, 0 );

	    

		if ( n > 0 )

		{

			try

			{

				unicode = new wchar_t[ n ];

			}

			catch ( ... )

			{

				unicode = NULL;

			}



			require_action( unicode, exit, err = ERROR_INSUFFICIENT_BUFFER );



			n = MultiByteToWideChar( CP_UTF8, 0, inString, -1, unicode, n );

		}



		outString = unicode;

	}


exit:



    if ( unicode != NULL )

    {

        delete [] unicode;

	}



	return ( !err ) ? TRUE : FALSE;

}





BOOL

ByteArrayToVariant

	(

	const void	*	inArray,

	size_t			inArrayLen,

	VARIANT		*	outVariant

	)

{

	LPBYTE			buf	= NULL;

	HRESULT			hr	= 0;

	BOOL			ok	= TRUE;



	VariantClear( outVariant );

	outVariant->vt		= VT_ARRAY|VT_UI1;

	outVariant->parray	= SafeArrayCreateVector( VT_UI1, 0, ( ULONG ) inArrayLen );

	require_action( outVariant->parray, exit, ok = FALSE );

	hr = SafeArrayAccessData( outVariant->parray, (LPVOID *)&buf );

	require_action( hr == S_OK, exit, ok = FALSE );

	memcpy( buf, inArray, inArrayLen );

	hr = SafeArrayUnaccessData( outVariant->parray );

	require_action( hr == S_OK, exit, ok = FALSE );



exit:



	return ok;

}





extern BOOL

VariantToByteArray
	(
	VARIANT				*	inVariant,
	std::vector< BYTE >	&	outArray
	)
{
	BOOL ok = TRUE;

	if ( V_VT( inVariant ) == VT_BSTR )
	{
		BSTR bstr = V_BSTR( inVariant );
		std::string utf8;

		BSTRToUTF8( bstr, utf8 );

		outArray.reserve( utf8.size() );
		outArray.assign( utf8.begin(), utf8.end() );
	}
	else if ( V_VT( inVariant ) == VT_ARRAY )
	{
		SAFEARRAY	*	psa			= NULL;
		BYTE		*	pData		= NULL;
		ULONG			cElements	= 0;
		HRESULT			hr;
		
		psa = V_ARRAY( inVariant );

		require_action( psa, exit, ok = FALSE );

		require_action( SafeArrayGetDim( psa ) == 1, exit, ok = FALSE );

		hr = SafeArrayAccessData( psa, ( LPVOID* )&pData );

		require_action( hr == S_OK, exit, ok = FALSE );

		cElements = psa->rgsabound[0].cElements;

		outArray.reserve( cElements );

		outArray.assign( cElements, 0 );

		memcpy( &outArray[ 0 ], pData, cElements );

		SafeArrayUnaccessData( psa );
	}
	else
	{
		ok = FALSE;
	}

exit:

	return ok;

}
