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

/*
 * This file is contained in WinDDK 6001.18002
 */

#ifndef _TCPXCV_
#define _TCPXCV_

#if (!defined(UNKNOWN_PROTOCOL))
    #define UNKNOWN_PROTOCOL        0
    #define PROTOCOL_UNKNOWN_TYPE   UNKNOWN_PROTOCOL
#endif

#if (!defined(RAWTCP))
#define RAWTCP                          1
#define PROTOCOL_RAWTCP_TYPE            RAWTCP
#endif

#if (!defined(LPR))
#define LPR                             2
#define PROTOCOL_LPR_TYPE               LPR
#endif

#define MAX_PORTNAME_LEN                63 +1       // port name length
#define MAX_NETWORKNAME_LEN             48 +1       // host name length
#define MAX_NETWORKNAME2_LEN            128         // host name or IPv6 address
#define MAX_SNMP_COMMUNITY_STR_LEN      32 +1       // SNMP Community String Name
#define MAX_QUEUENAME_LEN               32 +1       // lpr print que name
#define MAX_IPADDR_STR_LEN              15 +1       // ip address; string version
#define MAX_ADDRESS_STR_LEN             12 +1       // hw address length
#define MAX_DEVICEDESCRIPTION_STR_LEN   256+1



typedef struct _PORT_DATA_1
{
    WCHAR sztPortName[MAX_PORTNAME_LEN];
    DWORD dwVersion;
    DWORD dwProtocol;
    DWORD cbSize;
    DWORD dwReserved;
    WCHAR sztHostAddress[MAX_NETWORKNAME_LEN];
    WCHAR sztSNMPCommunity[MAX_SNMP_COMMUNITY_STR_LEN];
    DWORD dwDoubleSpool;
    WCHAR sztQueue[MAX_QUEUENAME_LEN];
    WCHAR sztIPAddress[MAX_IPADDR_STR_LEN];
    BYTE Reserved[540];
    DWORD dwPortNumber;
    DWORD dwSNMPEnabled;
    DWORD dwSNMPDevIndex;
}   PORT_DATA_1, *PPORT_DATA_1;

typedef struct _PORT_DATA_2
{
    WCHAR sztPortName[MAX_PORTNAME_LEN];
    DWORD dwVersion;
    DWORD dwProtocol;
    DWORD cbSize;
    DWORD dwReserved;
    WCHAR sztHostAddress [MAX_NETWORKNAME2_LEN];
    WCHAR sztSNMPCommunity[MAX_SNMP_COMMUNITY_STR_LEN];
    DWORD dwDoubleSpool;
    WCHAR sztQueue[MAX_QUEUENAME_LEN];
    BYTE Reserved[514];
    DWORD dwPortNumber;
    DWORD dwSNMPEnabled;
    DWORD dwSNMPDevIndex;
    DWORD dwPortMonitorMibIndex;
}   PORT_DATA_2, *PPORT_DATA_2;


typedef struct _PORT_DATA_LIST_1
{
    DWORD dwVersion;
    DWORD cPortData;
    PORT_DATA_2 pPortData[1];
}   PORT_DATA_LIST_1, *PPORT_DATA_LIST_1;


typedef struct _DELETE_PORT_DATA_1
{
    WCHAR psztPortName[MAX_PORTNAME_LEN];
    BYTE Reserved[98];
    DWORD dwVersion;
    DWORD dwReserved;
}   DELETE_PORT_DATA_1, *PDELETE_PORT_DATA_1;


typedef struct _CONFIG_INFO_DATA_1
{
    BYTE Reserved[128];
    DWORD dwVersion;
}   CONFIG_INFO_DATA_1, *PCONFIG_INFO_DATA_1;



#endif
