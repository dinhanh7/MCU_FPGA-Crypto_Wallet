/*_____ I N C L U D E S ____________________________________________________*/
#include "sn34f78x_board.h"

/*_____ D E F I N I T I O N S ______________________________________________*/
#define P0_AFIO0_VAL								0x00000000
#define P0_AFIO1_VAL								0x00000000
#define P0_AFIO2_VAL								0x00000000
#define P0_AFIO3_VAL								0x00000000
#define P1_AFIO0_VAL								0x00000000
#define P1_AFIO1_VAL								0x00000000
#define P1_AFIO2_VAL								0x00000000
#define P2_AFIO0_VAL								0x00000000
#define P2_AFIO1_VAL								0x00000000
#define P2_AFIO2_VAL								0x00000000
#define P3_AFIO0_VAL								0x00000000
#define P3_AFIO1_VAL								0x00000000
#define P3_AFIO2_VAL								0x00000000

/*_____ D E C L A R A T I O N S ____________________________________________*/
#define USB_MCU                                     0x00000000

#if (USB_MCU == 0)
#define P010_IS_UTXD0(x)						((((x >> 20) & 0x1F) == 0x1) ? 1 : 0)
#define P21_IS_UTXD0(x)							((((x >> 5) & 0x1F) == 0x2) ? 1 : 0)
#define P31_IS_UTXD0(x)							((((x >> 5) & 0x1F) == 0x1) ? 1 : 0)
#define P18_IS_UTXD0(x)							((((x >> 10) & 0x1F) == 0x3) ? 1 : 0)

const uint32_t BL_PFPA_UTXD0				__attribute__((section(".ARM.__at_0x000001E0"))) = (P18_IS_UTXD0(P1_AFIO1_VAL) << 3) |\
																			  (P31_IS_UTXD0(P3_AFIO0_VAL) << 2) |\
																			  (P21_IS_UTXD0(P2_AFIO0_VAL) << 1) |\
																			  (P010_IS_UTXD0(P0_AFIO1_VAL));

#define P011_IS_URXD0(x)						((((x >> 25) & 0x1F) == 0x1) ? 1 : 0)
#define P20_IS_URXD0(x)							((((x >> 0) & 0x1F) == 0x2) ? 1 : 0)
#define P32_IS_URXD0(x)							((((x >> 10) & 0x1F) == 0x1) ? 1 : 0)
#define P10_IS_URXD0(x)							((((x >> 0) & 0x1F) == 0x2) ? 1 : 0)

const uint32_t BL_PFPA_URXD0				__attribute__((section(".ARM.__at_0x000001E4"))) = (P10_IS_URXD0(P1_AFIO0_VAL) << 3) |\
																			  (P32_IS_URXD0(P3_AFIO0_VAL) << 2) |\
																			  (P20_IS_URXD0(P2_AFIO0_VAL) << 1) |\
																			  (P011_IS_URXD0(P0_AFIO1_VAL));
#endif

// define the weak AFIO_Init
__weak void AFIO_Init(void)
{
}
