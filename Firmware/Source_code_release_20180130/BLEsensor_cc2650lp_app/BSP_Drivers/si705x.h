/*
 * si705x.h
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */

#ifndef BSP_DRIVERS_SI705X_H_
#define BSP_DRIVERS_SI705X_H_
#include <ti/drivers/PIN.h>
#include "ti/drivers/I2C.h"
#define SI705X_DEV_ADDRESS  0x40



//defining commands
#define HOLD_MASTER_M       0xE3
#define NOHOLD_MASTER_M     0xF3
#define RESET               0xFE
#define WUSER_REG           0xE6
#define RUSER_REG           0xE7


//defining fucn
void SI705X_init(I2C_Handle *h);
double SI705X_getCurrentTemp();
void readElectronicID();
void Firmware_version();







#endif /* BSP_DRIVERS_SI705X_H_ */
