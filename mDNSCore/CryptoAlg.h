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

#ifndef __CRYPTO_ALG_H
#define __CRYPTO_ALG_H

typedef enum
{
    CRYPTO_ALG,
    DIGEST_ALG,
    ENC_ALG,
} AlgType;

typedef struct
{
    void *context;
    AlgType type;
    mDNSu8 alg;
} AlgContext;

typedef struct
{
    mStatus (*Create)(AlgContext *ctx);
    mStatus (*Destroy)(AlgContext *ctx);
    mDNSu32 (*Length)(AlgContext *ctx);
    mStatus (*Add)(AlgContext *ctx, const void *data, mDNSu32 len);
    // Verify the ctx using the key and compare it against signature/siglen
    mStatus (*Verify)(AlgContext *ctx, mDNSu8 *key, mDNSu32 keylen, mDNSu8 *signature, mDNSu32 siglen);
    // Encode the data and return the encoded data
    mDNSu8* (*Encode)(AlgContext *ctx);
    // Return the finalized data in data whose length is len (used by hash algorithms)
    mStatus (*Final)(AlgContext *ctx, void *data, mDNSu32 len);
} AlgFuncs;

mDNSexport mStatus DigestAlgInit(mDNSu8 digestType, AlgFuncs *func);
mDNSexport mStatus CryptoAlgInit(mDNSu8 algType, AlgFuncs *func);
mDNSexport mStatus EncAlgInit(mDNSu8 algType, AlgFuncs *func);


extern AlgContext *AlgCreate(AlgType type, mDNSu8 alg);
extern mStatus AlgDestroy(AlgContext *ctx);
extern mDNSu32 AlgLength(AlgContext *ctx);
extern mStatus AlgAdd(AlgContext *ctx, const void *data, mDNSu32 len);
extern mStatus AlgVerify(AlgContext *ctx, mDNSu8 *key, mDNSu32 keylen, mDNSu8 *signature, mDNSu32 siglen);
extern mDNSu8* AlgEncode(AlgContext *ctx);
extern mStatus AlgFinal(AlgContext *ctx, void *data, mDNSu32 len);

#endif // __CRYPTO_ALG_H
