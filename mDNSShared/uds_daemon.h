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

#include "mDNSEmbeddedAPI.h"
#include "dnssd_ipc.h"

/* Client interface: */

#define SRS_PORT(S) mDNSVal16((S)->RR_SRV.resrec.rdata->u.srv.port)

extern int udsserver_init(dnssd_sock_t skts[], mDNSu32 count);
extern mDNSs32 udsserver_idle(mDNSs32 nextevent);
extern void udsserver_info(mDNS *const m);  // print out info about current state
extern void udsserver_handle_configchange(mDNS *const m);
extern int udsserver_exit(void);    // should be called prior to app exit
extern void LogMcastStateInfo(mDNS *const m, mDNSBool mflag, mDNSBool start, mDNSBool mstatelog);
#define LogMcastQ       (mDNS_McastLoggingEnabled == 0) ? ((void)0) : LogMcastQuestion
#define LogMcastS       (mDNS_McastLoggingEnabled == 0) ? ((void)0) : LogMcastService
#define LogMcast        (mDNS_McastLoggingEnabled == 0) ? ((void)0) : LogMsg
#define LogMcastNoIdent (mDNS_McastLoggingEnabled == 0) ? ((void)0) : LogMsgNoIdent

/* Routines that uds_daemon expects to link against: */

typedef void (*udsEventCallback)(int fd, short filter, void *context);
extern mStatus udsSupportAddFDToEventLoop(dnssd_sock_t fd, udsEventCallback callback, void *context, void **platform_data);
extern int     udsSupportReadFD(dnssd_sock_t fd, char* buf, int len, int flags, void *platform_data);
extern mStatus udsSupportRemoveFDFromEventLoop(dnssd_sock_t fd, void *platform_data); // Note: This also CLOSES the file descriptor as well

extern void RecordUpdatedNiceLabel(mDNS *const m, mDNSs32 delay);

// Globals and functions defined in uds_daemon.c and also shared with the old "daemon.c" on OS X

extern mDNS mDNSStorage;
extern DNameListElem *AutoRegistrationDomains;
extern DNameListElem *AutoBrowseDomains;

extern mDNSs32 ChopSubTypes(char *regtype, char **AnonData);
extern AuthRecord *AllocateSubTypes(mDNSs32 NumSubTypes, char *p, char **AnonData);
extern int CountExistingRegistrations(domainname *srv, mDNSIPPort port);
extern void FreeExtraRR(mDNS *const m, AuthRecord *const rr, mStatus result);
extern int CountPeerRegistrations(mDNS *const m, ServiceRecordSet *const srs);

#if APPLE_OSX_mDNSResponder

extern void machserver_automatic_browse_domain_changed(const domainname *d, mDNSBool add);
extern void machserver_automatic_registration_domain_changed(const domainname *d, mDNSBool add);
// D2D interface support
extern void external_start_browsing_for_service(mDNSInterfaceID InterfaceID, const domainname *const type, DNS_TypeValues qtype, DNSServiceFlags flags);
extern void external_stop_browsing_for_service(mDNSInterfaceID InterfaceID, const domainname *const type, DNS_TypeValues qtype, DNSServiceFlags flags);
extern void external_start_advertising_service(const ResourceRecord *const resourceRecord, DNSServiceFlags flags);
extern void external_stop_advertising_service(const ResourceRecord *const resourceRecord, DNSServiceFlags flags);
extern void external_start_resolving_service(mDNSInterfaceID InterfaceID, const domainname *const fqdn, DNSServiceFlags flags);
extern void external_stop_resolving_service(mDNSInterfaceID InterfaceID, const domainname *const fqdn, DNSServiceFlags flags);
extern void external_connection_release(const domainname *instance);

#else   // APPLE_OSX_mDNSResponder

#define external_start_browsing_for_service(A,B,C,D) (void)(A)
#define external_stop_browsing_for_service(A,B,C,D)  (void)(A)
#define external_start_advertising_service(A,B)      (void)(A)
#define external_stop_advertising_service(A,B)       (void)(A)
#define external_start_resolving_service(A,B,C)      (void)(A)
#define external_stop_resolving_service(A,B,C)       (void)(A)
#define external_connection_release(A)               (void)(A)

#endif // APPLE_OSX_mDNSResponder

extern const char mDNSResponderVersionString_SCCS[];
#define mDNSResponderVersionString (mDNSResponderVersionString_SCCS+5)
