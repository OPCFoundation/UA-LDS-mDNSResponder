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

/* loclibrary.h                                                      
 * ----------------------------------------------------------------------
 * Header file for localization library                                       
 * Originally created by jsantamaria: 3 may 2004                         
 * ----------------------------------------------------------------------
 */
 
#ifndef _loclibrary_h_
#define _loclibrary_h_


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int PathForResourceW ( HMODULE module, const wchar_t *name, wchar_t *locFile, int locFileLen);
int PathForResourceWithPathW ( const wchar_t *path, const wchar_t *name, wchar_t *locFile, int locFileLen);

int PathForResourceA ( HMODULE module, const char *name, char *locFile, int locFileLen);
int PathForResourceWithPathA ( const char *path, const char *name, char *locFile, int locFileLen);


#ifdef UNICODE
#define PathForResource PathForResourceW
#define PathForResourceWithPath PathForResourceWithPathW
#else
#define PathForResource PathForResourceA
#define PathForResourceWithPath PathForResourceWithPathA
#endif // UNICODE


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // _loclibrary_h_
