/*
 * adxl362.h
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */

#ifndef BSP_DRIVERS_ADXL362_H_
#define BSP_DRIVERS_ADXL362_H_

#include "Board.h"
#include <ti/drivers/SPI.h>


//commands
#define WRITE_CMD       0x0A
#define READ_CMD        0x0B
#define READ_FIFO       0x0D


//ADDRESS OF REGSITERS
#define DEVID_AD        0x00
#define DEVID_MST       0x01
#define PARTID          0x02
#define REVID           0x03
#define XDATA           0x08
#define YDATA           0x09
#define ZDATA           0x0A
#define STATUS          0x0B
#define FIFO_ENTRIES_L  0x0C
#define FIFO_ENTRIES_H  0x0D
#define XDATA_L         0X0E
#define XDATA_H         0X0F
#define YDATA_L         0X10
#define YDATA_H         0X11
#define ZDATA_L         0X12
#define ZDATA_H         0X13
#define TEMP_L          0x14
#define TEMP_H          0x15

#define SOFT_RESET      0x1F
#define THRESH_ACT_L    0x20
#define THRESH_ACT_H    0x21
#define TIME_ACT        0x22
#define THRESH_INACT_L  0x23
#define THRESH_INACT_H  0x24
#define TIME_INACT_L    0x25
#define TIME_INACT_H    0x26
#define ACT_INACT_CTL   0x27
#define FIFO_CONTROL    0x28
#define FIFO_SAMPLES    0x29
#define INTMAP1         0x2A
#define INTMAP2         0x2B
#define FILTER_CTL      0x2C
#define POWER_CTL       0x2D
#define SELF_TEST       0x2E


//Status register bit mask
#define ERR_FLAG        0x80
#define AWAKE_FLAG      0x40
#define INACT_FLAG      0x20
#define ACT_FLAG        0x10
#define FIFO_OVR_FLAG   0x08
#define FIFO_WM_FLAG    0x04
#define FIFO_RDY_FLAG   0x02
#define DATA_READY_FLAG 0x01




//defined fucn
void ADXL362_initialize(SPI_Handle *handle);
void get_accleration_msb();
void get_device_data();
void get_accleration();
uint8_t xl362readFIFO(uint8_t *buffer, uint8_t v_length);
uint8_t adxl362GetStatus();




#endif /* BSP_DRIVERS_ADXL362_H_ */
