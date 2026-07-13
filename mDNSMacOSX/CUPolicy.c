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

#include "mDNSMacOSX.h"
#include <network/config.h>

#if TARGET_OS_IPHONE

mDNSexport void CUPInit(mDNS *const m)
{
    
    m->p->handle = cellular_usage_policy_create_client();
    if (!m->p->handle)
    {
        LogMsg("CUPInit: cellular_usage_policy_create_client failed");
    }
}

mDNSexport mDNSBool mDNSPlatformAllowPID(mDNS *const m, DNSQuestion *q)
{
    // Currently the policy applies only for DNS requests sent over cellular interface
    if (m->p->handle && q->qDNSServer && q->qDNSServer->cellIntf)
    {
        mDNSBool allowed;
        if (q->pid)
        {
            allowed = (mDNSBool) cellular_usage_policy_is_data_allowed_for_pid(m->p->handle, q->pid);
            if (!allowed)
            {
                xpc_object_t pidx = xpc_uint64_create(q->pid);
                if (pidx)
                {
                    network_config_cellular_blocked_notify(pidx, NULL, NULL);
                    LogInfo("mDNSPlaformAllowPID: Notified PID(%d) for %##s (%s)", q->pid, q->qname.c, DNSTypeName(q->qtype));
                    xpc_release(pidx);
                }
            }
        }
        else
        {
           xpc_object_t uuidx = xpc_uuid_create(q->uuid);
           if (uuidx)
           {
               allowed = (mDNSBool) cellular_usage_policy_is_data_allowed_for_uuid(m->p->handle, uuidx);
               if (!allowed)
               {
                   network_config_cellular_blocked_notify(NULL, uuidx, NULL);
                   LogInfo("mDNSPlaformAllowPID: Notified UUID for %##s (%s)", q->qname.c, DNSTypeName(q->qtype));
               }
               xpc_release(uuidx);
           }
           else
           {
               allowed = false;
           }
       }
        return allowed;
    }
    else
    {
        return mDNStrue;
    }
}

#else // TARGET_OS_IPHONE

mDNSexport void CUPInit(mDNS *const m)
{
    (void)m; //unused
}

mDNSexport mDNSBool mDNSPlatformAllowPID(mDNS *const m, DNSQuestion *q)
{
    (void)m;    //unused
    (void)q;    //unused
    //LogMsg("mDNSPlatformAllowPID: %##s (%s)", q->qname.c, DNSTypeName(q->qtype));
    return mDNStrue;
}

#endif // TARGET_OS_IPHONE
