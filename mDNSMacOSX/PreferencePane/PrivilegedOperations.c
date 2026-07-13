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

#include "PrivilegedOperations.h"
#include "ConfigurationAuthority.h"
#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <AssertMacros.h>
#include <Security/Security.h>

Boolean gToolApproved = false;

static pid_t    execTool(const char *args[])
// fork/exec and return new pid
{
    pid_t child;

    child = vfork();
    if (child == 0)
    {
        execv(args[0], (char *const *)args);
        printf("exec of %s failed; errno = %d\n", args[0], errno);
        _exit(-1);      // exec failed
    }
    else
        return child;
}

OSStatus EnsureToolInstalled(void)
// Make sure that the tool is installed in the right place, with the right privs, and the right version.
{
    CFURLRef bundleURL;
    pid_t toolPID;
    int status;
    OSStatus err = noErr;
    const char      *args[] = { kToolPath, "0", "V", NULL };
    char toolSourcePath[PATH_MAX] = {};
    char toolInstallerPath[PATH_MAX] = {};

    if (gToolApproved)
        return noErr;

    // Check version of installed tool
    toolPID = execTool(args);
    if (toolPID > 0)
    {
        waitpid(toolPID, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == PRIV_OP_TOOL_VERS)
            return noErr;
    }

    // Locate our in-bundle copy of privop tool
    bundleURL = CFBundleCopyBundleURL(CFBundleGetBundleWithIdentifier(CFSTR("com.apple.preference.bonjour")) );
    if (bundleURL != NULL)
    {
        CFURLGetFileSystemRepresentation(bundleURL, false, (UInt8*) toolSourcePath, sizeof toolSourcePath);
        if (strlcat(toolSourcePath,    "/Contents/Resources/" kToolName,      sizeof toolSourcePath   ) >= sizeof toolSourcePath   ) return(-1);
        CFURLGetFileSystemRepresentation(bundleURL, false, (UInt8*) toolInstallerPath, sizeof toolInstallerPath);
        if (strlcat(toolInstallerPath, "/Contents/Resources/" kToolInstaller, sizeof toolInstallerPath) >= sizeof toolInstallerPath) return(-1);
    }
    else
        return coreFoundationUnknownErr;

    // Obtain authorization and run in-bundle copy as root to install it
    {
        AuthorizationItem aewpRight = { kAuthorizationRightExecute, strlen(toolInstallerPath), toolInstallerPath, 0 };
        AuthorizationItemSet rights = { 1, &aewpRight };
        AuthorizationRef authRef;

        err = AuthorizationCreate(&rights, (AuthorizationEnvironment*) NULL,
                                  kAuthorizationFlagInteractionAllowed | kAuthorizationFlagExtendRights |
                                  kAuthorizationFlagPreAuthorize, &authRef);
        if (err == noErr)
        {
            char *installerargs[] = { toolSourcePath, NULL };
            err = AuthorizationExecuteWithPrivileges(authRef, toolInstallerPath, 0, installerargs, (FILE**) NULL);
            if (err == noErr) {
                int pid = wait(&status);
                if (pid > 0 && WIFEXITED(status)) {
                    err = WEXITSTATUS(status);
                    if (err == noErr) {
                        gToolApproved = true;
                    }
                } else {
                    err = -1;
                }
            }
            (void) AuthorizationFree(authRef, kAuthorizationFlagDefaults);
        }
    }

    return err;
}


static OSStatus ExecWithCmdAndParam(const char *subCmd, CFDataRef paramData)
// Execute our privop tool with the supplied subCmd and parameter
{
    OSStatus err = noErr;
    int commFD, dataLen;
    u_int32_t len;
    pid_t child;
    char fileNum[16];
    UInt8                   *buff;
    const char              *args[] = { kToolPath, NULL, "A", NULL, NULL };
    AuthorizationExternalForm authExt;

    err = ExternalizeAuthority(&authExt);
    require_noerr(err, AuthFailed);

    dataLen = CFDataGetLength(paramData);
    buff = (UInt8*) malloc(dataLen * sizeof(UInt8));
    require_action(buff != NULL, AllocBuffFailed, err=memFullErr;);
    {
        CFRange all = { 0, dataLen };
        CFDataGetBytes(paramData, all, buff);
    }

    commFD = fileno(tmpfile());
    sprintf(fileNum, "%d", commFD);
    args[1] = fileNum;
    args[3] = subCmd;

    // write authority to pipe
    len = 0;    // tag, unused
    write(commFD, &len, sizeof len);
    len = sizeof authExt;   // len
    write(commFD, &len, sizeof len);
    write(commFD, &authExt, len);

    // write parameter to pipe
    len = 0;    // tag, unused
    write(commFD, &len, sizeof len);
    len = dataLen;  // len
    write(commFD, &len, sizeof len);
    write(commFD, buff, len);

    child = execTool(args);
    if (child > 0) {
        int status;
        waitpid(child, &status, 0);
        if (WIFEXITED(status))
            err = WEXITSTATUS(status);
        //fprintf(stderr, "child exited; status = %d (%ld)\n", status, err);
    }

    close(commFD);

    free(buff);
AllocBuffFailed:
AuthFailed:
    return err;
}

OSStatus
WriteBrowseDomain(CFDataRef domainArrayData)
{
    if (!CurrentlyAuthorized())
        return authFailErr;
    return ExecWithCmdAndParam("Wb", domainArrayData);
}

OSStatus
WriteRegistrationDomain(CFDataRef domainArrayData)
{
    if (!CurrentlyAuthorized())
        return authFailErr;
    return ExecWithCmdAndParam("Wd", domainArrayData);
}

OSStatus
WriteHostname(CFDataRef domainArrayData)
{
    if (!CurrentlyAuthorized())
        return authFailErr;
    return ExecWithCmdAndParam("Wh", domainArrayData);
}

OSStatus
SetKeyForDomain(CFDataRef secretData)
{
    if (!CurrentlyAuthorized())
        return authFailErr;
    return ExecWithCmdAndParam("Wk", secretData);
}
