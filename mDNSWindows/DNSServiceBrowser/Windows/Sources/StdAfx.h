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

#if !defined(AFX_STDAFX_H__424305D2_0A97_4AA0_B9B1_A7D90D18EBA0__INCLUDED_)
#define AFX_STDAFX_H__424305D2_0A97_4AA0_B9B1_A7D90D18EBA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
	#define WINVER 0x0400	// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#include	<afxwin.h>		// MFC core and standard components
#include	<afxext.h>		// MFC extensions
#include	<afxdtctl.h>	// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include	<afxcmn.h>	// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include	<winsock2.h>

#include	<stdlib.h>

#include	"DNSServices.h"

#include	"Application.h"

#include	"ChooserDialog.h"

#endif // !defined(AFX_STDAFX_H__424305D2_0A97_4AA0_B9B1_A7D90D18EBA0__INCLUDED_)
