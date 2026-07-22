#include "../include/btc_mcu_flow.h"

#include <string.h>

static int different(const uint8_t *a, const uint8_t *b, size_t len)
{
    uint8_t value = 0;
    for (size_t i = 0; i < len; ++i) value |= a[i] ^ b[i];
    return value != 0;
}

static void wipe(const struct btc_mcu_platform *platform,
                 void *data, size_t len)
{
    if (platform->secure_zero) platform->secure_zero(platform->context, data, len);
    else {
        volatile uint8_t *p = (volatile uint8_t *)data;
        while (len--) *p++ = 0;
    }
}

static void hash256(const struct btc_mcu_platform *platform,
                    const uint8_t *data, size_t len, uint8_t out[32])
{
    uint8_t first[32];
    platform->sha256(platform->context, data, len, first);
    platform->sha256(platform->context, first, sizeof(first), out);
    wipe(platform, first, sizeof(first));
}

static void expected_bip143(const struct btc_mcu_platform *platform,
                            const struct btc_fpga_bip143_request *request,
                            uint8_t out[32])
{
    uint8_t hp[32],hs[32],ho[32],preimage[182];
    size_t off=0;
    hash256(platform,request->outpoint,sizeof(request->outpoint),hp);
    hash256(platform,request->input_sequence,sizeof(request->input_sequence),hs);
    hash256(platform,request->outputs,request->outputs_len,ho);
#define APPEND(source,count) do { memcpy(preimage+off,(source),(count));off+=(count); } while(0)
    APPEND(request->tx_version,4);APPEND(hp,32);APPEND(hs,32);
    APPEND(request->outpoint,36);
    { const uint8_t prefix[4]={0x19,0x76,0xa9,0x14};APPEND(prefix,4); }
    APPEND(request->pubkey_hash,20);
    { const uint8_t suffix[2]={0x88,0xac};APPEND(suffix,2); }
    APPEND(request->prevout_amount,8);APPEND(request->input_sequence,4);
    APPEND(ho,32);APPEND(request->locktime,4);APPEND(request->sighash_type,4);
#undef APPEND
    if(off==sizeof(preimage))hash256(platform,preimage,sizeof(preimage),out);
    else memset(out,0,32);
    wipe(platform,hp,sizeof(hp));wipe(platform,hs,sizeof(hs));
    wipe(platform,ho,sizeof(ho));wipe(platform,preimage,sizeof(preimage));
}

int btc_mcu_authorize_fpga_signature(
    const struct btc_mcu_platform *platform,
    uint8_t *psbt, size_t psbt_len,
    const struct btc_mcu_review *review,
    struct btc_fpga_bip143_request *fpga_request,
    struct btc_fpga_signature_response *signature)
{
    uint8_t freeze_before[32],freeze_after[32],expected_digest[32];
    int result = -1;
    if (!platform || !platform->sha256 || !platform->display_review ||
        !platform->verify_passkey || !platform->wait_decision ||
        !psbt || !psbt_len || !review || !fpga_request || !signature)
        return -1;

    platform->sha256(platform->context, psbt, psbt_len, freeze_before);
    memcpy(fpga_request->freeze_id, freeze_before, 32);
    if (platform->lock_buffer &&
        platform->lock_buffer(platform->context, psbt, psbt_len) != 0)
        goto reject;
    platform->display_review(platform->context, review, freeze_before);

    unsigned attempt;
    for (attempt = 1; attempt <= 3; ++attempt)
        if (platform->verify_passkey(platform->context, attempt)) break;
    if (attempt > 3 || platform->wait_decision(platform->context) != BTC_MCU_APPROVE)
        goto reject;

    platform->sha256(platform->context, psbt, psbt_len, freeze_after);
    if (different(freeze_before, freeze_after, 32)) goto reject;
    if (btc_mcu_fpga_exchange(&platform->fpga_uart, fpga_request, signature,
                              30000U) != 0)
        goto reject;
    expected_bip143(platform,fpga_request,expected_digest);
    if (different(expected_digest,signature->bip143_digest,32)) goto reject;

    /* Check again after the long FPGA operation before signature insertion. */
    platform->sha256(platform->context, psbt, psbt_len, freeze_after);
    if (different(freeze_before, freeze_after, 32)) goto reject;
    result = 0;
    goto out;

reject:
    if (platform->unlock_buffer) platform->unlock_buffer(platform->context);
    wipe(platform, psbt, psbt_len);
    wipe(platform, signature, sizeof(*signature));
out:
    wipe(platform, freeze_before, sizeof(freeze_before));
    wipe(platform, freeze_after, sizeof(freeze_after));
    wipe(platform, expected_digest, sizeof(expected_digest));
    return result;
}

void btc_mcu_release_frozen_buffer(const struct btc_mcu_platform *platform)
{
    if (platform && platform->unlock_buffer)
        platform->unlock_buffer(platform->context);
}
