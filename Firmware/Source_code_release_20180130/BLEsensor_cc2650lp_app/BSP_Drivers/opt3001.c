
/*
 * opt3001.c
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 */

#include "opt3001.h"
#include "Board.h"
#include <ti/drivers/PIN.h>

#include "simple_broadcaster.h"
#include "util.h"


static I2C_Handle hi2c;
static PIN_State  hStateHui;
///static PIN_Handle   hHandleHui;

static double lux;
double opt3001_getdata();
static uint16_t data;
uint8_t rxBuffer[3], txBuffer[3];
const PIN_Config aPinCfg[] = {
                              Board_OPT3001_INT   |  PIN_INPUT_EN | PIN_PULLUP | PIN_HYSTERESIS | PIN_IRQ_NEGEDGE,
//                              PIN_ID(14) | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
//                              PIN_ID(10) | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                               PIN_TERMINATE
};

void huiPinIntCb(PIN_Handle handle, PIN_Id pinId);
static void OPT3001_writeRegister(uint8_t address, uint16_t value);
static uint16_t OPT3001_readRegister(uint8_t address);


//configuring OPT3011 device
void OPT3001_initialize(I2C_Handle *h)
{

    hi2c = *h; //I2C handle



    PIN_open(&hStateHui, aPinCfg);
    PIN_registerIntCb(&hStateHui, huiPinIntCb);
    PIN_setInterrupt(&hStateHui,Board_OPT3001_INT|PIN_IRQ_BOTHEDGES );
//    PIN_setInterrupt(&hStateHui, PIN_ID(14) | PIN_IRQ_BOTHEDGES);


    OPT3001_writeRegister(OPT3001_CONFIG_ADDR, (OPT3001_M | OPT3001_CT | OPT3001_MODE_SHUTDOWN) );
    OPT3001_writeRegister(OPT3001_LOW_LIMIT_ADDR, OPT3001_INT_MODE);

    data=OPT3001_readRegister(OPT3001_CONFIG_ADDR);

}

double opt3001_getdata(){
    return data;
}



double OPT3001_getCurrnetLux() {
    return lux;
}

void OPT3001_startMeasureContinious()
{

    OPT3001_writeRegister(OPT3001_CONFIG_ADDR, (OPT3001_M | OPT3001_CT | OPT3001_MODE_CONTINIOUS));
}


void OPT3001_stopMeasure()
{

    OPT3001_writeRegister(OPT3001_CONFIG_ADDR, (OPT3001_M | OPT3001_CT | OPT3001_MODE_SHUTDOWN));
}

//helper function to write OPT3001 register
static void OPT3001_writeRegister(uint8_t address, uint16_t value)
{
  //  uint8_t txBuffer[3];
    uint8_t ret;
    I2C_Transaction i2cTransaction;

    txBuffer[0] = address;
    txBuffer[1] = (uint8_t) ((value >> 8) & 0xFF);
    txBuffer[2] = (uint8_t) (value & 0xFF);

    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 3;

    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    i2cTransaction.slaveAddress = OPT3001_DEV_ADDRESS;

    ret = I2C_transfer(hi2c, &i2cTransaction);
    if (!ret) {
//        System_printf("Unsuccessful I2C transfer");
    }
}

//helper function to read OPT3001 register
static uint16_t OPT3001_readRegister(uint8_t address)
{
  //  uint8_t txBuffer[1];
  //  uint8_t rxBuffer[2];
    uint8_t ret;
    I2C_Transaction i2cTransaction;

    txBuffer[0] = address;

    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;

    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    i2cTransaction.slaveAddress = OPT3001_DEV_ADDRESS;

    ret = I2C_transfer(hi2c, &i2cTransaction);
    if (!ret) {
//        System_printf("Unsuccessful I2C transfer");
    }

    return (((uint16_t)rxBuffer[0]) << 8 | rxBuffer[1]);
}

//  reading data from read register
static double OPT3001_readSensor()
{
    uint16_t result;

    uint8_t exponent;
    uint16_t mantisa;

    double calcLux;

    result = OPT3001_readRegister(OPT3001_RESULT_ADDR);
    exponent = (result >> 12) & 0x0F;
    mantisa = result & 0x0FFF;


    calcLux = 0.01 * (2^exponent) * (mantisa);

    return (calcLux);


}

//Handle interrupts in task context
void OPT3001_intHandler()
{

    if(OPT3001_readRegister(OPT3001_CONFIG_ADDR) & CFG_BM_CRF) {
        lux=OPT3001_readSensor();
    }

}

// Pin interrupt callback
void huiPinIntCb(PIN_Handle handle, PIN_Id pinId) {




    if(pinId == Board_OPT3001_INT) {
        AppEnqueuEvent(SBB_OPT_INT_EVT);
    }
//    if(pinId == PIN_ID(14)) {
//        if(PIN_getInputValue(pinId)) {
//            PIN_setOutputValue(handle, PIN_ID(10), 1);
//        } else {
//            PIN_setOutputValue(handle, PIN_ID(10), 0);
//        }
//    }
}
