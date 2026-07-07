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

// Define the required CFM Shared Library entry and exit points
#include <CodeFragments.h>
#include "mDNSEmbeddedAPI.h"            // Defines the interface to the client layer above
#include "mDNSMacOS9.h"                 // Defines the specific types needed to run mDNS on this platform

mDNS mDNSStorage;
static mDNS_PlatformSupport PlatformSupportStorage;
// Start off with a default cache of 16K (about 100 records)
#define RR_CACHE_SIZE ((16*1024) / sizeof(CacheRecord))
static CacheEntity rrcachestorage[RR_CACHE_SIZE];

mDNSlocal void mDNS_StatusCallback(mDNS *const m, mStatus result)
{
    if (result == mStatus_GrowCache)
    {
        // Allocate another chunk of cache storage
        CacheEntity *storage = OTAllocMem(sizeof(CacheEntity) * RR_CACHE_SIZE);
        if (storage) mDNS_GrowCache(m, storage, RR_CACHE_SIZE);
    }
}

extern pascal OSErr mDNS_CFMInit(const CFragInitBlock *theInitBlock);
pascal OSErr mDNS_CFMInit(const CFragInitBlock *theInitBlock)
{
    extern pascal OSErr __initialize(const CFragInitBlock *theInitBlock);
    __initialize(theInitBlock); // MUST do this first!
    {
        mStatus err;
        THz oldZone = GetZone();
        SetZone(SystemZone());
        LogMsg("mDNS/DNS-SD with Macsbug breaks -- do not ship this version to customers");
        err = mDNS_Init(&mDNSStorage, &PlatformSupportStorage, rrcachestorage, RR_CACHE_SIZE,
                        mDNS_Init_AdvertiseLocalAddresses, mDNS_StatusCallback, mDNS_Init_NoInitCallbackContext);
        SetZone(oldZone);
        return((OSErr)err);
    }
}

extern void mDNS_CFMTerm(void);
void mDNS_CFMTerm(void)
{
    extern pascal void __terminate(void);
    LogMsg("mDNS_CFMTerm");
    mDNS_Close(&mDNSStorage);
    __terminate();
}
