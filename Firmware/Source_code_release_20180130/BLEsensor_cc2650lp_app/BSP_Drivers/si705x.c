/*
 * si705x.c
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */

#include "si705x.h"
#include "Board.h"
#include <ti/drivers/PIN.h>

//define cmd address for 2 byte
uint8_t cmd1[2]={0x84,0xB8};
uint8_t cmd2[2]={0xFA,0x0F};
uint8_t cmd3[3]={0xFC,0xC9};

static I2C_Handle hi2c_si;

static double temp;
static uint32_t data_1,data_2;

static uint8_t txBuffer_si[4];
static uint8_t rxBuffer_si[4];
//functions
static void SI705X_writeRegister(uint8_t cmd, uint8_t value);
static uint16_t SI705X_readRegister(uint8_t cmd,uint8_t length);
static uint32_t SI705X_readRegister1(uint8_t cmd[],uint8_t length);
static void SI705X_readSensor();
static void User_data();


//configuring si705x device
void SI705X_init(I2C_Handle *h)
{
    hi2c_si=*h;
    //Do nothing, use default settings

}

double SI705X_getCurrentTemp() {

    uint16_t temp_code;
    temp_code = SI705X_readRegister(HOLD_MASTER_M,2);
    return(((175.72*temp_code)/65536)-46.85);
}


void Firmware_version(){
    SI705X_readRegister1(cmd1,1);
}

uint32_t IDdata(uint32_t);

void readElectronicID(){
    //uint8_t data_1[4],data_2[4];
   // uint32_t data_1,data_2;
    data_1=SI705X_readRegister1(cmd2,4);
    data_2=SI705X_readRegister1(cmd3,4);
    IDdata(data_1);
    IDdata(data_2);
}

uint32_t IDdata(uint32_t data){

    return data;
}
////geting user data of rsvd
//static void User_data(){
//    uint8_t read_data1,result;
//    read_data1=SI705X_readRegister(RUSER_REG,1);
//    if(!read_data1){
//  //      System_printf("no read data");
//    }
//    else
//    {
//    result=((read_data1 & ~0x81)| (011111110 & 0x81));
//    SI705X_writeRegister(WUSER_REG, result);
//    }
//    if((read_data1 & 0x40)==0)
//    {
//        //System_printf("temperature value is correct");
//        SI705X_readSensor();
//    }
//    else
//    {
//      //  System_printf("temperature value is incorrect");
//    }
//
//}

//  reading data from read register
static void SI705X_readSensor(){
    uint16_t temp_code;
    temp_code = SI705X_readRegister(HOLD_MASTER_M,2);
    temp=((175.72*temp_code)/65536)-46.85;


}
//helper function to write SI705X register with 1 byte cmd
static void SI705X_writeRegister(uint8_t cmd, uint8_t value)
{
    uint8_t ret;
    I2C_Transaction i2cTransaction;

    txBuffer_si[0] = cmd;
    txBuffer_si[1] = (uint8_t) value ;

    i2cTransaction.writeBuf = txBuffer_si;
    i2cTransaction.writeCount = 2;

    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    i2cTransaction.slaveAddress = SI705X_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_si, &i2cTransaction);
    if (!ret) {
         //      System_printf("Unsuccessful I2C transfer");
    }
}


//helper function to read SI705X register with 1 byte cmd
static uint16_t SI705X_readRegister(uint8_t cmd,uint8_t length)
{
    uint8_t ret;
    I2C_Transaction i2cTransaction;

    txBuffer_si[0] = cmd;

    i2cTransaction.writeBuf = txBuffer_si;
    i2cTransaction.writeCount = 1;

    i2cTransaction.readBuf = rxBuffer_si;
    i2cTransaction.readCount = length;

    i2cTransaction.slaveAddress =SI705X_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_si, &i2cTransaction);
    if (!ret) {
       // System_printf("Unsuccessful I2C transfer");
    }
    else{

        if(length==1)
        {
            return (uint16_t)rxBuffer_si[0];
        }
        else if(length==2)
        {
            return (((uint16_t)rxBuffer_si[0]) << 8 | rxBuffer_si[1]);
        }
        else{
       // System_printf("invalid value");
    }}
return 0;
}
//readRegister for 2 byte cmd
static uint32_t SI705X_readRegister1(uint8_t *cmd,uint8_t length)
{
     uint8_t i;
    //    uint8_t data[4];
    //    uint8_t *p,*r;
    //    p=&rxBuffer[0];
    //    r=&data[0];
    uint8_t ret;
    I2C_Transaction i2cTransaction;
    for(i=0;i<=2;i++)
    {
        txBuffer_si[i]=cmd[i];
    }
    i2cTransaction.writeBuf = txBuffer_si;
    i2cTransaction.writeCount = 2;

    i2cTransaction.readBuf = rxBuffer_si;
    i2cTransaction.readCount = length;

    i2cTransaction.slaveAddress =SI705X_DEV_ADDRESS;

    ret = I2C_transfer(hi2c_si, &i2cTransaction);
    if (!ret) {
   //     System_printf("Unsuccessful I2C transfer");
    }

    else{
        if(length==1)
            return (uint32_t)rxBuffer_si;
        else if(length==4)
        return (uint32_t)(rxBuffer_si[0]<<24|rxBuffer_si[1]<<16|rxBuffer_si[2]<<8|rxBuffer_si[3]);
        else{
         // System_printif("invalid input");
    }}
    return 0;
}
