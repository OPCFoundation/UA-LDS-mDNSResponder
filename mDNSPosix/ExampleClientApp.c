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

#include <stdio.h>          // For printf()
#include <stdlib.h>         // For exit() etc.
#include <string.h>         // For strlen() etc.
#include <unistd.h>         // For select()
#include <errno.h>          // For errno, EINTR
#include <netinet/in.h>     // For INADDR_NONE
#include <arpa/inet.h>      // For inet_addr()
#include <netdb.h>          // For gethostbyname()
#include <signal.h>         // For SIGINT, etc.

#include "mDNSEmbeddedAPI.h"  // Defines the interface to the client layer above
#include "mDNSPosix.h"      // Defines the specific types needed to run mDNS on this platform

//*******************************************************************************************
// Main

static volatile mDNSBool StopNow;

mDNSlocal void HandleSIG(int signal)
{
    (void)signal;   // Unused
    debugf("%s","");
    debugf("HandleSIG");
    StopNow = mDNStrue;
}

mDNSexport void ExampleClientEventLoop(mDNS *const m)
{
    signal(SIGINT, HandleSIG);  // SIGINT is what you get for a Ctrl-C
    signal(SIGTERM, HandleSIG);

    while (!StopNow)
    {
        int nfds = 0;
        fd_set readfds;
        struct timeval timeout;
        int result;

        // 1. Set up the fd_set as usual here.
        // This example client has no file descriptors of its own,
        // but a real application would call FD_SET to add them to the set here
        FD_ZERO(&readfds);

        // 2. Set up the timeout.
        // This example client has no other work it needs to be doing,
        // so we set an effectively infinite timeout
        timeout.tv_sec = 0x3FFFFFFF;
        timeout.tv_usec = 0;

        // 3. Give the mDNSPosix layer a chance to add its information to the fd_set and timeout
        mDNSPosixGetFDSet(m, &nfds, &readfds, &timeout);

        // 4. Call select as normal
        verbosedebugf("select(%d, %d.%06d)", nfds, timeout.tv_sec, timeout.tv_usec);
        result = select(nfds, &readfds, NULL, NULL, &timeout);

        if (result < 0)
        {
            verbosedebugf("select() returned %d errno %d", result, errno);
            if (errno != EINTR) StopNow = mDNStrue;
        }
        else
        {
            // 5. Call mDNSPosixProcessFDSet to let the mDNSPosix layer do its work
            mDNSPosixProcessFDSet(m, &readfds);

            // 6. This example client has no other work it needs to be doing,
            // but a real client would do its work here
            // ... (do work) ...
        }
    }

    debugf("Exiting");
}
