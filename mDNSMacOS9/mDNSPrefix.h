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

// Global definitions that apply to all source files
//
// Symbols that are defined here are available within all source files.
// This is the equivalent of using using "-d SYMBOL=VALUE" in a Makefile
// in command-line build environments.

// For normal DeferredTask time execution, set MDNS_ONLYSYSTEMTASK to 0
// For easier debugging, set MDNS_ONLYSYSTEMTASK to 1, and OT Notifier executions
// will be deferred until SystemTask time. (This option is available only for building
// the standalone application samples that have their own event loop -- don't try
// to build the System Extension with MDNS_ONLYSYSTEMTASK set because it won't work.)

#if   __ide_target("Standalone TestResponder")           || __ide_target("Standalone TestSearcher")           || __ide_target("Standalone SubTypeTester")
#define TARGET_API_MAC_CARBON 1
#define OTCARBONAPPLICATION 1
#define MDNS_ONLYSYSTEMTASK 0
#define MDNS_DEBUGMSGS 0

#elif __ide_target("Standalone TestResponder (Debug)")   || __ide_target("Standalone TestSearcher (Debug)")
#define TARGET_API_MAC_CARBON 1
#define OTCARBONAPPLICATION 1
#define MDNS_ONLYSYSTEMTASK 1
#define MDNS_DEBUGMSGS 1

#elif __ide_target("Standalone TestResponder (Classic)") || __ide_target("Standalone TestSearcher (Classic)")
#define MDNS_ONLYSYSTEMTASK 0
#define MDNS_DEBUGMSGS 0

#elif __ide_target("CFM Library for Extensions Folder")
#define MDNS_BUILDINGSHAREDLIBRARY 2

#elif __ide_target("CFM Library for Extensions (Debug)")
#define MDNS_DEBUGMSGS 0
#define MDNS_BUILDINGSHAREDLIBRARY 1

#elif __ide_target("CFM Stub for clients to link against")
#define MDNS_BUILDINGSTUBLIBRARY 1

#else
#error Options for this target not found in prefix file
#endif

// dnssd_clientlib.c assumes malloc() and free(), so we #define them here to be the OT equivalents
#if MDNS_BUILDINGSHAREDLIBRARY || MDNS_BUILDINGSTUBLIBRARY
#define malloc(x) OTAllocMem(x)
#define free(x) OTFreeMem(x)
#endif
