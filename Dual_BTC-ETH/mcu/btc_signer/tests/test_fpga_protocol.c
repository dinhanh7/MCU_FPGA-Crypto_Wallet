#include "../src/fpga_protocol.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    uint8_t outputs[3]={1,2,3},frame[BTC_FPGA_MAX_REQUEST_FRAME];
    size_t frame_len=0;
    struct btc_fpga_bip143_request request={0};
    request.sequence=0x42;request.outputs=outputs;request.outputs_len=3;
    request.sighash_type[0]=1;
    for(unsigned i=0;i<32;i++)request.freeze_id[i]=(uint8_t)i;
    assert(btc_fpga_encode_bip143_request(&request,frame,sizeof(frame),&frame_len)==0);
    assert(frame_len==7+BTC_FPGA_REQUEST_FIXED_BYTES+3+2);
    assert(frame[0]==0xa5&&frame[1]==0x5a&&frame[3]==0x42&&frame[4]==0x10);
    assert(frame[7+106]==1&&frame[7+107]==2&&frame[7+108]==3);
    assert(btc_fpga_crc16_ccitt(frame+2,frame_len-4)==
           (uint16_t)(((uint16_t)frame[frame_len-2]<<8)|frame[frame_len-1]));

    uint8_t response_frame[8+128+2]={0};
    response_frame[0]=0x5a;response_frame[1]=0xa5;response_frame[2]=1;
    response_frame[3]=0x42;response_frame[4]=0x10;response_frame[5]=0;
    response_frame[6]=0;response_frame[7]=128;
    memcpy(response_frame+8,request.freeze_id,32);
    for(unsigned i=0;i<96;i++)response_frame[40+i]=(uint8_t)(0x80+i);
    uint16_t crc=btc_fpga_crc16_ccitt(response_frame+2,sizeof(response_frame)-4);
    response_frame[sizeof(response_frame)-2]=(uint8_t)(crc>>8);
    response_frame[sizeof(response_frame)-1]=(uint8_t)crc;
    struct btc_fpga_signature_response response;
    assert(btc_fpga_decode_signature_response(response_frame,sizeof(response_frame),
           0x42,request.freeze_id,&response)==0);
    assert(response.bip143_digest[0]==0x80&&response.r[0]==0xa0&&response.s[0]==0xc0);
    response_frame[20]^=1;
    assert(btc_fpga_decode_signature_response(response_frame,sizeof(response_frame),
           0x42,request.freeze_id,&response)!=0);

    uint8_t eth_hash[32], eth_request[41];
    for (unsigned i=0;i<32;i++) eth_hash[i]=(uint8_t)(0xf0U+i);
    assert(btc_fpga_encode_eth_hash_request(0x17,eth_hash,eth_request,
           sizeof(eth_request),&frame_len)==0);
    assert(frame_len==41&&eth_request[3]==0x17&&eth_request[4]==0x01);
    assert(memcmp(eth_request+7,eth_hash,32)==0);

    uint8_t eth_response[8+65+2]={0};
    eth_response[0]=0x5a;eth_response[1]=0xa5;eth_response[2]=1;
    eth_response[3]=0x17;eth_response[4]=0x01;eth_response[5]=0;
    eth_response[6]=0;eth_response[7]=65;eth_response[8]=1;
    for(unsigned i=0;i<64;i++)eth_response[9+i]=(uint8_t)(i+1);
    crc=btc_fpga_crc16_ccitt(eth_response+2,sizeof(eth_response)-4);
    eth_response[sizeof(eth_response)-2]=(uint8_t)(crc>>8);
    eth_response[sizeof(eth_response)-1]=(uint8_t)crc;
    struct btc_fpga_eth_signature_response eth_sig;
    assert(btc_fpga_decode_eth_signature_response(eth_response,
           sizeof(eth_response),0x17,&eth_sig)==0);
    assert(eth_sig.y_parity==1&&eth_sig.r[0]==1&&eth_sig.s[31]==64);
    puts("FPGA_PROTOCOL_TEST_OK");
    return 0;
}
