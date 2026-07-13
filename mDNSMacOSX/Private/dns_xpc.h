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

#ifndef DNS_XPC_H
#define DNS_XPC_H

#define kDNSProxyService "com.apple.mDNSResponder.dnsproxy"

#define kDNSProxyParameters     "DNSProxyParameters"

#define kDNSInIfindex0          "InputArrayInterfaceIndex[0]"
#define kDNSInIfindex1          "InputArrayInterfaceIndex[1]"
#define kDNSInIfindex2          "InputArrayInterfaceIndex[2]"
#define kDNSInIfindex3          "InputArrayInterfaceIndex[3]"
#define kDNSInIfindex4          "InputArrayInterfaceIndex[4]"

#define kDNSOutIfindex          "OutputInterfaceIndex"

#define kDNSDaemonReply         "DaemonReplyStatusToClient"

typedef enum
{
    kDNSMsgReceived       =  0,
    kDNSDaemonEngaged 
} DaemonReplyStatusCodes;

#endif // DNS_XPC_H
