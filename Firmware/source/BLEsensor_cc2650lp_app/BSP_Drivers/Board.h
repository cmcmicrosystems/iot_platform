/*
 * board.h
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */

#ifndef STARTUP_BOARD_H_
#define STARTUP_BOARD_H_



#ifdef __cplusplus
extern "C" {
#endif

#include <ti/drivers/Power.h>

#include "bspBoard.h"

/* These #defines allow us to reuse TI-RTOS across other device families */
#define     Board_LED0              Board_RLED
//#define     Board_LED1              Board_GLED
//#define     Board_LED2              Board_LED0
//
//#define     Board_BUTTON0           Board_BTN1
//#define     Board_BUTTON1           Board_BTN2

//#define     Board_UART0             Board_UART
#define     Board_AES0              Board_AES
//#define     Board_WATCHDOG0         Board_WATCHDOG

#define     Board_initGeneral() { \
    Power_init(); \
    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) \
        {System_abort("Error with PIN_init\n"); \
    } \
}

#define     Board_initGPIO()
#define     Board_initSPI()         SPI_init()
//#define     Board_initUART()        UART_init()
//#define     Board_initWatchdog()    Watchdog_init()
#define     GPIO_toggle(n)
#define     GPIO_write(n,m)

#ifdef __cplusplus
}
#endif




#endif /* STARTUP_BOARD_H_ */
