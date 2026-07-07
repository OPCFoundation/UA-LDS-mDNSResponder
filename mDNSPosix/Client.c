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

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "mDNSEmbeddedAPI.h" // Defines the interface to the mDNS core code
#include "mDNSPosix.h"    // Defines the specific types needed to run mDNS on this platform
#include "ExampleClientApp.h"

// Globals
static mDNS mDNSStorage;       // mDNS core uses this to store its globals
static mDNS_PlatformSupport PlatformStorage;  // Stores this platform's globals
#define RR_CACHE_SIZE 500
static CacheEntity gRRCache[RR_CACHE_SIZE];

mDNSexport const char ProgramName[] = "mDNSClientPosix";

static const char *gProgramName = ProgramName;

static void BrowseCallback(mDNS *const m, DNSQuestion *question, const ResourceRecord *const answer, QC_result AddRecord)
// A callback from the core mDNS code that indicates that we've received a
// response to our query.  Note that this code runs on the main thread
// (in fact, there is only one thread!), so we can safely printf the results.
{
    domainlabel name;
    domainname type;
    domainname domain;
    char nameC  [MAX_DOMAIN_LABEL+1];           // Unescaped name: up to 63 bytes plus C-string terminating NULL.
    char typeC  [MAX_ESCAPED_DOMAIN_NAME];
    char domainC[MAX_ESCAPED_DOMAIN_NAME];
    const char *state;

    (void)m;        // Unused
    (void)question; // Unused

    assert(answer->rrtype == kDNSType_PTR);

    DeconstructServiceName(&answer->rdata->u.name, &name, &type, &domain);

    ConvertDomainLabelToCString_unescaped(&name, nameC);
    ConvertDomainNameToCString(&type, typeC);
    ConvertDomainNameToCString(&domain, domainC);

    // If the TTL has hit 0, the service is no longer available.
    if (!AddRecord) {
        state = "Lost ";
    } else {
        state = "Found";
    }
    fprintf(stderr, "*** %s name = '%s', type = '%s', domain = '%s'\n", state, nameC, typeC, domainC);
}

static mDNSBool CheckThatServiceTypeIsUsable(const char *serviceType, mDNSBool printExplanation)
// Checks that serviceType is a reasonable service type
// label and, if it isn't and printExplanation is true, prints
// an explanation of why not.
{
    mDNSBool result;

    result = mDNStrue;
    if (result && strlen(serviceType) > 63) {
        if (printExplanation) {
            fprintf(stderr,
                    "%s: Service type specified by -t is too long (must be 63 characters or less)\n",
                    gProgramName);
        }
        result = mDNSfalse;
    }
    if (result && serviceType[0] == 0) {
        if (printExplanation) {
            fprintf(stderr,
                    "%s: Service type specified by -t can't be empty\n",
                    gProgramName);
        }
        result = mDNSfalse;
    }
    return result;
}

static const char kDefaultServiceType[] = "_afpovertcp._tcp";
static const char kDefaultDomain[]      = "local.";

static void PrintUsage()
{
    fprintf(stderr,
            "Usage: %s [-v level] [-t type] [-d domain]\n",
            gProgramName);
    fprintf(stderr, "          -v verbose mode, level is a number from 0 to 2\n");
    fprintf(stderr, "             0 = no debugging info (default)\n");
    fprintf(stderr, "             1 = standard debugging info\n");
    fprintf(stderr, "             2 = intense debugging info\n");
    fprintf(stderr, "          -t uses 'type' as the service type (default is '%s')\n", kDefaultServiceType);
    fprintf(stderr, "          -d uses 'domain' as the domain to browse (default is '%s')\n", kDefaultDomain);
}

static const char *gServiceType      = kDefaultServiceType;
static const char *gServiceDomain    = kDefaultDomain;

static void ParseArguments(int argc, char **argv)
// Parses our command line arguments into the global variables
// listed above.
{
    int ch;

    // Set gProgramName to the last path component of argv[0]

    gProgramName = strrchr(argv[0], '/');
    if (gProgramName == NULL) {
        gProgramName = argv[0];
    } else {
        gProgramName += 1;
    }

    // Parse command line options using getopt.

    do {
        ch = getopt(argc, argv, "v:t:d:");
        if (ch != -1) {
            switch (ch) {
            case 'v':
                gMDNSPlatformPosixVerboseLevel = atoi(optarg);
                if (gMDNSPlatformPosixVerboseLevel < 0 || gMDNSPlatformPosixVerboseLevel > 2) {
                    fprintf(stderr,
                            "%s: Verbose mode must be in the range 0..2\n",
                            gProgramName);
                    exit(1);
                }
                break;
            case 't':
                gServiceType = optarg;
                if ( !CheckThatServiceTypeIsUsable(gServiceType, mDNStrue) ) {
                    exit(1);
                }
                break;
            case 'd':
                gServiceDomain = optarg;
                break;
            case '?':
            default:
                PrintUsage();
                exit(1);
                break;
            }
        }
    } while (ch != -1);

    // Check for any left over command line arguments.

    if (optind != argc) {
        fprintf(stderr, "%s: Unexpected argument '%s'\n", gProgramName, argv[optind]);
        exit(1);
    }
}

int main(int argc, char **argv)
// The program's main entry point.  The program does a trivial
// mDNS query, looking for all AFP servers in the local domain.
{
    int result;
    mStatus status;
    DNSQuestion question;
    domainname type;
    domainname domain;

    // Parse our command line arguments.  This won't come back if there's an error.
    ParseArguments(argc, argv);

    // Initialise the mDNS core.
    status = mDNS_Init(&mDNSStorage, &PlatformStorage,
                       gRRCache, RR_CACHE_SIZE,
                       mDNS_Init_DontAdvertiseLocalAddresses,
                       mDNS_Init_NoInitCallback, mDNS_Init_NoInitCallbackContext);
    if (status == mStatus_NoError) {

        // Construct and start the query.

        MakeDomainNameFromDNSNameString(&type, gServiceType);
        MakeDomainNameFromDNSNameString(&domain, gServiceDomain);

        status = mDNS_StartBrowse(&mDNSStorage, &question, &type, &domain, mDNSNULL, mDNSInterface_Any, 0, mDNSfalse, mDNSfalse, BrowseCallback, NULL);

        // Run the platform main event loop until the user types ^C.
        // The BrowseCallback routine is responsible for printing
        // any results that we find.

        if (status == mStatus_NoError) {
            fprintf(stderr, "Hit ^C when you're bored waiting for responses.\n");
            ExampleClientEventLoop(&mDNSStorage);
            mDNS_StopQuery(&mDNSStorage, &question);
            mDNS_Close(&mDNSStorage);
        }
    }

    if (status == mStatus_NoError) {
        result = 0;
    } else {
        result = 2;
    }
    if ( (result != 0) || (gMDNSPlatformPosixVerboseLevel > 0) ) {
        fprintf(stderr, "%s: Finished with status %d, result %d\n", gProgramName, (int)status, result);
    }

    return 0;
}
