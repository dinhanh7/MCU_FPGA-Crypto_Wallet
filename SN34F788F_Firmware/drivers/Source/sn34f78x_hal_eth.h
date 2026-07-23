/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_ETH_H_
#define _SN34F78X_HAL_ETH_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* debug log -------------------------------------------------------------*/
/*
Fatal (fatal): signifies critical conditions in the program that demand intervention to prevent system failure.
Error (error): indicates error conditions that impair some operation but are less severe than critical situations.
Warn (warn): signifies potential issues that may lead to errors or unexpected behavior in the future if not addressed.
Notice (notice): applies to normal but significant conditions that may require monitoring.
Informational (info): includes messages that provide a record of the normal operation of the system.
Debug (debug): intended for logging detailed information about the system for debugging purposes.
*/

#define ETH_DEBUG 0
#if (ETH_DEBUG == 1)
#define ETH_DBG_FATAL  (1 << 5)
#define ETH_DBG_ERR    (1 << 4)
#define ETH_DBG_WARN   (1 << 3)
#define ETH_DBG_NOTICE (1 << 2)
#define ETH_DBG_INFO   (1 << 1)
#define ETH_DBG_DEBUG  (1 << 0)

#define ETH_DBG_ON (ETH_DBG_DEBUG | ETH_DBG_INFO | ETH_DBG_NOTICE | ETH_DBG_WARN | ETH_DBG_ERR | ETH_DBG_FATAL)

#define ETH_PRINT(debug, fmt, args...) \
    if (debug & ETH_DBG_ON)            \
    printf("[ETH]" fmt "\r\n", ##args)

#else
#define ETH_PRINT(debug, fmt, args...)
#endif

/* private types -------------------------------------------------------------*/
/*
 * Interrupt Status Register 0x00 and 0x04
 */
#define ETH_INT_PDELAY_RESP_OUT (1 << 24)
#define ETH_INT_PDELAY_RESP_IN  (1 << 23)
#define ETH_INT_PDELAY_REQ_OUT  (1 << 22)
#define ETH_INT_PDELAY_REQ_IN   (1 << 21)
#define ETH_INT_DELAY_REQ_OUT   (1 << 20)
#define ETH_INT_DELAY_REQ_IN    (1 << 19)
#define ETH_INT_SYNC_OUT        (1 << 18)
#define ETH_INT_SYNC_IN         (1 << 17)
#define ETH_INT_TSU_SEC_INC     (1 << 16)
#define ETH_INT_RX_LPI_IN       (1 << 12)
#define ETH_INT_RX_LPI_EXIT     (1 << 11)
#define ETH_INT_HPTXBUF_UNAVA   (1 << 10)
#define ETH_INT_PHYSTS_CHG      (1 << 9)
#define ETH_INT_AHB_ERR         (1 << 8)
#define ETH_INT_TPKT_LOST       (1 << 7)
#define ETH_INT_NPTXBUF_UNAVA   (1 << 6)
#define ETH_INT_TPKT2F          (1 << 5)
#define ETH_INT_TPKT2E          (1 << 4)
#define ETH_INT_RPKT_LOST       (1 << 3)
#define ETH_INT_RXBUF_UNAVA     (1 << 2)
#define ETH_INT_RPKT2F          (1 << 1)
#define ETH_INT_RPKT2B          (1 << 0)

#define ETH_INT_DEFAULT ( \
    ETH_INT_RPKT2B |      \
    ETH_INT_TPKT2E |      \
    ETH_INT_TPKT2F |      \
    ETH_INT_TPKT_LOST |   \
    ETH_INT_AHB_ERR |     \
    ETH_INT_PHYSTS_CHG)

#define ETH_INT_ALL (       \
    ETH_INT_RPKT2B |        \
    ETH_INT_RPKT2F |        \
    ETH_INT_RXBUF_UNAVA |   \
    ETH_INT_RPKT_LOST |     \
    ETH_INT_TPKT2E |        \
    ETH_INT_TPKT2F |        \
    ETH_INT_NPTXBUF_UNAVA | \
    ETH_INT_TPKT_LOST |     \
    ETH_INT_AHB_ERR |       \
    ETH_INT_PHYSTS_CHG |    \
    ETH_INT_HPTXBUF_UNAVA)

/*
 * TX Interrupt Timer Control Register 0x30
 */
#define ETH_TXITC_TIME_SEL    (1 << 16)
#define ETH_TXITC_CYL(x)      ((x & 0xff) << 8)
#define ETH_TXITC_THR(x)      ((x & 0x7) << 4)
#define ETH_TXITC_THR_UNIT(x) ((x & 0x3) << 0)

#define ETH_TXITC_DEFAULT ETH_TXITC_THR(1)

/*
 * RX Interrupt Timer Control Register 0x34
 */
#define ETH_RXITC_RST(x)      ((x & 0xff) << 16)
#define ETH_RXITC_TIME_SEL    (1 << 16)
#define ETH_RXITC_CYL(x)      ((x & 0xff) << 8)
#define ETH_RXITC_THR(x)      ((x & 0x7) << 4)
#define ETH_RXITC_THR_UNIT(x) ((x & 0x3) << 0)

#define ETH_RXITC_DEFAULT ETH_RXITC_THR(1)

/*
 * Automatic Polling Timer Control Register 0x34
 */
#define ETH_APTC_TX_TIME   (1 << 12)
#define ETH_APTC_TX_CNT(x) ((x & 0xf) << 8)
#define ETH_APTC_RX_TIME   (1 << 4)
#define ETH_APTC_RX_CNT(x) ((x & 0xf) << 0)

#define ETH_APTC_DEFAULT ETH_APTC_RX_CNT(1)

/*
 * DMA Burst Length and Arbitration Control Register 0x38
 */
#define ETH_DBLAC_IFG_INC        (1 << 23)
#define ETH_DBLAC_IFG_CNT(x)     ((x & 0x7) << 20)
#define ETH_DBLAC_TXDES_SIZE(x)  ((x & 0xf) << 16)
#define ETH_DBLAC_RXDES_SIZE(x)  ((x & 0xf) << 12)
#define ETH_DBLAC_TXBST_SIZE(x)  ((x & 0x3) << 10)
#define ETH_DBLAC_RXBST_SIZE(x)  ((x & 0x3) << 8)
#define ETH_DBLAC_RX_THR_EN      (1 << 6)
#define ETH_DBLAC_RXFIFO_HTHR(x) ((x & 0x7) << 3)
#define ETH_DBLAC_RXFIFO_LTHR(x) ((x & 0x7) << 0)

#define ETH_DBLAC_DEFAULT ETH_DBLAC_RXFIFO_LTHR(2) |     \
                              ETH_DBLAC_RXFIFO_HTHR(6) | \
                              ETH_DBLAC_RX_THR_EN |      \
                              ETH_DBLAC_RXBST_SIZE(3) |  \
                              ETH_DBLAC_TXBST_SIZE(3)
/*
 * DMA/FIFO State Register 0x3c
 */
#define ETH_DMAFIFOS_TXD_REQ      (1 << 31)
#define ETH_DMAFIFOS_RXD_REQ      (1 << 30)
#define ETH_DMAFIFOS_DARB_TXGNT   (1 << 29)
#define ETH_DMAFIFOS_DARB_RXGNT   (1 << 28)
#define ETH_DMAFIFOS_TXFIFO_EMPTY (1 << 27)
#define ETH_DMAFIFOS_RXFIFO_EMPTY (1 << 26)
#define ETH_DMAFIFOS_TXDMA3_SM(x) ((x & 0xf) << 18)
#define ETH_DMAFIFOS_TXDMA2_SM(x) ((x & 0xf) << 16)
#define ETH_DMAFIFOS_TXDMA1_SM(x) ((x & 0xf) << 12)
#define ETH_DMAFIFOS_RXDMA3_SM(x) ((x & 0xf) << 8)
#define ETH_DMAFIFOS_RXDMA2_SM(x) ((x & 0xf) << 4)
#define ETH_DMAFIFOS_RXDMA1_SM(x) ((x & 0xf) << 0)

/*
 * Revision Register 0x40
 */
#define ETH_REV_B1(x) ((x >> 16) & 0xff)
#define ETH_REV_B2(x) ((x >> 8) & 0xff)
#define ETH_REV_B3(x) (x & 0xff)

/*
 * Feature Register 0xF8
 */
#define ETH_FEAR_TFIFO_RSIZE(x) ((x & 0x7) >> 4)
#define ETH_FEAR_RFIFO_RSIZE(x) ((x & 0x7) >> 0)

/*
 * Transmit Priority Arbitration and FIFO Control Register 0x48
 */
#define ETH_TPAFCR_TFIFO_SIZE(x)  ((x & 0x7) << 27)
#define ETH_TPAFCR_RFIFO_SIZE(x)  ((x & 0x7) << 24)
#define ETH_TPAFCR_EARLY_TXTHR(x) ((x & 0xff) << 16)
#define ETH_TPAFCR_EARLY_RXTHR(x) ((x & 0xff) << 8)
#define ETH_TPAFCR_HPKT_THR(x)    ((x & 0xf) << 4)
#define ETH_TPAFCR_NPKT_THR(x)    ((x & 0xf) << 0)

/*
 * Receive Buffer Size Register 0x4c
 */
#define ETH_RBSR_RXBUF_SIZE(x) ((x & 0x3fff) << 0)

/*
 * MAC Control Register 0x50
 */
#define ETH_MACCR_SW_RST     ((uint32_t)1 << 31)
#define ETH_MACCR_FULLDUP    (1 << 26)
#define ETH_MACCR_SPEED_MASK (0x3 << 24)
// #define ETH_MACCR_SPEED_1000     (2 << 24)
// #define ETH_MACCR_SPEED_100M     (1 << 24)
// #define ETH_MACCR_SPEED_10M      (0 << 24)
#define ETH_MACCR_HPTXR          (1 << 22)
#define ETH_MACCR_LOOPBACK       (1 << 21)
#define ETH_MACCR_PTP_EN         (1 << 20)
#define ETH_MACCR_REMOVE_VLAN    (1 << 18)
#define ETH_MACCR_CRC_APD        (1 << 17)
#define ETH_MACCR_DROP_CRC_ERR   (1 << 16)
#define ETH_MACCR_ENRX_IN_HALFTX (1 << 14)
#define ETH_MACCR_JUMBO_LF       (1 << 13)
#define ETH_MACCR_RX_RUNT        (1 << 12)
#define ETH_MACCR_BROADPKT       (1 << 11)
#define ETH_MACCR_MULTIPKT       (1 << 10)
#define ETH_MACCR_HT_EN          (1 << 9)
#define ETH_MACCR_ALLADDR        (1 << 8)
#define ETH_MACCR_RXMAC          (1 << 3)
#define ETH_MACCR_TXMAC          (1 << 2)
#define ETH_MACCR_RXDMA          (1 << 1)
#define ETH_MACCR_TXDMA          (1 << 0)

#define ETH_MACCR_DEFAULT ( \
    ETH_MACCR_RX_RUNT |     \
    ETH_MACCR_CRC_APD |     \
    ETH_MACCR_FULLDUP |     \
    ETH_MACCR_HPTXR |       \
    ETH_MACCR_TXDMA |       \
    ETH_MACCR_RXMAC |       \
    ETH_MACCR_RXDMA |       \
    ETH_MACCR_TXMAC)

/*
 * MAC Status Register 0x54
 */
#define ETH_MACSR_COL_EXCEED
#define ETH_MACSR_LATE_COL
#define ETH_MACSR_XPKT_LOST
#define ETH_MACSR_XPKT_OK
#define ETH_MACSR_RUNT
#define ETH_MACSR_FTL
#define ETH_MACSR_CRC_ERR
#define ETH_MACSR_RPKT_LOST
#define ETH_MACSR_RPKT_SAVE
#define ETH_MACSR_COL
#define ETH_MACSR_BROADCAST
#define ETH_MACSR_MULTICAST

/*
 * Test Mode Register 0x58
 */
#define ETH_TM_PTIMER_TEST    (1 << 20)
#define ETH_TM_ITIMER_TEST    (1 << 19)
#define ETH_TM_TEST_COL       (1 << 15)
#define ETH_TM_TEST_BKOFF(x)  ((x & 0x3ff) << 5)
#define ETH_TM_TEST_EXSTHR(x) ((x & 0x1f) << 0)

/*
 * PHY Control Register 0x60
 */
#define ETH_MDIO_SOF     1
#define ETH_MDIO_EXT_SOF 0
#define ETH_MDIO_OP_RD   2
#define ETH_MDIO_OP_WR   1

#define ETH_PHYCR_PHYWR    (1 << 27)
#define ETH_PHYCR_PHYRD    (1 << 26)
#define ETH_PHYCR_REGAD(x) ((x & 0x1f) << 21)
#define ETH_PHYCR_PHYAD(x) ((x & 0x1f) << 16)
#define ETH_PHYCR_OP(x)    ((x & 0x3) << 14)
#define ETH_PHYCR_SOF(x)   ((x & 0x3) << 12)

/*
 * PHY Data Register 0x64
 */
#define ETH_PHYDATA_MIIRDATA(x) ((x & 0xffff0000) >> 16)
#define ETH_PHYDATA_MIIWDATA(x) ((x & 0xffff) >> 0)

/*
 * Flow Control Register 0x68
 */
#define ETH_FCR_PAUSE_TIME(x) ((x & 0xffff) << 16)
#define ETH_FCR_FC_H_L(x)     ((x & 0x7f) << 9)
#define ETH_FCR_HTHR          (1 << 8)
#define ETH_FCR_RX_PAUSE      (1 << 4)
#define ETH_FCR_TXPAUSED      (1 << 3)
#define ETH_FCR_FCTHR_EN      (1 << 2)
#define ETH_FCR_TX_PAUSE      (1 << 1)
#define ETH_FCR_FC_EN         (1 << 0)

/*
 * Back Pressure Register 0x6c
 */
#define ETH_BPR_BK_LOW(x)    ((x & 0x7f) << 8)
#define ETH_BPR_BKJAM_LEN(x) ((x & 0xf) << 4)
#define ETH_BPR_BKADR_MODE   (1 << 1)
#define ETH_BPR_BKEN         (1 << 0)

/*
 * Wake-On-LAN Control Register 0x70
 */
#define ETH_WOLCR_WOL_TYPE(x)   ((x & 0x3) << 24)
#define ETH_WOLCR_SW_PDNPHY     (1 << 18)
#define ETH_WOLCR_WAKEUP_SEL(x) ((x & 0x3) << 16)
#define ETH_WOLCR_WAKEUP4       (1 << 6)
#define ETH_WOLCR_WAKEUP3       (1 << 5)
#define ETH_WOLCR_WAKEUP2       (1 << 4)
#define ETH_WOLCR_WAKEUP1       (1 << 3)
#define ETH_WOLCR_MAGICPKT      (1 << 2)
#define ETH_WOLCR_LINKCHG1      (1 << 1)
#define ETH_WOLCR_LINKCHG0      (1 << 0)

/*
 * Wake-On-LAN Status Register 0x74
 */
#define ETH_WOLSR_WAKEUP4  (1 << 6)
#define ETH_WOLSR_WAKEUP3  (1 << 5)
#define ETH_WOLSR_WAKEUP2  (1 << 4)
#define ETH_WOLSR_WAKEUP1  (1 << 3)
#define ETH_WOLSR_MAGICPKT (1 << 2)
#define ETH_WOLSR_LINKCHG1 (1 << 1)
#define ETH_WOLSR_LINKCHG0 (1 << 0)

#define ETH_TXDES0_TXDMA_OWN   ((uint32_t)1 << 31)
#define ETH_TXDES0_FTS         (1 << 29)
#define ETH_TXDES0_LTS         (1 << 28)
#define ETH_TXDES0_CRC_ERR     (1 << 19)
#define ETH_TXDES0_EDOTR       (1 << 15)
#define ETH_TXDES0_BUF_SIZE(x) ((x & 0x3fff) << 0)

#define ETH_TXDES1_TXIC         ((uint32_t)1 << 31)
#define ETH_TXDES1_TX2FIC       (1 << 30)
#define ETH_TXDES1_LLC_PKT      (1 << 22)
#define ETH_TXDES1_IPV6_PKT     (1 << 20)
#define ETH_TXDES1_OTHER_PKT    (2 << 20)
#define ETH_TXDES1_IPCS_EN      (1 << 19)
#define ETH_TXDES1_UDPCS_EN     (1 << 18)
#define ETH_TXDES1_TCPCS_EN     (1 << 17)
#define ETH_TXDES1_INS_VLAN     (1 << 16)
#define ETH_TXDES1_VLAN_TAGC(x) ((x & 0xffff) << 0)

#define ETH_TXDES1_DEFAULT (ETH_TXDES1_TXIC | ETH_TXDES1_TX2FIC)

#define ETH_RXDES0_RXPKT_RDY    ((uint32_t)1 << 31)
#define ETH_RXDES0_FRS          (1 << 29)
#define ETH_RXDES0_LRS          (1 << 28)
#define ETH_RXDES0_PAUSE_FRAME  (1 << 25)
#define ETH_RXDES0_PAUSE_OPCODE (1 << 24)
#define ETH_RXDES0_FIFO_FULL    (1 << 23)
#define ETH_RXDES0_RX_ODD_NB    (1 << 22)
#define ETH_RXDES0_RUNT         (1 << 21)
#define ETH_RXDES0_FTL          (1 << 20)
#define ETH_RXDES0_CRC_ERR      (1 << 19)
#define ETH_RXDES0_RX_ERR       (1 << 18)
#define ETH_RXDES0_BROADCAST    (1 << 17)
#define ETH_RXDES0_MULTICAST    (1 << 16)
// #define ETH_RXDES0_EDORR        (1 << 15)
#define ETH_RXDES0_VDBC(x) ((x & 0x3fff) << 0)

#define ETH_RXDES1_IPCS_FAIL     (1 << 27)
#define ETH_RXDES1_UDPCS_FAIL    (1 << 26)
#define ETH_RXDES1_TCPCS_FAIL    (1 << 25)
#define ETH_RXDES1_VLAN_AVA      (1 << 24)
#define ETH_RXDES1_DF            (1 << 23)
#define ETH_RXDES1_LLC_PKT       (1 << 22)
#define ETH_RXDES1_PROTL_TYPE(x) ((x >> 20) & 0x3)
#define ETH_RXDES1_IP6_TYPE(x)   ((x >> 19) & 0x1)
#define ETH_RXDES1_VLAN_PRIO(x)  ((x & 7) << 13)
#define ETH_RXDES1_VLAN_CFI(x)   ((x & 1) << 12)
#define ETH_RXDES1_VLAN_VID(x)   (x & 0x0fff)
#define ETH_RXDES1_VLAN_TAGC(x)  (x & 0xffff)

#define ETH_RXDES1_PROTL_NOTIP 0
#define ETH_RXDES1_PROTL_IP4   1
#define ETH_RXDES1_PROTL_TCPIP 2
#define ETH_RXDES1_PROTL_UDPIP 3

/* Parameter check define */
#define IS_ETH_PHY_ADDRESS(ADDRESS) ((ADDRESS) <= 0x20U)
#define IS_ETH_AUTONEGOTIATION(CMD) (((CMD) == ETH_AUTONEGOTIATION_ENABLE) || \
                                     ((CMD) == ETH_AUTONEGOTIATION_DISABLE))

#define IS_ETH_RX_MODE(MODE) (((MODE) == ETH_RXPOLLING_MODE) || \
                              ((MODE) == ETH_RXINTERRUPT_MODE))
#define IS_ETH_CHECKSUM_MODE(MODE) (((MODE) == ETH_CHECKSUM_BY_HARDWARE) || \
                                    ((MODE) == ETH_CHECKSUM_BY_SOFTWARE))
#define IS_ETH_MEDIA_INTERFACE(MODE) (((MODE) == ETH_MEDIA_INTERFACE_MII) || \
                                      ((MODE) == ETH_MEDIA_INTERFACE_RMII))
#define IS_ETH_SPEED(SPEED)      (((SPEED) == ETH_MACCR_SPEED_10M) || ((SPEED) == ETH_MACCR_SPEED_100M)) /*!< Check Eth speed */
#define IS_ETH_DUPLEX_MODE(MODE) (((MODE) == ETH_MACCR_FULLDUPLEX) || ((MODE) == ETH_MACCR_HALFDUPLEX))  /*!< Check Transfer mode */

/* DMA Description table */
#define ETH_TXDES0_TXDMA_OWN   ((uint32_t)1 << 31)
#define ETH_TXDES0_FTS         (1 << 29)
#define ETH_TXDES0_LTS         (1 << 28)
#define ETH_TXDES0_CRC_ERR     (1 << 19)
#define ETH_TXDES0_EDOTR       (1 << 15)
#define ETH_TXDES0_BUF_SIZE(x) ((x & 0x3fff) << 0)

#define ETH_TXDES1_TXIC         ((uint32_t)1 << 31)
#define ETH_TXDES1_TX2FIC       (1 << 30)
#define ETH_TXDES1_LLC_PKT      (1 << 22)
#define ETH_TXDES1_IPV6_PKT     (1 << 20)
#define ETH_TXDES1_OTHER_PKT    (2 << 20)
#define ETH_TXDES1_IPCS_EN      (1 << 19)
#define ETH_TXDES1_UDPCS_EN     (1 << 18)
#define ETH_TXDES1_TCPCS_EN     (1 << 17)
#define ETH_TXDES1_INS_VLAN     (1 << 16)
#define ETH_TXDES1_VLAN_TAGC(x) ((x & 0xffff) << 0)

#define ETH_TXDES1_DEFAULT (ETH_TXDES1_TXIC | ETH_TXDES1_TX2FIC)

#define ETH_RXDES0_RXPKT_RDY    ((uint32_t)1 << 31)
#define ETH_RXDES0_FRS          (1 << 29)
#define ETH_RXDES0_LRS          (1 << 28)
#define ETH_RXDES0_PAUSE_FRAME  (1 << 25)
#define ETH_RXDES0_PAUSE_OPCODE (1 << 24)
#define ETH_RXDES0_FIFO_FULL    (1 << 23)
#define ETH_RXDES0_RX_ODD_NB    (1 << 22)
#define ETH_RXDES0_RUNT         (1 << 21)
#define ETH_RXDES0_FTL          (1 << 20)
#define ETH_RXDES0_CRC_ERR      (1 << 19)
#define ETH_RXDES0_RX_ERR       (1 << 18)
#define ETH_RXDES0_BROADCAST    (1 << 17)
#define ETH_RXDES0_MULTICAST    (1 << 16)
#define ETH_RXDES0_EDORR        (1 << 15)
#define ETH_RXDES0_VDBC(x)      ((x & 0x3fff) << 0)

#define ETH_RXDES1_IPCS_FAIL     (1 << 27)
#define ETH_RXDES1_UDPCS_FAIL    (1 << 26)
#define ETH_RXDES1_TCPCS_FAIL    (1 << 25)
#define ETH_RXDES1_VLAN_AVA      (1 << 24)
#define ETH_RXDES1_DF            (1 << 23)
#define ETH_RXDES1_LLC_PKT       (1 << 22)
#define ETH_RXDES1_PROTL_TYPE(x) ((x >> 20) & 0x3)
#define ETH_RXDES1_IP6_TYPE(x)   ((x >> 19) & 0x1)
#define ETH_RXDES1_VLAN_PRIO(x)  ((x & 7) << 13)
#define ETH_RXDES1_VLAN_CFI(x)   ((x & 1) << 12)
#define ETH_RXDES1_VLAN_VID(x)   (x & 0x0fff)
#define ETH_RXDES1_VLAN_TAGC(x)  (x & 0xffff)

#define ETH_RXDES1_PROTL_NOTIP 0
#define ETH_RXDES1_PROTL_IP4   1
#define ETH_RXDES1_PROTL_TCPIP 2
#define ETH_RXDES1_PROTL_UDPIP 3

/* PHY Configuration delay */
#define PHY_CONFIG_DELAY 0x00000FFFU
#define PHY_READ_TO      0x0000FFFFU
#define PHY_WRITE_TO     0x0000FFFFU

/* Eethernet mac addr length */
#define ETH_HWADDR_LEN 6

/** Ethernet header */
struct eth_hdr
{
    uint8_t  dest[ETH_HWADDR_LEN];
    uint8_t  src[ETH_HWADDR_LEN];
    uint16_t type;
};

/*A list of ethtypes*/
enum eth_type
{
    /** Internet protocol v4 */
    ETHTYPE_IP   = 0x0800U,
    ETHTYPE_IPV6 = 0x86DDU
};

/* These macros should be calculated by the preprocessor and are used
   with compile-time constants only (so that there is no little-endian
   overhead at runtime). */
#define PP_HTONS(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))

/* private variables ---------------------------------------------------------*/

/* private constants ---------------------------------------------------------*/

#endif /* _SN34F78X_HAL_ETH_H_ */
