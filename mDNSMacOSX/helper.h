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

#ifndef H_HELPER_H
#define H_HELPER_H

#define kmDNSHelperServiceName "com.apple.mDNSResponderHelper"

enum mDNSPreferencesSetNameKey
{
    kmDNSComputerName = 1,
    kmDNSLocalHostName
};

enum mDNSUpDown
{
    kmDNSUp = 1,
    kmDNSDown
};

enum mDNSAutoTunnelSetKeysReplaceDelete
{
    kmDNSAutoTunnelSetKeysReplace = 1,
    kmDNSAutoTunnelSetKeysDelete
};

// helper parses the system keychain and returns the information to mDNSResponder.
// It returns four attributes. Attributes are defined after how they show up in
// keychain access utility (the actual attribute name to retrieve these are different).
enum mDNSKeyChainAttributes
{
    kmDNSKcWhere,   // Where
    kmDNSKcAccount, // Account
    kmDNSKcKey,     // Key
    kmDNSKcName     // Name
};

#define ERROR(x, y) x,
enum mDNSHelperErrors
{
    mDNSHelperErrorBase = 2300,
    #include "helper-error.h"
    mDNSHelperErrorEnd
};
#undef ERROR

#include "mDNSEmbeddedAPI.h"
#include "helpermsg-types.h"

extern const char *mDNSHelperError(int errornum);

extern mStatus mDNSHelperInit(void);
extern void mDNSRequestBPF(void);
extern int  mDNSPowerRequest(int key, int interval);
extern int  mDNSSetLocalAddressCacheEntry(int ifindex, int family, const v6addr_t ip, const ethaddr_t eth);
extern void mDNSNotify(const char *title, const char *msg);     // Both strings are UTF-8 text
extern void mDNSPreferencesSetName(int key, domainlabel *old, domainlabel *new);
extern int  mDNSKeychainGetSecrets(CFArrayRef *secrets);
extern void mDNSConfigureServer(int updown, const char *const prefix, const domainname *const fqdn);
extern int  mDNSAutoTunnelSetKeys(int replacedelete, v6addr_t local_inner,
                                  v6addr_t local_outer, short local_port, v6addr_t remote_inner,
                                  v6addr_t remote_outer, short remote_port, const char *const prefix, const domainname *const fqdn);
extern void mDNSSendWakeupPacket(unsigned ifid, char *eth_addr, char *ip_addr, int iteration);
extern void mDNSPacketFilterControl(uint32_t command, char * ifname, uint32_t count, pfArray_t portArray, pfArray_t protocolArray);
extern void mDNSSendKeepalive(v6addr_t sadd, v6addr_t dadd, uint16_t lport, uint16_t rport, unsigned seq, unsigned ack, uint16_t win);
extern int  mDNSRetrieveTCPInfo(int family, v6addr_t laddr, uint16_t lport, v6addr_t raddr, uint16_t rport, uint32_t *seq, uint32_t *ack, uint16_t *win, int32_t *intfid);
extern void mDNSGetRemoteMAC(mDNS *const m, int family, v6addr_t raddr);
extern void mDNSStoreSPSMACAddress(int family, v6addr_t spsaddr, char *ifname);

#endif /* H_HELPER_H */
