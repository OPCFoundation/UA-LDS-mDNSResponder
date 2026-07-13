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

ERROR(kmDNSHelperCommunicationFailed,             "Mach communication failed")
ERROR(kmDNSHelperNotAuthorized,                   "Not authorized")
ERROR(kmDNSHelperCreationFailed,                  "Object creation failed")
ERROR(kmDNSHelperInvalidPList,                    "Invalid property list")
ERROR(kmDNSHelperInvalidNameKey,                  "Invalid name key")
ERROR(kmDNSHelperInvalidConfigKey,                "Invalid configuration key")
ERROR(kmDNSHelperTypeError,                       "Object was not of expected type")
ERROR(kmDNSHelperPreferencesFailed,               "Could not create preferences session")
ERROR(kmDNSHelperPreferencesLockFailed,           "Could not lock preferences")
ERROR(kmDNSHelperPreferencesSetFailed,            "Could not update preferences")
ERROR(kmDNSHelperKeychainCopyDefaultFailed,       "Could not copy keychain default")
ERROR(kmDNSHelperKeychainSearchCreationFailed,    "Could not create keychain search")
ERROR(kmDNSHelperPListWriteFailed,                "Could not write property list to stream")
ERROR(kmDNSHelperResultTooLarge,                  "Result too large")
ERROR(kmDNSHelperInterfaceCreationFailed,         "Could not create auto-tunnel interface")
ERROR(kmDNSHelperInterfaceDeletionFailed,         "Could not delete auto-tunnel interface")
ERROR(kmDNSHelperInvalidInterfaceState,           "Invalid interface state requested")
ERROR(kmDNSHelperInvalidServerState,              "Invalid server state requested")
ERROR(kmDNSHelperRacoonConfigCreationFailed,      "Could not create racoon configuration file")
ERROR(kmDNSHelperRacoonStartFailed,               "Could not start racoon")
ERROR(kmDNSHelperRacoonNotificationFailed,        "Could not notify racoon")
ERROR(kmDNSHelperInvalidTunnelSetKeysOperation,   "Invalid tunnel setkey operation requested")
ERROR(kmDNSHelperInvalidNetworkAddress,           "Invalid network address")
ERROR(kmDNSHelperRouteAdditionFailed,             "Could not add route")
ERROR(kmDNSHelperRouteDeletionFailed,             "Could not remove route")
ERROR(kmDNSHelperRoutingSocketCreationFailed,     "Could not create routing socket")
ERROR(kmDNSHelperDatagramSocketCreationFailed,    "Could not create datagram socket")
ERROR(kmDNSHelperIPsecPolicyCreationFailed,       "Could not create IPsec policy")
ERROR(kmDNSHelperIPsecPolicySetFailed,            "Could not set IPsec policy")
ERROR(kmDNSHelperIPsecRemoveSAFailed,             "Could not remove IPsec SA")
ERROR(kmDNSHelperIPsecPolicySocketCreationFailed, "Could not create IPsec policy socket")
ERROR(kmDNSHelperIPsecDisabled,                   "IPSec support was not compiled in to the helper")
