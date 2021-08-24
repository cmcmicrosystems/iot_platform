/*
 * bspBoard.h
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */

#ifndef BSP_DRIVERS_BSPBOARD_H_
#define BSP_DRIVERS_BSPBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

/** ============================================================================
 *  Includes
 *  ==========================================================================*/
#include <ti/drivers/PIN.h>
#include <driverlib/ioc.h>

/** ============================================================================
 *  Externs
 *  ==========================================================================*/
extern const PIN_Config BoardGpioInitTable[];

/** ============================================================================
 *  Defines
 *  ==========================================================================*/

/* Same RF Configuration as 7x7 EM */
#define CC2650EM_7ID

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>        <pin mapping>
 */

/* Discrete outputs */
#define Board_RLED                  IOID_5
#define Board_LED_ON                0
#define Board_LED_OFF               1

/* Discrete inputs */
//#define Board_BTN1                  IOID_13
//#define Board_BTN2                  IOID_14

/* UART Board */
//#define Board_UART_RX               IOID_2          /* RXD  */
//#define Board_UART_TX               IOID_3          /* TXD  */
//#define Board_UART_CTS              PIN_UNASSIGNED         /* CTS  */
//#define Board_UART_RTS              PIN_UNASSIGNED         /* RTS */

/* SPI Board */
#define Board_SPI0_MISO             IOID_7          /* RF1.20 */
#define Board_SPI0_MOSI             IOID_8          /* RF1.18 */
#define Board_SPI0_CLK              IOID_9         /* RF1.16 */
#define Board_SPI0_CSN              PIN_UNASSIGNED
//#define Board_SPI1_MISO             PIN_UNASSIGNED
//#define Board_SPI1_MOSI             PIN_UNASSIGNED
//#define Board_SPI1_CLK              PIN_UNASSIGNED
//#define Board_SPI1_CSN              PIN_UNASSIGNED

/* I2C */
#define Board_I2C0_SCL0             IOID_0
#define Board_I2C0_SDA0             IOID_1

/* SPI */
#define Board_SPI_ADX_CS          IOID_6
#define Board_ADX_CS_ON           0
#define Board_ADX_CS_OFF          1

#define Board_OPT3001_INT        IOID_2
#define Board_ADX_INT1           IOID_3
#define Board_ADX_INT2           IOID_4



/* Booster pack generic */
//#define Board_DIO0                  IOID_0
//#define Board_DIO1_RFSW             IOID_1
//#define Board_DIO12                 IOID_12
//#define Board_DIO15                 IOID_15
//#define Board_DIO16_TDO             IOID_16
//#define Board_DIO17_TDI             IOID_17
//#define Board_DIO21                 IOID_21
//#define Board_DIO22                 IOID_22
//
//#define Board_DIO23_ANALOG          IOID_23
//#define Board_DIO24_ANALOG          IOID_24
//#define Board_DIO25_ANALOG          IOID_25
//#define Board_DIO26_ANALOG          IOID_26
//#define Board_DIO27_ANALOG          IOID_27
//#define Board_DIO28_ANALOG          IOID_28
//#define Board_DIO29_ANALOG          IOID_29
//#define Board_DIO30_ANALOG          IOID_30

/* Booster pack LCD (430BOOST - Sharp96 Rev 1.1) */
//#define Board_LCD_CS                IOID_24 // SPI chip select
//#define Board_LCD_EXTCOMIN          IOID_12 // External COM inversion
//#define Board_LCD_ENABLE            IOID_22 // LCD enable
//#define Board_LCD_POWER             IOID_23 // LCD power control
//#define Board_LCD_CS_ON             1
//#define Board_LCD_CS_OFF            0

/** ============================================================================
 *  Instance identifiers
 *  ==========================================================================*/
/* Generic I2C instance identifiers */
#define Board_I2C                   CC2650_LAUNCHXL_I2C0
/* Generic SPI instance identifiers */
#define Board_SPI                  CC2650_LAUNCHXL_SPI0
//#define Board_SPI1                  CC2650_LAUNCHXL_SPI1
/* Generic UART instance identifiers */
//#define Board_UART                  CC2650_LAUNCHXL_UART0


/** ============================================================================
 *  Number of peripherals and their names
 *  ==========================================================================*/

/*!
 *  @def    CC2650_LAUNCHXL_I2CName
 *  @brief  Enum of I2C names on the CC2650 dev board
 */
typedef enum CC2650_LAUNCHXL_I2CName {
    CC2650_LAUNCHXL_I2C0 = 0,

    CC2650_LAUNCHXL_I2CCOUNT
} CC2650_LAUNCHXL_I2CName;

/*!
 *  @def    CC2650_LAUNCHXL_CryptoName
 *  @brief  Enum of Crypto names on the CC2650 dev board
 */
typedef enum CC2650_LAUNCHXL_CryptoName {
    CC2650_LAUNCHXL_CRYPTO0 = 0,

    CC2650_LAUNCHXL_CRYPTOCOUNT
} CC2650_LAUNCHXL_CryptoName;


/*!
 *  @def    CC2650_LAUNCHXL_SPIName
 *  @brief  Enum of SPI names on the CC2650 dev board
 */
typedef enum CC2650_LAUNCHXL_SPIName {
    CC2650_LAUNCHXL_SPI0 = 0,
//    CC2650_LAUNCHXL_SPI1,

    CC2650_LAUNCHXL_SPICOUNT
} CC2650_LAUNCHXL_SPIName;

/*!
 *  @def    CC2650_LAUNCHXL_UARTName
 *  @brief  Enum of UARTs on the CC2650 dev board
 */
//typedef enum CC2650_LAUNCHXL_UARTName {
//    CC2650_LAUNCHXL_UART0 = 0,
//
//    CC2650_LAUNCHXL_UARTCOUNT
//} CC2650_LAUNCHXL_UARTName;

/*!
 *  @def    CC2650_LAUNCHXL_UdmaName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC2650_LAUNCHXL_UdmaName {
    CC2650_LAUNCHXL_UDMA0 = 0,

    CC2650_LAUNCHXL_UDMACOUNT
} CC2650_LAUNCHXL_UdmaName;



/*!
 *  @def    CC2650_LAUNCHXL_TRNGName
 *  @brief  Enum of TRNG names on the board
 */
typedef enum CC2650_LAUNCHXL_TRNGName {
    CC2650_LAUNCHXL_TRNG0 = 0,
    CC2650_LAUNCHXL_TRNGCOUNT
} CC2650_LAUNCHXL_TRNGName;


#ifdef __cplusplus
}
#endif



#endif /* BSP_DRIVERS_BSPBOARD_H_ */
