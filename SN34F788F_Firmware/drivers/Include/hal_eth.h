/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_ETH_H_
#define _HAL_ETH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* ETH SPEED */
#define ETH_MACCR_SPEED_1000M (2 << 24) /*!< ETH SPEED 1G */
#define ETH_MACCR_SPEED_100M  (1 << 24) /*!< ETH SPEED 100M */
#define ETH_MACCR_SPEED_10M   (0 << 24) /*!< ETH SPEED 10M */

/* ETH DUPLEX */
#define ETH_MACCR_FULLDUPLEX (1 << 26)
#define ETH_MACCR_HALFDUPLEX (0 << 26)

/* DM9162_PHY_ADDRESS Address*/
#define DM9162_PHY_ADDRESS 0U /*!< DM9162 PHY ADDRESS*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  HAL State structures definition
 */
typedef enum
{
    HAL_ETH_STATE_RESET      = 0x00U, /*!< Peripheral not yet Initialized or disabled         */
    HAL_ETH_STATE_READY      = 0x01U, /*!< Peripheral Initialized and ready for use           */
    HAL_ETH_STATE_BUSY       = 0x02U, /*!< an internal process is ongoing                     */
    HAL_ETH_STATE_BUSY_TX    = 0x12U, /*!< Data Transmission process is ongoing               */
    HAL_ETH_STATE_BUSY_RX    = 0x22U, /*!< Data Reception process is ongoing                  */
    HAL_ETH_STATE_BUSY_TX_RX = 0x32U, /*!< Data Transmission and Reception process is ongoing */
    HAL_ETH_STATE_BUSY_WR    = 0x42U, /*!< Write process is ongoing                           */
    HAL_ETH_STATE_BUSY_RD    = 0x82U, /*!< Read process is ongoing                            */
    HAL_ETH_STATE_TIMEOUT    = 0x03U, /*!< Timeout state                                      */
    HAL_ETH_STATE_ERROR      = 0x04U  /*!< Reception process is ongoing                       */
} HAL_ETH_StateTypeDef;

/**
 * @brief  ETH Init Structure definition
 * \ingroup eth_struct_define
 */

typedef struct
{
    uint32_t AutoNegotiation; /*!< Selects or not the AutoNegotiation mode for the external PHY
                                       The AutoNegotiation allows an automatic setting of the Speed (10/100Mbps)
                                       and the mode (half/full-duplex).
                                       This parameter can be a value of ETH_AutoNegotiation */
    uint32_t Speed;           /*!< Sets the Ethernet speed: 10/100 Mbps.
                                       This parameter can be a value of ETH_Speed */
    uint32_t DuplexMode;      /*!< Selects the MAC duplex mode: Half-Duplex or Full-Duplex mode
                                       This parameter can be a value of ETH_Duplex_Mode */
    uint16_t PhyAddress;      /*!< Ethernet PHY address.
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 32 */
    uint8_t  MACAddr[6];      /*!< MAC Address of used Hardware: must be pointer on an array of 6 bytes */
    uint32_t RxMode;          /*!< Selects the Ethernet Rx mode: Polling mode, Interrupt mode.
                                       This parameter can be a value of ETH_Rx_Mode */
    uint32_t ChecksumMode;    /*!< Selects if the checksum is check by hardware or by software.
                                     This parameter can be a value of ETH_Checksum_Mode */
    uint32_t MediaInterface;  /*!< Selects the media-independent interface or the reduced media-independent interface.
                              This parameter can be a value of ETH_Media_Interface */
} ETH_Init_T;

/**
 * @brief  ETH MAC Configuration Structure definition
 */

/**
 * @brief  ETH DMA Descriptors data structure definition
 * \ingroup eth_struct_define
 */

typedef struct
{
    unsigned int StatusAndVDBC;       /*!< reserved */
    unsigned int StatusAndVLAN;       /*!< reserved */
    unsigned int Buffer2NextDescAddr; /*!< reserved */
    unsigned int Buffer1Addr;         /*!<  reserved */
} ETH_DMADescTypeDef;

/**
 * @brief  Received Frame Information structure definition
 * \ingroup eth_struct_define
 */
typedef struct
{
    ETH_DMADescTypeDef *FSRxDesc; /*!< First Segment Rx Desc */
    ETH_DMADescTypeDef *LSRxDesc; /*!< Last Segment Rx Desc */
    uint32_t            SegCount; /*!< Segment count */
    uint32_t            length;   /*!< Frame length */
    uint32_t            buffer;   /*!< Frame buffer */
} ETH_DMARxFrameInfos;

/**
 * @brief  ETH Power Down structure definition
 * \ingroup eth_struct_define
 */
typedef struct
{
    uint32_t LinkDetect;         /*!< Enable or Disable Global unicast packet detection in power down mode */
    uint32_t MagicPacket;        /*!< Enable or Disable Magic packet detection in power down mode */
    uint32_t WakeUpFrame;        /*!< Enable or Disable Wake up frame detection in power down mode */
    uint32_t WakeUpFrameMask[4]; /*!< Wakeup frame mask for  Wake up frame detection */
    uint8_t *pFrame;             /*!< Pointer to wakeup frame content */
    uint32_t FrameSize;          /*!< Wakeup frame size */
    uint32_t FrameNumber;        /*!< Wakeup frame number, it can be 0~3 */
} ETH_PowerDownConfigTypeDef;
/**
 *
 */

/**
 * @brief  ETH WOL structure definition
 * \ingroup eth_struct_define
 */
typedef struct
{
    uint32_t WakeUpPacket;  /*!< Enable or Disable Wake up packet detection in power down mode */
    uint32_t MagicPacket;   /*!< Enable or Disable Magic packet detection in power down mode */
    uint32_t GlobalUnicast; /*!< Enable or Disable Global unicast packet detection in power down mode */
    uint32_t WakeUpForward; /*!< Enable or Disable Forwarding Wake up packets */
} ETH_WOLConfigTypeDef;
/**
 *
 */

/**
 * @brief  ETH Handle Structure definition
 * \ingroup eth_struct_define
 */

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
typedef struct __ETH_Handle_T
#else
typedef struct
#endif /* configUSE_ETH_REGISTER_CALLBACKS */
{
    SN_ETHMAC_Type           *Instance;          /*!< Register base address       */
    ETH_Init_T                Init;              /*!< Ethernet Init Configuration */
    uint32_t                  LinkStatus;        /*!< Ethernet link status        */
    ETH_DMADescTypeDef       *RxDesc;            /*!< Rx descriptor to Get        */
    ETH_DMADescTypeDef       *RxDMADescBaseAddr; /*!< Rx descriptor Base address        */
    ETH_DMADescTypeDef       *TxDesc;            /*!< Tx descriptor to Set        */
    ETH_DMADescTypeDef       *TxDMADescBaseAddr; /*!< Tx descriptor Base address        */
    ETH_DMARxFrameInfos       RxFrameInfos;      /*!< last Rx frame infos         */
    __IO HAL_ETH_StateTypeDef State;             /*!< ETH communication state     */

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)

    void (*TxCpltCallback)(struct __ETH_Handle_T *heth);    /*!< ETH Tx Complete Callback   */
    void (*RxCpltCallback)(struct __ETH_Handle_T *heth);    /*!< ETH Rx  Complete Callback   */
    void (*DMAErrorCallback)(struct __ETH_Handle_T *heth);  /*!< DMA Error Callback      */
    void (*MspInitCallback)(struct __ETH_Handle_T *heth);   /*!< ETH Msp Init callback       */
    void (*MspDeInitCallback)(struct __ETH_Handle_T *heth); /*!< ETH Msp DeInit callback     */
    void (*WakeUpCallback)(struct __ETH_Handle_T *heth);    /*!< ETH Wake UP Callback   */
    void (*LinkChgCallback)(struct __ETH_Handle_T *heth);   /*!< ETH Link Change Callback   */

#endif /* configUSE_ETH_REGISTER_CALLBACKS */

} ETH_Handle_T;

/**
 * @brief  HAL ETH Callback ID enumeration definition
 * \ingroup eth_struct_define
 */
typedef enum
{
    HAL_ETH_MSPINIT_CB_ID     = 0x00U, /*!< ETH MspInit callback ID            */
    HAL_ETH_MSPDEINIT_CB_ID   = 0x01U, /*!< ETH MspDeInit callback ID          */
    HAL_ETH_TX_COMPLETE_CB_ID = 0x02U, /*!< ETH Tx Complete Callback ID        */
    HAL_ETH_RX_COMPLETE_CB_ID = 0x03U, /*!< ETH Rx Complete Callback ID        */
    HAL_ETH_DMA_ERROR_CB_ID   = 0x04U, /*!< ETH DMA Error Callback ID          */
    HAL_ETH_WAKEUP_CB_ID      = 0x05U, /*!< ETH Wake UP Callback ID            */
    HAL_ETH_LINKCHG_CB_ID     = 0x06U  /*!< ETH Wake UP Callback ID            */
} HAL_ETH_CallbackIDTypeDef;

/**
 * @brief  HAL ETH Callback pointer definition
 * \ingroup eth_control
 */
typedef void (*pETH_CallbackTypeDef)(ETH_Handle_T *heth); /*!< pointer to an ETH callback function */

#define ETH_MAX_PACKET_SIZE     1520U /*!< ETH_HEADER + ETH_EXTRA + ETH_VLAN_TAG + ETH_MAX_ETH_PAYLOAD + ETH_CRC and it must be 8 alignment*/
#define ETH_HEADER              14U   /*!< 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define ETH_CRC                 4U    /*!< Ethernet CRC */
#define ETH_EXTRA               2U    /*!< Extra bytes in some cases */
#define ETH_VLAN_TAG            4U    /*!< optional 802.1q VLAN Tag */
#define ETH_MIN_ETH_PAYLOAD     46U   /*!< Minimum Ethernet payload size */
#define ETH_MAX_ETH_PAYLOAD     1500U /*!< Maximum Ethernet payload size */
#define ETH_JUMBO_FRAME_PAYLOAD 9000U /*!< Jumbo frame payload size */

#define ETH_RXDES0_EDORR (1 << 15) /*!< Reserved */

/* Ethernet driver receive buffers are organized in a chained linked-list, when
   an ethernet packet is received, the Rx-DMA will transfer the packet from RxFIFO
   to the driver receive buffers memory.

   Depending on the size of the received ethernet packet and the size of
   each ethernet driver receive buffer, the received packet can take one or more
   ethernet driver receive buffer.

   In below are defined the size of one ethernet driver receive buffer ETH_RX_BUF_SIZE
   and the total count of the driver receive buffers ETH_RXBUFNB.

   The configured value for ETH_RX_BUF_SIZE and ETH_RXBUFNB are only provided as
   example, they can be reconfigured in the application layer to fit the application
   needs */

/* Here we configure each Ethernet driver receive buffer to fit the Max size Ethernet
   packet */
#ifndef ETH_RX_BUF_SIZE
#define ETH_RX_BUF_SIZE ETH_MAX_PACKET_SIZE
#endif

/* 5 Ethernet driver receive buffers are used (in a chained linked list)*/
#ifndef ETH_RXBUFNB
#define ETH_RXBUFNB 4U /*  8 Rx buffers of size ETH_RX_BUF_SIZE */
#endif

/* Ethernet driver transmit buffers are organized in a chained linked-list, when
   an ethernet packet is transmitted, Tx-DMA will transfer the packet from the
   driver transmit buffers memory to the TxFIFO.

   Depending on the size of the Ethernet packet to be transmitted and the size of
   each ethernet driver transmit buffer, the packet to be transmitted can take
   one or more ethernet driver transmit buffer.

   In below are defined the size of one ethernet driver transmit buffer ETH_TX_BUF_SIZE
   and the total count of the driver transmit buffers ETH_TXBUFNB.

   The configured value for ETH_TX_BUF_SIZE and ETH_TXBUFNB are only provided as
   example, they can be reconfigured in the application layer to fit the application
   needs */

/* Here we configure each Ethernet driver transmit buffer to fit the Max size Ethernet
   packet */
#ifndef ETH_TX_BUF_SIZE
#define ETH_TX_BUF_SIZE ETH_MAX_PACKET_SIZE
#endif

/* 5 ethernet driver transmit buffers are used (in a chained linked list)*/
#ifndef ETH_TXBUFNB
#define ETH_TXBUFNB 4U /* 8  Tx buffers of size ETH_TX_BUF_SIZE */
#endif

/**
 * @brief Generic MII registers definition
 * \ingroup eth_control
 */
#define PHY_BCR              0x00 /*!<  Basic mode control register */
#define PHY_BSR              0x01 /*!<  Basic mode status register  */
#define PHY_PHYSID1          0x02 /*!<  PHYS ID 1                   */
#define PHY_PHYSID2          0x03 /*!<  PHYS ID 2                   */
#define PHY_ADVERTISE        0x04 /*!<  Advertisement control reg   */
#define PHY_LPA              0x05 /*!<  Link partner ability reg    */
#define PHY_EXPANSION        0x06 /*!<  Expansion register          */
#define PHY_CTRL1000         0x09 /*!<  1000BASE-T control          */
#define PHY_STAT1000         0x0a /*!<  1000BASE-T status           */
#define PHY_MMD_CTRL         0x0d /*!<  MMD Access Control Register */
#define PHY_MMD_DATA         0x0e /*!<  MMD Access Data Register */
#define PHY_ESTATUS          0x0f /*!<  Extended Status             */
#define PHY_DSCR             0x10 /*!<  DAVICOM Specified Configuration Register  */
#define PHY_DAVICOM_SPEC_INT 0x15 /*!<   DAVICOM Specified Interrupt Register �V 21  */
#define PHY_DCOUNTER         0x12 /*!<  Disconnect counter          */
#define PHY_FCSCOUNTER       0x13 /*!<  False carrier counter       */
#define PHY_NWAYTEST         0x14 /*!<  N-way auto-neg test reg     */
#define PHY_RERRCOUNTER      0x15 /*!<  Receive error counter       */
#define PHY_SREVISION        0x16 /*!<  Silicon revision            */
#define PHY_RESV1            0x17 /*!<  Reserved...                 */
#define PHY_LBRERROR         0x18 /*!<  Lpback, rx, bypass error    */
#define PHY_PHYADDR          0x19 /*!<  PHY address                 */
#define PHY_RESV2            0x1a /*!<  Reserved...                 */
#define PHY_TPISTATUS        0x1b /*!<  TPI status for 10mbps       */
#define PHY_NCONFIG          0x1c /*!<  Network interface config    */

/**
 * @brief Basic mode control register definition
 * \ingroup eth_control
 */
#define BCR_RESV      0x003f /*!<  Unused...                   */
#define BCR_SPEED1000 0x0040 /*!<  MSB of Speed (1000)         */
#define BCR_CTST      0x0080 /*!<  Collision test              */
#define BCR_FULLDPLX  0x0100 /*!<  Full duplex                 */
#define BCR_ANRESTART 0x0200 /*!<  Auto negotiation restart    */
#define BCR_ISOLATE   0x0400 /*!<  Isolate data paths from MII */
#define BCR_PDOWN     0x0800 /*!<  Enable low power state      */
#define BCR_ANENABLE  0x1000 /*!<  Enable auto negotiation     */
#define BCR_SPEED100  0x2000 /*!<  Select 100Mbps              */
#define BCR_LOOPBACK  0x4000 /*!<  TXD loopback bits           */
#define BCR_RESET     0x8000 /*!<  Reset to default state      */

/**
 * @brief Basic mode status register definition
 * \ingroup eth_control
 */
#define BSR_ERCAP        0x0001 /*!<  Ext-reg capability          */
#define BSR_JCD          0x0002 /*!<  Jabber detected             */
#define BSR_LSTATUS      0x0004 /*!<  Link status                 */
#define BSR_ANEGCAPABLE  0x0008 /*!<  Able to do auto-negotiation */
#define BSR_RFAULT       0x0010 /*!<  Remote fault detected       */
#define BSR_ANEGCOMPLETE 0x0020 /*!<  Auto-negotiation complete   */
#define BSR_RESV         0x00c0 /*!<  Unused...                   */
#define BSR_ESTATEN      0x0100 /*!<  Extended Status in R15      */
#define BSR_100HALF2     0x0200 /*!<  Can do 100BASE-T2 HDX       */
#define BSR_100FULL2     0x0400 /*!<  Can do 100BASE-T2 FDX       */
#define BSR_10HALF       0x0800 /*!<  Can do 10mbps, half-duplex  */
#define BSR_10FULL       0x1000 /*!<  Can do 10mbps, full-duplex  */
#define BSR_100HALF      0x2000 /*!<  Can do 100mbps, half-duplex */
#define BSR_100FULL      0x4000 /*!<  Can do 100mbps, full-duplex */
#define BSR_100BASE4     0x8000 /*!<  Can do 100mbps, 4k packets  */

/**
 * @brief Advertisement control register definition
 * \ingroup eth_control
 */
#define ADVERTISE_SLCT          0x001f                                                                        /*!<  Selector bits               */
#define ADVERTISE_CSMA          0x0001                                                                        /*!<  Only selector supported     */
#define ADVERTISE_10HALF        0x0020                                                                        /*!<  Try for 10mbps half-duplex  */
#define ADVERTISE_1000XFULL     0x0020                                                                        /*!<  Try for 1000BASE-X full-duplex */
#define ADVERTISE_10FULL        0x0040                                                                        /*!<  Try for 10mbps full-duplex  */
#define ADVERTISE_1000XHALF     0x0040                                                                        /*!<  Try for 1000BASE-X half-duplex */
#define ADVERTISE_100HALF       0x0080                                                                        /*!<  Try for 100mbps half-duplex */
#define ADVERTISE_1000XPAUSE    0x0080                                                                        /*!<  Try for 1000BASE-X pause    */
#define ADVERTISE_100FULL       0x0100                                                                        /*!<  Try for 100mbps full-duplex */
#define ADVERTISE_1000XPSE_ASYM 0x0100                                                                        /*!<  Try for 1000BASE-X asym pause */
#define ADVERTISE_100BASE4      0x0200                                                                        /*!<  Try for 100mbps 4k packets  */
#define ADVERTISE_PAUSE_CAP     0x0400                                                                        /*!<  Try for pause               */
#define ADVERTISE_PAUSE_ASYM    0x0800                                                                        /*!<  Try for asymetric pause     */
#define ADVERTISE_RESV          0x1000                                                                        /*!<  Unused...                   */
#define ADVERTISE_RFAULT        0x2000                                                                        /*!<  Say we can detect faults    */
#define ADVERTISE_LPACK         0x4000                                                                        /*!<  Ack link partners response  */
#define ADVERTISE_NPAGE         0x8000                                                                        /*!<  Next page bit               */
#define ADVERTISE_FULL          (ADVERTISE_100FULL | ADVERTISE_10FULL | ADVERTISE_CSMA)                       /*!< Full */
#define ADVERTISE_ALL           (ADVERTISE_10HALF | ADVERTISE_10FULL | ADVERTISE_100HALF | ADVERTISE_100FULL) /*!< All */

/**
 * @brief Link partner ability register definition
 * \ingroup eth_control
 */
#define LPA_SLCT            0x001f /*!<  Same as advertise selector  */
#define LPA_10HALF          0x0020 /*!<  Can do 10mbps half-duplex   */
#define LPA_1000XFULL       0x0020 /*!<  Can do 1000BASE-X full-duplex */
#define LPA_10FULL          0x0040 /*!<  Can do 10mbps full-duplex   */
#define LPA_1000XHALF       0x0040 /*!<  Can do 1000BASE-X half-duplex */
#define LPA_100HALF         0x0080 /*!<  Can do 100mbps half-duplex  */
#define LPA_1000XPAUSE      0x0080 /*!<  Can do 1000BASE-X pause     */
#define LPA_100FULL         0x0100 /*!<  Can do 100mbps full-duplex  */
#define LPA_1000XPAUSE_ASYM 0x0100 /*!<  Can do 1000BASE-X pause asym*/
#define LPA_100BASE4        0x0200 /*!<  Can do 100mbps 4k packets   */
#define LPA_PAUSE_CAP       0x0400 /*!<  Can pause                   */
#define LPA_PAUSE_ASYM      0x0800 /*!<  Can pause asymetrically     */
#define LPA_RESV            0x1000 /*!<  Unused...                   */
#define LPA_RFAULT          0x2000 /*!<  Link partner faulted        */
#define LPA_LPACK           0x4000 /*!<  Link partner acked us       */
#define LPA_NPAGE           0x8000 /*!<  Next page bit               */

/**
 * @brief Expansion register for auto-negotiation
 * \ingroup eth_control
 */
#define EXPANSION_NWAY        0x0001 /*!<  Can do N-way auto-nego      */
#define EXPANSION_LCWP        0x0002 /*!<  Got new RX page code word   */
#define EXPANSION_ENABLENPAGE 0x0004 /*!<  This enables npage words    */
#define EXPANSION_NPCAPABLE   0x0008 /*!<  Link partner supports npage */
#define EXPANSION_MFAULTS     0x0010 /*!<  Multiple faults detected    */
#define EXPANSION_RESV        0xffe0 /*!<  Unused...                   */

#define ESTATUS_1000_TFULL 0x2000 /*!<  Can do 1000BT Full          */
#define ESTATUS_1000_THALF 0x1000 /*!<  Can do 1000BT Half          */

/**
 * @brief N-way test register
 * \ingroup eth_control
 */
#define NWAYTEST_RESV1    0x00ff /*!<  Unused...                   */
#define NWAYTEST_LOOPBACK 0x0100 /*!<  Enable loopback for N-way   */
#define NWAYTEST_RESV2    0xfe00 /*!<  Unused...                   */

/**
 * @brief 1000BASE-T Control register
 * \ingroup eth_control
 */
#define ADVERTISE_1000FULL    0x0200 /*!<  Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF    0x0100 /*!<  Advertise 1000BASE-T half duplex */
#define CTL1000_AS_MASTER     0x0800 /*!< Reserved */
#define CTL1000_ENABLE_MASTER 0x1000 /*!< Reserved */

/**
 * @brief 1000BASE-T Status register
 * \ingroup eth_control
 */
#define LPA_1000LOCALRXOK 0x2000 /*!<  Link partner local receiver status */
#define LPA_1000REMRXOK   0x1000 /*!<  Link partner remote receiver status */
#define LPA_1000FULL      0x0800 /*!<  Link partner 1000BASE-T full duplex */
#define LPA_1000HALF      0x0400 /*!<  Link partner 1000BASE-T half duplex */

#define LPA_DUPLEX (LPA_10FULL | LPA_100FULL)                 /*!< Reserved */
#define LPA_100    (LPA_100FULL | LPA_100HALF | LPA_100BASE4) /*!< Reserved */

/**
 * @brief DAVICOM Specified Configuration Register (DSCR) foe DM9162B- 16
 * \ingroup eth_control
 */
#define RMII_ENABLE 0x0100 /*!< Reduced MII Enable */

/**
 * @brief Initialize parameter define
 * \ingroup eth_control
 */
#define ETH_AUTONEGOTIATION_ENABLE  0x00000001U /*!< Autonegotiation enable */
#define ETH_AUTONEGOTIATION_DISABLE 0x00000000U /*!< Autonegotiation disable */

#define ETH_SPEED_10M  0x00000000U /*!< Eth speed 10 M */
#define ETH_SPEED_100M 0x00004000U /*!< Eth speed 100 M */

#define ETH_MODE_FULLDUPLEX 0x00000800U /*!< Eth full duplex mode */
#define ETH_MODE_HALFDUPLEX 0x00000000U /*!< Eth half duplex mode */

#define ETH_CHECKSUM_BY_HARDWARE 0x00000000U /*!< Eth checksum by hardware */
#define ETH_CHECKSUM_BY_SOFTWARE 0x00000001U /*!< Eth checksum by software */

#define ETH_MEDIA_INTERFACE_MII  0x00000000U /*!< Eth MII intrface */
#define ETH_MEDIA_INTERFACE_RMII 0x00000001U /*!< Eth RMII intrface */

#define ETH_LOOPBACKMODE_ENABLE  0x00001000U /*!< Eth loopback mode enable */
#define ETH_LOOPBACKMODE_DISABLE 0x00000000U /*!< Eth loopback mode disable */

#define ETH_CHECKSUMOFFLAOD_ENABLE  0x00000400U /*!< Eth checksum offload enable */
#define ETH_CHECKSUMOFFLAOD_DISABLE 0x00000000U /*!< Eth checksum offload disable */

//
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions  ****************************/

HAL_Status_T HAL_ETH_Init(ETH_Handle_T *heth);
HAL_Status_T HAL_ETH_DeInit(ETH_Handle_T *heth);
void         HAL_ETH_MspInit(ETH_Handle_T *heth);
void         HAL_ETH_MspDeInit(ETH_Handle_T *heth);
HAL_Status_T HAL_ETH_DMATxDescListInit(ETH_Handle_T *heth, ETH_DMADescTypeDef *DMATxDescTab, uint8_t *TxBuff, uint32_t TxBuffCount);
HAL_Status_T HAL_ETH_DMARxDescListInit(ETH_Handle_T *heth, ETH_DMADescTypeDef *DMARxDescTab, uint8_t *RxBuff, uint32_t RxBuffCount);
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_Status_T HAL_ETH_RegisterCallback(ETH_Handle_T *heth, HAL_ETH_CallbackIDTypeDef CallbackID, pETH_CallbackTypeDef pCallback);
HAL_Status_T HAL_ETH_UnRegisterCallback(ETH_Handle_T *heth, HAL_ETH_CallbackIDTypeDef CallbackID);

/* IO operation functions  ****************************************************/

HAL_Status_T HAL_ETH_TransmitFrame(ETH_Handle_T *heth, uint32_t FrameLength);
HAL_Status_T HAL_ETH_GetReceivedFrame(ETH_Handle_T *heth);
/* Communication with PHY functions*/
HAL_Status_T HAL_ETH_ReadPHYRegister(ETH_Handle_T *heth, uint16_t PHYReg, uint16_t *RegValue);
HAL_Status_T HAL_ETH_WritePHYRegister(ETH_Handle_T *heth, uint16_t PHYReg, uint16_t RegValue);
/* Non-Blocking mode: Interrupt */
HAL_Status_T HAL_ETH_GetReceivedFrame_IT(ETH_Handle_T *heth);
void         HAL_ETH_IRQHandler(ETH_Handle_T *heth);
void         HAL_ETH_WOL_IRQHandler(ETH_Handle_T *heth);
/* Callback in non blocking modes (Interrupt) */
void HAL_ETH_TxCpltCallback(ETH_Handle_T *heth);
void HAL_ETH_RxCpltCallback(ETH_Handle_T *heth);
void HAL_ETH_ErrorCallback(ETH_Handle_T *heth);
void HAL_ETH_WakeUpCallback(ETH_Handle_T *heth);
void HAL_ETH_LinkChgCallback(ETH_Handle_T *heth);

/* Peripheral Control functions  **********************************************/

HAL_Status_T HAL_ETH_Start(ETH_Handle_T *heth);
HAL_Status_T HAL_ETH_Stop(ETH_Handle_T *heth);
HAL_Status_T HAL_ETH_ConfigMAC(ETH_Handle_T *heth);
void         HAL_ETH_EnterPowerDownMode(ETH_Handle_T *heth, ETH_PowerDownConfigTypeDef *pPowerDownConfig);
void         HAL_ETH_ExitPowerDownMode(ETH_Handle_T *heth);
HAL_Status_T HAL_ETH_SetWakeUpFilter(ETH_Handle_T *heth, ETH_PowerDownConfigTypeDef *pPowerDownConfig);

/* Peripheral State functions  ************************************************/

HAL_ETH_StateTypeDef HAL_ETH_GetState(ETH_Handle_T *heth);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_ETH_H_ */
