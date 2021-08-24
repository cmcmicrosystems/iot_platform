/*
 * tmp100.h
 *
 *  Created on: July, 2021
 *      Author: Olive Zhao
 */

#ifndef BSP_DRIVERS_TMP100_H_
#define BSP_DRIVERS_TMP100_H_
#include <ti/drivers/PIN.h>
#include "ti/drivers/I2C.h"


//defining commands
//#define HOLD_MASTER_M       0xE3
//#define NOHOLD_MASTER_M     0xF3
//#define RESET               0xFE
//#define WUSER_REG           0xE6
//#define RUSER_REG           0xE7


// define for TMP100
#define TMP100_DEV_ADDRESS  0x48


#define TMP100_normal 0x00
#define TMP100_shutdown 0x01

#define TMP100_9bits 0x00
#define TMP100_10bits 0x01
#define TMP100_11bits 0x03
#define TMP100_12bits 0x04

#define TMP100_TEMP_REG 0x00
#define TMP100_CONF_REG 0x01
#define TMP100_TLOW_REG 0x02
#define TMP100_THIGH_REG 0x03

#define LENGTH 2
#define RESOLUTION 0.0625

// define funcs
void TMP100_init(I2C_Handle *h);
double TMP100_getCurrentTemp();

//void readElectronicID();
//void Firmware_version();

#endif /* BSP_DRIVERS_TMP100_H_ */
