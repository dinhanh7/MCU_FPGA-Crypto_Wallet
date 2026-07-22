/*
 * SN34F788F adapter template. Add this file to the Sonix/Keil project, replace
 * the board_* calls with the SDK UART/LCM/keypad/MPU functions, then populate
 * btc_mcu_platform with these callbacks. No private-key API exists on the MCU.
 */
#include "include/btc_mcu_flow.h"

extern int board_uart_write(const uint8_t *,size_t,uint32_t);
extern int board_uart_read(uint8_t *,size_t,uint32_t);
extern void board_uart_flush(void);
extern void board_lcd_show_review(const struct btc_mcu_review *,const uint8_t[32]);
extern int board_pin_verify(unsigned);
extern int board_wait_approve_cancel(void);
extern void board_sha256(const uint8_t *,size_t,uint8_t[32]);
extern int board_mpu_make_read_only(const uint8_t *,size_t);
extern void board_mpu_restore(void);

static int uart_write(void *ctx,const uint8_t *p,size_t n,uint32_t timeout)
{ (void)ctx;return board_uart_write(p,n,timeout); }
static int uart_read(void *ctx,uint8_t *p,size_t n,uint32_t timeout)
{ (void)ctx;return board_uart_read(p,n,timeout); }
static void uart_flush(void *ctx){(void)ctx;board_uart_flush();}
static int lock_buffer(void *ctx,const uint8_t *p,size_t n)
{(void)ctx;return board_mpu_make_read_only(p,n);}
static void unlock_buffer(void *ctx){(void)ctx;board_mpu_restore();}
static void sha256_cb(void *ctx,const uint8_t *p,size_t n,uint8_t out[32])
{(void)ctx;board_sha256(p,n,out);}
static void display_cb(void *ctx,const struct btc_mcu_review *r,const uint8_t f[32])
{(void)ctx;board_lcd_show_review(r,f);}
static int pin_cb(void *ctx,unsigned attempt){(void)ctx;return board_pin_verify(attempt);}
static enum btc_mcu_decision decision_cb(void *ctx)
{(void)ctx;return board_wait_approve_cancel()?BTC_MCU_APPROVE:BTC_MCU_CANCEL;}
static void zero_cb(void *ctx,void *data,size_t len)
{(void)ctx;volatile uint8_t *p=data;while(len--)*p++=0;}

const struct btc_mcu_platform sn34f788f_wallet_platform={
  .context=0,.lock_buffer=lock_buffer,.unlock_buffer=unlock_buffer,
  .sha256=sha256_cb,.display_review=display_cb,.verify_passkey=pin_cb,
  .wait_decision=decision_cb,.secure_zero=zero_cb,
  .fpga_uart={.context=0,.write=uart_write,.read=uart_read,.flush=uart_flush}
};
