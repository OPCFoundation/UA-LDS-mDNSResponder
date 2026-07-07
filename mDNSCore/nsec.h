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

#ifndef __NSEC_H
#define __NSEC_H

#include "dnssec.h"

extern mDNSBool AddNSECSForCacheRecord(mDNS *const m, CacheRecord *crlist, CacheRecord *negcr, mDNSu8 rcode);
extern void WildcardAnswerProof(mDNS *const m, DNSSECVerifier *dv);
extern void ValidateWithNSECS(mDNS *const m, DNSSECVerifier *dv, CacheRecord *rr);
extern mDNSBool NSECAnswersDS(mDNS *const m, ResourceRecord *rr, DNSQuestion *q);
extern int CountLabelsMatch(const domainname *const d1, const domainname *const d2);
extern void NameErrorNSECCallback(mDNS *const m, DNSSECVerifier *dv, DNSSECStatus status);
extern void VerifyNSEC(mDNS *const m, ResourceRecord *rr, RRVerifier *rv, DNSSECVerifier *pdv, CacheRecord *ncr,
	DNSSECVerifierCallback callback);
extern CacheRecord *NSECRecordIsDelegation(mDNS *const m, domainname *name, mDNSu16 qtype);
extern void NoDataNSECCallback(mDNS *const m, DNSSECVerifier *dv, DNSSECStatus status);

#endif // __NSEC_H
