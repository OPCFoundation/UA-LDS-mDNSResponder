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

#ifndef _dnsextd_h
#define _dnsextd_h


#include <mDNSEmbeddedAPI.h>
#include <DNSCommon.h>
#include <GenLinkedList.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define LLQ_TABLESIZE   1024    // !!!KRS make this dynamically growable


typedef enum DNSZoneSpecType
{
    kDNSZonePublic,
    kDNSZonePrivate
} DNSZoneSpecType;


typedef struct DNSZone
{
    domainname name;
    DNSZoneSpecType type;
    DomainAuthInfo      *   updateKeys; // linked list of keys for signing deletion updates
    DomainAuthInfo      *   queryKeys;  // linked list of keys for queries
    struct DNSZone      *   next;
} DNSZone;


typedef struct
{
    struct sockaddr_in src;
    size_t len;
    DNSZone * zone;
    mDNSBool isZonePublic;
    DNSMessage msg;
    // Note: extra storage for oversized (TCP) messages goes here
} PktMsg;

// lease table entry
typedef struct RRTableElem
{
    struct RRTableElem *next;
    struct sockaddr_in cli;   // client's source address
    long expire;              // expiration time, in seconds since epoch
    domainname zone;          // from zone field of update message
    domainname name;          // name of the record
    CacheRecord rr;           // last field in struct allows for allocation of oversized RRs
} RRTableElem;

typedef enum
{
    RequestReceived = 0,
    ChallengeSent   = 1,
    Established     = 2
} LLQState;

typedef struct AnswerListElem
{
    struct AnswerListElem *next;
    domainname name;
    mDNSu16 type;
    CacheRecord *KnownAnswers;  // All valid answers delivered to client
    CacheRecord *EventList;     // New answers (adds/removes) to be sent to client
    int refcount;
    mDNSBool UseTCP;            // Use TCP if UDP would cause truncation
    pthread_t tid;              // Allow parallel list updates
} AnswerListElem;

// llq table entry
typedef struct LLQEntry
{
    struct LLQEntry *next;
    struct sockaddr_in cli;   // clien'ts source address
    domainname qname;
    mDNSu16 qtype;
    mDNSOpaque64 id;
    LLQState state;
    mDNSu32 lease;            // original lease, in seconds
    mDNSs32 expire;           // expiration, absolute, in seconds since epoch
    AnswerListElem *AnswerList;
} LLQEntry;


typedef void (*EventCallback)( void * context );

typedef struct EventSource
{
    EventCallback callback;
    void                *   context;
    TCPSocket *         sock;
    int fd;
    mDNSBool markedForDeletion;
    struct  EventSource *   next;
} EventSource;


// daemon-wide information
typedef struct
{
    // server variables - read only after initialization (no locking)
    struct sockaddr_in addr;            // the address we will bind to
    struct sockaddr_in llq_addr;        // the address we will receive llq requests on.
    struct sockaddr_in ns_addr;         // the real ns server address
    int tcpsd;                          // listening TCP socket for dns requests
    int udpsd;                          // listening UDP socket for dns requests
    int tlssd;                          // listening TCP socket for private browsing
    int llq_tcpsd;                      // listening TCP socket for llq service
    int llq_udpsd;                      // listening UDP socket for llq service
    DNameListElem   *   public_names;   // list of public SRV names
    DNSZone         *   zones;

    // daemon variables - read only after initialization (no locking)
    mDNSIPPort private_port;           // listening port for private messages
    mDNSIPPort llq_port;           // listening port for llq

    // lease table variables (locked via mutex after initialization)
    RRTableElem **table;       // hashtable for records with leases
    pthread_mutex_t tablelock; // mutex for lease table
    mDNSs32 nbuckets;          // buckets allocated
    mDNSs32 nelems;            // elements in table

    // LLQ table variables
    LLQEntry *LLQTable[LLQ_TABLESIZE];  // !!!KRS change this and RRTable to use a common data structure
    AnswerListElem *AnswerTable[LLQ_TABLESIZE];
    int AnswerTableCount;
    int LLQEventNotifySock;          // Unix domain socket pair - update handling thread writes to EventNotifySock, which wakes
    int LLQEventListenSock;          // the main thread listening on EventListenSock, indicating that the zone has changed

    GenLinkedList eventSources;     // linked list of EventSource's
} DaemonInfo;


int
ParseConfig
(
    DaemonInfo  *   d,
    const char  *   file
);


#endif
