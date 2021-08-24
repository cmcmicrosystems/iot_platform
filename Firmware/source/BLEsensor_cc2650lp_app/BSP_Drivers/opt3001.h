/*
 * opt3001.h
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */



#ifndef BSP_DRIVERS_OPT3001_H_
#define BSP_DRIVERS_OPT3001_H_
#include <ti/drivers/PIN.h>
#include "ti/drivers/I2C.h"
#define OPT3001_DEV_ADDRESS         0x44


//defining registers
#define OPT3001_RESULT_ADDR         0x00
#define OPT3001_CONFIG_ADDR         0x01
#define OPT3001_LOW_LIMIT_ADDR      0x02
#define OPT3001_HIGH_LIMIT_ADDR     0x03


//CONFIG register default values
#define OPT3001_M                   ((uint16_t) 0x000C << 12)        //Automatic range selection
#define OPT3001_CT                  ((uint16_t) 0x0001 << 11)        //800ms exposure time
#define OPT3001_MODE_SHUTDOWN       ((uint16_t) 0x0000 << 9)
#define OPT3001_MODE_SINGLE         ((uint16_t) 0x0001 << 9)
#define OPT3001_MODE_CONTINIOUS     ((uint16_t) 0x0002 << 9)

//Low Limit register default value
#define OPT3001_INT_MODE            ((uint16_t) 0xC000 )    //End of conversion mode interrupt

//Config register bit mask
#define CFG_BM_CRF          (1<<7)

//Exported functions
void OPT3001_initialize();
void OPT3001_startMeasureContinious();
void OPT3001_stopMeasure();
double OPT3001_getCurrnetLux();
void OPT3001_intHandler();

#endif /* BSP_DRIVERS_OPT3001_H_ */
