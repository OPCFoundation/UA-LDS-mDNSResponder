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

#include <net/if.h>
#include <System/net/pfvar.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <AssertMacros.h>
#include "P2PPacketFilter.h"

#define AIRDROP_ANCHOR_PATH "com.apple/200.AirDrop"
#define MDNS_ANCHOR_NAME    "Bonjour"
#define MDNS_ANCHOR_PATH    AIRDROP_ANCHOR_PATH "/" MDNS_ANCHOR_NAME

#define PF_DEV_PATH "/dev/pf"
#define BONJOUR_PORT 5353

static int openPFDevice( int * outFD )
{
    int err;
    int fd = open( PF_DEV_PATH, O_RDWR );

    if( fd >= 0 )
    {
        err = 0;
        *outFD = fd;
    }
    else
    {
        err = errno;
    }

    return err;
}

static int getTicket( int devFD, u_int32_t * outTicket, char * anchorPath )
{
    struct pfioc_trans_e trans_e;

    trans_e.rs_num = PF_RULESET_FILTER;
    strlcpy( trans_e.anchor, anchorPath, sizeof( trans_e.anchor ) );

    struct pfioc_trans trans;

    trans.size = 1;
    trans.esize = sizeof( trans_e );
    trans.array = &trans_e;

    int result, ioctlError;

    ioctlError = ioctl( devFD, DIOCXBEGIN, &trans );
    if( ioctlError )
    {
        result = errno;
    }
    else
    {
        result = 0;
        *outTicket = trans_e.ticket;
    }

    return result;
}

static int commitChange( int devFD, u_int32_t ticket, char * anchorPath )
{
    struct pfioc_trans_e trans_e;

    trans_e.rs_num = PF_RULESET_FILTER;
    strlcpy( trans_e.anchor, anchorPath, sizeof( trans_e.anchor ) );
    trans_e.ticket = ticket;

    struct pfioc_trans trans;

    trans.size = 1;
    trans.esize = sizeof( trans_e );
    trans.array = &trans_e;

    int result, ioctlError;

    ioctlError = ioctl( devFD, DIOCXCOMMIT, &trans );
    if( ioctlError )
        result = errno;
    else
        result = 0;

    return result;
}

static int getPoolTicket( int devFD, u_int32_t * outPoolTicket )
{
    struct pfioc_pooladdr pp;

    int result, ioctlError;

    ioctlError = ioctl( devFD, DIOCBEGINADDRS, &pp );
    if( ioctlError )
    {
        result = errno;
    }
    else
    {
        result = 0;
        *outPoolTicket = pp.ticket;
    }

    return result;
}

static int addRule( int devFD, struct pfioc_rule * pr )
{
    int result, ioctlResult;

    ioctlResult = ioctl( devFD, DIOCADDRULE, pr );
    if( ioctlResult )
        result = errno;
    else
        result = 0;

    return result;
}

static void initRuleHeader( struct pfioc_rule * pr,
                            u_int32_t ticket,
                            u_int32_t poolTicket,
                            char * anchorPath )
{
    pr->action = PF_CHANGE_NONE;
    pr->ticket = ticket;
    pr->pool_ticket = poolTicket;
    strlcpy( pr->anchor, anchorPath, sizeof( pr->anchor ) );
}

// allow inbound traffice on the Bonjour port (5353)
static void initBonjourRule( struct pfioc_rule * pr,
                             const char * interfaceName,
                             u_int32_t ticket,
                             u_int32_t poolTicket,
                             char * anchorPath )
{
    memset( pr, 0, sizeof( *pr ) );

    // Header
    initRuleHeader( pr, ticket, poolTicket, anchorPath );

    // Rule
    pr->rule.dst.xport.range.port[0] = htons( BONJOUR_PORT );
    pr->rule.dst.xport.range.op = PF_OP_EQ;

    strlcpy( pr->rule.ifname, interfaceName, sizeof( pr->rule.ifname ) );

    pr->rule.action = PF_PASS;
    pr->rule.direction = PF_IN;
    pr->rule.keep_state = 1;
    pr->rule.af = AF_INET6;
    pr->rule.proto = IPPROTO_UDP;
    pr->rule.extfilter = PF_EXTFILTER_APD;
}

// allow outbound TCP connections and return traffic for those connections
static void initOutboundTCPRule( struct pfioc_rule * pr,
                                 const char * interfaceName,
                                 u_int32_t ticket,
                                 u_int32_t poolTicket,
                                 char * anchorPath )
{
    memset( pr, 0, sizeof( *pr ) );

    // Header
    initRuleHeader( pr, ticket, poolTicket, anchorPath );

    // Rule
    strlcpy( pr->rule.ifname, interfaceName, sizeof( pr->rule.ifname ) );

    pr->rule.action = PF_PASS;
    pr->rule.direction = PF_OUT;
    pr->rule.keep_state = 1;
    pr->rule.proto = IPPROTO_TCP;
}

// allow inbound traffic on the specified port and protocol
static void initPortRule( struct pfioc_rule * pr,
                          const char * interfaceName,
                          u_int32_t ticket,
                          u_int32_t poolTicket,
                          char * anchorPath,
                          u_int16_t port,
                          u_int16_t protocol )
{
    memset( pr, 0, sizeof( *pr ) );

    // Header
    initRuleHeader( pr, ticket, poolTicket, anchorPath );

    // Rule
    // mDNSResponder passes the port in Network Byte Order, so htons(port) is not required
    pr->rule.dst.xport.range.port[0] = port;
    pr->rule.dst.xport.range.op = PF_OP_EQ;

    strlcpy( pr->rule.ifname, interfaceName, sizeof( pr->rule.ifname ) );

    pr->rule.action = PF_PASS;
    pr->rule.direction = PF_IN;
    pr->rule.keep_state = 1;
    pr->rule.af = AF_INET6;
    pr->rule.proto = protocol;
    pr->rule.extfilter = PF_EXTFILTER_APD;
}

// allow inbound traffic on the Bonjour port (5353) and the specified port and protocol sets
int P2PPacketFilterAddBonjourRuleSet(const char * interfaceName, u_int32_t count, pfArray_t portArray, pfArray_t protocolArray )
{
    int result;
    u_int32_t i, ticket, poolTicket;
    int devFD = -1;
    char * anchorPath = MDNS_ANCHOR_PATH;

    result = openPFDevice( &devFD );
    require( result == 0, exit );

    result = getTicket( devFD, &ticket, anchorPath );
    require( result == 0, exit );

    result = getPoolTicket( devFD, &poolTicket );
    require( result == 0, exit );

    struct pfioc_rule pr;

    // allow inbound Bonjour traffice to port 5353
    initBonjourRule( &pr, interfaceName, ticket, poolTicket, anchorPath);

    result = addRule( devFD, &pr );
    require( result == 0, exit );

    // open inbound port for each service
    for (i = 0; i < count; i++) {
        initPortRule( &pr, interfaceName, ticket, poolTicket, anchorPath, portArray[i], protocolArray[i] );
        result = addRule( devFD, &pr );
        require( result == 0, exit );
    }

    // allow outbound TCP connections and return traffic for those connections
    initOutboundTCPRule( &pr, interfaceName, ticket, poolTicket, anchorPath);

    result = addRule( devFD, &pr );
    require( result == 0, exit );

    result = commitChange( devFD, ticket, anchorPath );
    require( result == 0, exit );

exit:

    if( devFD >= 0 )
        close( devFD );

    return result;
}

int P2PPacketFilterClearBonjourRules()
{
    int result;
    int pfDev = -1;
    u_int32_t ticket;
    char * anchorPath = MDNS_ANCHOR_PATH;

    result = openPFDevice( &pfDev );
    require( result == 0, exit );

    result = getTicket( pfDev, &ticket, anchorPath );
    require( result == 0, exit );

    result = commitChange( pfDev, ticket, anchorPath );

exit:

    if( pfDev >= 0 )
        close( pfDev );

    return result;
}
