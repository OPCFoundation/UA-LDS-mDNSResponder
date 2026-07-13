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

//----------------------------------------------------------------------------------------
//	Registry Constants
//----------------------------------------------------------------------------------------

#if defined(UNICODE)

#	define kServiceParametersSoftware			L"SOFTWARE"
#	define kServiceParametersAppleComputer		L"Apple Computer, Inc."
#	define kServiceParametersBonjour			L"Bonjour"
#	define kServiceParametersNode				L"SOFTWARE\\Apple Inc.\\OPCF Bonjour"
#	define kServiceName							L"OPCF Bonjour Service"
#	define kServiceDynDNSBrowseDomains			L"BrowseDomains"
#	define kServiceDynDNSHostNames				L"HostNames"
#	define kServiceDynDNSRegistrationDomains	L"RegistrationDomains"
#	define kServiceDynDNSDomains				L"Domains"	// value is comma separated list of domains
#	define kServiceDynDNSEnabled				L"Enabled"
#	define kServiceDynDNSStatus					L"Status"
#	define kServiceManageLLRouting				L"ManageLLRouting"
#	define kServiceCacheEntryCount				L"CacheEntryCount"
#	define kServiceManageFirewall				L"ManageFirewall"
#	define kServiceAdvertisedServices			L"Services"

# else

#	define kServiceParametersSoftware			"SOFTWARE"
#	define kServiceParametersAppleComputer		"Apple Computer, Inc."
#	define kServiceParametersBonjour			"Bonjour"
#	define kServiceParametersNode				"SOFTWARE\\Apple Inc.\\OPCF Bonjour"
#	define kServiceName							"OPCF Bonjour Service"
#	define kServiceDynDNSBrowseDomains			"BrowseDomains"
#	define kServiceDynDNSHostNames				"HostNames"
#	define kServiceDynDNSRegistrationDomains	"RegistrationDomains"
#	define kServiceDynDNSDomains				"Domains"	// value is comma separated list of domains
#	define kServiceDynDNSEnabled				"Enabled"
#	define kServiceDynDNSStatus					"Status"
#	define kServiceManageLLRouting				"ManageLLRouting"
#	define kServiceCacheEntryCount				"CacheEntryCount"
#	define kServiceManageFirewall				"ManageFirewall"

#endif
