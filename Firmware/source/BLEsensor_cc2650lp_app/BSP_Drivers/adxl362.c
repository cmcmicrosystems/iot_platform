

/*
 * adxl362.c
 *
 *  Created on: 14-Nov-2017
 *      Author: Niraj
 *      Modified in Sept. 2021 by Olive
 */

#include "adxl362.h"
#include "Board.h"
#include <ti/drivers/PIN.h>
#include "simple_broadcaster.h"


SPI_Handle  hSpi;



//Local function decleration
static void xl362writeRegister(uint8_t address,uint8_t value);
static uint8_t xl362readRegister(uint8_t address);
static void hAdxlPinIntCb(PIN_Handle handle, PIN_Id pinId);
static uint8_t txBufferSpi[3];
static uint8_t rxBufferSpi[3];
static uint16_t errCount=0;

//Local variables
const PIN_Config xlPinCfg[] = {
                               Board_ADX_INT1   |  PIN_INPUT_EN | PIN_PULLUP | PIN_HYSTERESIS | PIN_IRQ_DIS,
                               Board_ADX_INT2   |  PIN_INPUT_EN | PIN_PULLUP | PIN_HYSTERESIS | PIN_IRQ_DIS,
                               Board_SPI_ADX_CS | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MIN,  /* chip select    */
                               PIN_TERMINATE
};

PIN_State hAxlPin;

//Utility function to write a register to sensor
static void xl362writeRegister(uint8_t address,uint8_t value)
{

    uint8_t ret;
    SPI_Transaction spiTransaction;

    txBufferSpi[0]=WRITE_CMD;
    txBufferSpi[1]=address;
    txBufferSpi[2]=value;

    spiTransaction.count =3;
    spiTransaction.txBuf = txBufferSpi;
    spiTransaction.rxBuf = rxBufferSpi;
    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_ON);
    ret = SPI_transfer(hSpi, &spiTransaction);
    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_OFF);

    if (!ret) {
        //     System_printf("Unsuccessful SPI transfer");
        errCount++;
    }

}

//Utility function to read a register from sensor
static uint8_t xl362readRegister(uint8_t address)
{
    uint8_t ret;
    SPI_Transaction spiTransaction;
    txBufferSpi[0]=READ_CMD;
    txBufferSpi[1]=address;
    txBufferSpi[2]=0x00;
    spiTransaction.count =3;
    spiTransaction.txBuf = txBufferSpi;
    spiTransaction.rxBuf =rxBufferSpi;

    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_ON);
    ret = SPI_transfer(hSpi, &spiTransaction);
    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_OFF);
    if (!ret) {
        //   System_printf("Unsuccessful SPI transfer");
        errCount++;
    }
    else{
        return((uint8_t)rxBufferSpi[2]);
    }
    return 0;
}

//Utility funtion to read FIFO of ADXL362
uint8_t xl362readFIFO(uint8_t *buffer, uint8_t v_length){
    uint8_t *txBuffer;
    uint8_t ret;
    SPI_Transaction spiTransaction;


    txBuffer = (uint8_t*) malloc(v_length + 1);
    memset(txBuffer, 0x00 , (v_length+1));
    txBuffer[0] = READ_FIFO;
    spiTransaction.count = v_length+1;
    spiTransaction.txBuf = txBufferSpi;
    spiTransaction.rxBuf = buffer;
    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_ON);
    ret = SPI_transfer(hSpi, &spiTransaction);
    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_OFF);

    if (!ret) {
        //     System_printf("Unsuccessful SPI transfer");
        return 0;
    }
    else{
        return 1;
    }
    return 0;
}

//Iniitlization of the ADXL362
void ADXL362_initialize(SPI_Handle *handle){

    hSpi = *handle;

    //volatile uint32_t wait = 5000;
    volatile uint32_t wait = 500;
    // Initialize pins
    PIN_open(&hAxlPin, xlPinCfg );
    PIN_registerIntCb(&hAxlPin, hAdxlPinIntCb);

    //conf of register for wakeup mode
    xl362writeRegister(SOFT_RESET, 0x52);
    while(wait--);
    xl362writeRegister(THRESH_ACT_L,0xFA);//config activity threshold
    xl362writeRegister(THRESH_ACT_H,0x00);//
    xl362writeRegister(TIME_ACT,0x32);//
    xl362writeRegister(THRESH_INACT_L,0x64);//config inactivity threshold
    xl362writeRegister(THRESH_INACT_H,0x00);//
   // xl362writeRegister(TIME_INACT_L,0xDC);// Inactivity time threshold
    //xl362writeRegister(TIME_INACT_H,0x05);// 15sec inactivity at ODR=100Hz

    xl362writeRegister(TIME_INACT_L,0xE8);// Inactivity time threshold
    xl362writeRegister(TIME_INACT_H,0x03);// 10sec inactivity at ODR=100Hz

       //xl362writeRegister(TIME_INACT_L,0xF4);// Inactivity time threshold
       //xl362writeRegister(TIME_INACT_H,0x01);// 5sec inactivity at ODR=100Hz


    xl362writeRegister(ACT_INACT_CTL,0x1F);//Link mode, reference based act/inact control
    xl362writeRegister(FILTER_CTL,0x03);// ODR = 100 Hz Range=2g
//    xl362writeRegister(POWER_CTL,0x0E); // Wake up mode, autosleep, measure enable
    xl362writeRegister(POWER_CTL,0x06); // Wake up mode, autosleep, measure enable

    adxl362GetStatus(); //Read status to clear interrupt
    get_accleration();
    PIN_setInterrupt(&hAxlPin, Board_ADX_INT1|PIN_IRQ_NEGEDGE);
//    PIN_setInterrupt(&hAxlPin, Board_ADX_INT2|PIN_IRQ_NEGEDGE);

    xl362writeRegister(INTMAP1,0xB0);//INACT, ACT mapped it INT1
    xl362writeRegister(INTMAP2,0xC0);//AWAKE mapped it INT2



}

uint8_t adxl362GetStatus()
{
    return(xl362readRegister(STATUS));
}

uint8_t adxl362_isAwake()
{
    return ((adxl362GetStatus() & AWAKE_FLAG) ? 1:0);
}
uint8_t adxl362_isDataReady()
{
    return ((adxl362GetStatus() & DATA_READY_FLAG) ? 1:0);
}
void readAll()
{
    SPI_Transaction spiTransaction;
    uint8_t ret;

    txBufferSpi[0] = READ_CMD;
    txBufferSpi[1] = DEVID_AD;

    spiTransaction.txBuf = txBufferSpi;
    spiTransaction.count = 2;
    spiTransaction.rxBuf = rxBufferSpi;

    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_ON);
    ret = SPI_transfer(hSpi, &spiTransaction);
    txBufferSpi[0] = 0;
    txBufferSpi[1] = 0;
    spiTransaction.txBuf = txBufferSpi;
    spiTransaction.count = 1;
    spiTransaction.rxBuf = rxBufferSpi;

    for(int i=DEVID_AD; i<=SELF_TEST;i++) {
        ret = SPI_transfer(hSpi, &spiTransaction);
    }
    PIN_setOutputValue(&hAxlPin, Board_SPI_ADX_CS, Board_ADX_CS_OFF);


//    xl362readRegister(ACT_INACT_CTL);
}
void get_accleration()
{
    uint8_t  accl_dx_l, accl_dx_h,accl_dy_l,accl_dy_h,accl_dz_l,accl_dz_h;
    uint16_t  accl_data_12_x, accl_data_12_y, accl_data_12_z;
    // reading 12 bits data in parts
    accl_dx_l=xl362readRegister(XDATA_L);
    accl_dx_h=xl362readRegister(XDATA_H);
    accl_dy_l=xl362readRegister(YDATA_L);
    accl_dy_h=xl362readRegister(YDATA_H);
    accl_dz_l=xl362readRegister(ZDATA_L);
    accl_dz_h=xl362readRegister(ZDATA_H);


    //accletation data in 12 bits
    accl_data_12_x=(uint16_t)(((accl_dx_h)& 0xF0)|accl_dx_l);
    accl_data_12_y=(uint16_t)(((accl_dy_h)& 0xF0)|accl_dy_l);
    accl_data_12_z=(uint16_t)(((accl_dz_h)& 0xF0)|accl_dz_l);


}

void get_device_data(){
    uint8_t device_data_1,device_data_2,device_data_3,device_data_4,status;
    device_data_1=xl362readRegister(DEVID_AD);
    device_data_2=xl362readRegister(DEVID_MST);
    device_data_3=xl362readRegister(PARTID);
    device_data_4=xl362readRegister(REVID);
    status=xl362readRegister(STATUS);

}
void get_accleration_msb(){
    uint8_t accl_data_8_x,accl_data_8_y,accl_data_8_z;


    //read accleration data in 8 bits
    accl_data_8_x=xl362readRegister(XDATA);
    accl_data_8_y=xl362readRegister(YDATA);
    accl_data_8_z=xl362readRegister(ZDATA);


}




static void hAdxlPinIntCb(PIN_Handle handle, PIN_Id pinId)
{
    AppEnqueuEvent(SBB_ADXL_INT_EVT);
}
