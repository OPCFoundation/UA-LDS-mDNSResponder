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

#ifndef __ANONYMOUS_H_
#define __ANONYMOUS_H_

extern void ReInitAnonInfo(AnonymousInfo **si, const domainname *name);
extern AnonymousInfo *AllocateAnonInfo(const domainname *service, const mDNSu8 *AnonData, int len, const ResourceRecord *rr);
extern void FreeAnonInfo(AnonymousInfo *ai);
extern void SetAnonData(DNSQuestion *q, ResourceRecord *rr, mDNSBool ForQuestion);
extern int AnonInfoAnswersQuestion(const ResourceRecord *const rr, const DNSQuestion *const q);
extern void InitializeAnonInfoForCR(mDNS *const m, CacheRecord **McastNSEC3Records, CacheRecord *cr);
extern void InitializeAnonInfoForQuestion(mDNS *const m, CacheRecord **McastNSEC3Records, DNSQuestion *q);
extern void CopyAnonInfoForCR(mDNS *const m, CacheRecord *crto, CacheRecord *crfrom);
extern mDNSBool IdenticalAnonInfo(AnonymousInfo *a1, AnonymousInfo *a2);

#endif
