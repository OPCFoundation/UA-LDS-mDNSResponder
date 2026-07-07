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

#include <Resources.h>
#include <CodeFragments.h>
#include "ShowInitIcon.h"

extern pascal OSErr FragRegisterFileLibs(ConstFSSpecPtr fss, Boolean unregister);

extern void main(void)
{
    OSStatus err;
    FCBPBRec fcbPB;
    FSSpec fss;

    // 1. Show our "icon march" icon
    ShowInitIcon(128, true);

    // 2. Find our FSSpec
    fss.name[0] = 0;
    fcbPB.ioNamePtr = fss.name;
    fcbPB.ioVRefNum = 0;
    fcbPB.ioRefNum = (short)CurResFile();
    fcbPB.ioFCBIndx = 0;
    err = PBGetFCBInfoSync(&fcbPB);

    // 3. Tell CFM that we're a CFM library container file
    fss.vRefNum = fcbPB.ioFCBVRefNum;
    fss.parID = fcbPB.ioFCBParID;
    if (err == noErr) err = FragRegisterFileLibs(&fss, false);

    // 4. Now that CFM knows we're a library container, tell it to go and get our library
    if (err == noErr)
    {
        CFragConnectionID c;
        Ptr m;
        Str255 e;
        THz oldZone = GetZone();
        SetZone(SystemZone());
        err = GetSharedLibrary("\pDarwin;mDNS", kPowerPCCFragArch, kLoadCFrag, &c, &m, e);
        SetZone(oldZone);
    }
}

// There's no CFM stub library for the FragRegisterFileLibs() call, so we'll make our own
#if __ide_target("FragRegisterFileLibsStub")
#pragma export on
pascal OSErr FragRegisterFileLibs(ConstFSSpecPtr fss, Boolean unregister)
{
    (void)fss;          // Unused
    (void)unregister;   // Unused
    return(0);
}
#endif
