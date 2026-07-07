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

#include "ConfigurationAuthority.h"
#include "ConfigurationRights.h"

#include <AssertMacros.h>


static AuthorizationRef gAuthRef = 0;

static AuthorizationItem gAuthorizations[] = {   { UPDATE_SC_RIGHT, 0, NULL, 0 },
                                                 { EDIT_SYS_KEYCHAIN_RIGHT, 0, NULL, 0 }};
static AuthorizationRights gAuthSet = { sizeof gAuthorizations / sizeof gAuthorizations[0], gAuthorizations };

static CFDictionaryRef  CreateRightsDict( CFStringRef prompt)
/* Create a CFDictionary decribing an auth right. See /etc/authorization for examples. */
/* Specifies that the right requires admin authentication, which persists for 5 minutes. */
{
    CFMutableDictionaryRef dict = NULL, tmpDict;
    CFMutableArrayRef mechanisms;
    CFNumberRef timeout;
    int val;

    tmpDict = CFDictionaryCreateMutable( (CFAllocatorRef) NULL, 0, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    require( tmpDict != NULL, MakeDictFailed);

    CFDictionaryAddValue(tmpDict, CFSTR("class"), CFSTR("user"));
    CFDictionaryAddValue(tmpDict, CFSTR("comment"), prompt);
    CFDictionaryAddValue(tmpDict, CFSTR("group"), CFSTR("admin"));

    mechanisms = CFArrayCreateMutable((CFAllocatorRef) NULL, 1, &kCFTypeArrayCallBacks);
    require( mechanisms != NULL, MakeArrayFailed);
    CFArrayAppendValue( mechanisms, CFSTR("builtin:authenticate"));
    CFDictionaryAddValue( tmpDict, CFSTR("mechanisms"), mechanisms);

    val = 300;  // seconds
    timeout = CFNumberCreate((CFAllocatorRef) NULL, kCFNumberIntType, &val);
    require( timeout != NULL, MakeIntFailed);
    CFDictionaryAddValue( tmpDict, CFSTR("timeout"), timeout);
    CFDictionaryAddValue( tmpDict, CFSTR("shared"), kCFBooleanTrue);

    dict = tmpDict;
    tmpDict = NULL;

    CFRelease( timeout);
MakeIntFailed:
    CFRelease( mechanisms);
MakeArrayFailed:
    if ( tmpDict)
        CFRelease( tmpDict);
MakeDictFailed:
    return dict;
}

OSStatus InitConfigAuthority(void)
/* Initialize the authorization record-keeping */
{
    OSStatus err;
    CFDictionaryRef dict;
    CFStringRef rightInfo;

    err = AuthorizationCreate((AuthorizationRights*) NULL, (AuthorizationEnvironment*) NULL,
                              (AuthorizationFlags) 0, &gAuthRef);
    require_noerr( err, NewAuthFailed);

    err = AuthorizationRightGet( UPDATE_SC_RIGHT, (CFDictionaryRef*) NULL);
    if (err == errAuthorizationDenied)
    {
        rightInfo = CFCopyLocalizedString(CFSTR("Authentication required to set Dynamic DNS preferences."),
                                          CFSTR("Describes operation that requires user authorization"));
        require_action( rightInfo != NULL, GetStrFailed, err=coreFoundationUnknownErr;);
        dict = CreateRightsDict(rightInfo);
        require_action( dict != NULL, GetStrFailed, err=coreFoundationUnknownErr;);

        err = AuthorizationRightSet(gAuthRef, UPDATE_SC_RIGHT, dict, (CFStringRef) NULL,
                                    (CFBundleRef) NULL, (CFStringRef) NULL);
        CFRelease(rightInfo);
        CFRelease(dict);
    }
    require_noerr( err, AuthSetFailed);

    err = AuthorizationRightGet( EDIT_SYS_KEYCHAIN_RIGHT, (CFDictionaryRef*) NULL);
    if (err == errAuthorizationDenied)
    {
        rightInfo = CFCopyLocalizedString( CFSTR("Authentication required to edit System Keychain."),
                                           CFSTR("Describes operation that requires user authorization"));
        require_action( rightInfo != NULL, GetStrFailed, err=coreFoundationUnknownErr;);
        dict = CreateRightsDict( rightInfo);
        require_action( dict != NULL, GetStrFailed, err=coreFoundationUnknownErr;);

        err = AuthorizationRightSet(gAuthRef, EDIT_SYS_KEYCHAIN_RIGHT, dict, (CFStringRef) NULL,
                                    (CFBundleRef) NULL, (CFStringRef) NULL);
        CFRelease( rightInfo);
        CFRelease( dict);
    }
    require_noerr( err, AuthSetFailed);

AuthSetFailed:
GetStrFailed:
NewAuthFailed:
    return err;
}

OSStatus    AttemptAcquireAuthority( Boolean allowUI)
/* Try to get permission for privileged ops, either implicitly or by asking the user for */
/* authority to perform operations (if necessary) */
{
    AuthorizationFlags allowFlag = allowUI ? kAuthorizationFlagInteractionAllowed : 0;
    OSStatus err;

    err = AuthorizationCopyRights( gAuthRef, &gAuthSet, (AuthorizationEnvironment*) NULL,
                                   kAuthorizationFlagExtendRights | kAuthorizationFlagPreAuthorize |
                                   allowFlag,
                                   (AuthorizationRights**) NULL);
    return err;
}

OSStatus ReleaseAuthority(void)
/* Discard authority to perform operations */
{
    (void) AuthorizationFree( gAuthRef, kAuthorizationFlagDefaults);
    gAuthRef = 0;
    return AuthorizationCreate( (AuthorizationRights*) NULL, (AuthorizationEnvironment*) NULL,
                                (AuthorizationFlags) 0, &gAuthRef);
}

Boolean CurrentlyAuthorized(void)
{
    OSStatus err = AttemptAcquireAuthority(true);
    return err == noErr;
}


OSStatus ExternalizeAuthority(AuthorizationExternalForm *pAuth)
/* Package up current authorizations for transfer to another process */
{
    return AuthorizationMakeExternalForm(gAuthRef, pAuth);
}
