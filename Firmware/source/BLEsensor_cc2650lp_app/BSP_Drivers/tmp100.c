/*
 * tmp100.c
 *
 *  Created on: July, 2021
 *      Author: Olive Zhao
 */

#include "Board.h"
#include <ti/drivers/PIN.h>
#include <tmp100.h>

//define cmd address for 2 byte
//uint8_t cmd1[2]={0x84,0xB8};
//uint8_t cmd2[2]={0xFA,0x0F};
//uint8_t cmd3[3]={0xFC,0xC9};


//static int TemperatureSum;

static I2C_Handle hi2c_tmp;


static uint8_t txBuffer_tmp[4];
static uint8_t rxBuffer_tmp[4];
//functions

//static void User_data();

static uint16_t TMP100_readRegister(uint8_t reg_name,uint8_t length);
static void TMP100_writeRegister(uint8_t cmd, uint8_t value);

void TMP100_init(I2C_Handle *h)
{
    hi2c_tmp=*h;

    uint8_t ret;

    I2C_Transaction i2cTransaction;


    txBuffer_tmp[0] = TMP100_CONF_REG;

    i2cTransaction.writeBuf = txBuffer_tmp;
    i2cTransaction.writeCount = 1;

    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    i2cTransaction.slaveAddress = TMP100_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_tmp, &i2cTransaction);
    if (!ret) {
         //      System_printf("Unsuccessful I2C transfer");
    }

    txBuffer_tmp[0] = TMP100_12bits;

    i2cTransaction.writeBuf = txBuffer_tmp;
    i2cTransaction.writeCount = 1;

    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    i2cTransaction.slaveAddress = TMP100_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_tmp, &i2cTransaction);
    if (!ret) {
         //      System_printf("Unsuccessful I2C transfer");
    }


}

double TMP100_getCurrentTemp()
{
   // int t;

    uint16_t temp_code;
    temp_code = TMP100_readRegister(TMP100_TEMP_REG, LENGTH);

    return temp_code * RESOLUTION;

}

static uint16_t TMP100_readRegister(uint8_t reg_name,uint8_t length)
{
    uint8_t ret;
    I2C_Transaction i2cTransaction;

    txBuffer_tmp[0] = reg_name;

    i2cTransaction.writeBuf = txBuffer_tmp;
    i2cTransaction.writeCount = 1;

    i2cTransaction.readBuf = rxBuffer_tmp;
    i2cTransaction.readCount = length;

    i2cTransaction.slaveAddress = TMP100_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_tmp, &i2cTransaction);
    if (!ret) {
        //System_printf("Unsuccessful I2C transfer");

    }else {
        if (length == 1){
            return (uint16_t)rxBuffer_tmp[0];
        }else if (length == 2) {
            return ((uint16_t)(((rxBuffer_tmp[0] << 8) | rxBuffer_tmp[1]) >> 4));
        }
    }
return 0;
}


static void TMP100_writeRegister(uint8_t cmd, uint8_t value)
{
    uint8_t ret;
    I2C_Transaction i2cTransaction;

    txBuffer_tmp[0] = cmd;
    txBuffer_tmp[1] = (uint8_t) value ;

    i2cTransaction.writeBuf = txBuffer_tmp;
    i2cTransaction.writeCount = 2;

    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    i2cTransaction.slaveAddress = TMP100_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_tmp, &i2cTransaction);
    if (!ret) {
         //      System_printf("Unsuccessful I2C transfer");
    }
}



/*
void Firmware_version(){
    TMP100_readRegister(cmd1,1);
}
*/

/*
uint32_t IDdata(uint32_t);

void readElectronicID(){
    //uint8_t data_1[4],data_2[4];
   // uint32_t data_1,data_2;
    data_1=TMP100_readRegister(cmd2,4);
    data_2=TMP100_readRegister(cmd3,4);
    IDdata(data_1);
    IDdata(data_2);
}

uint32_t IDdata(uint32_t data){

    return data;
}
*/




