
/******************************************************************************

 @file  simple_broadcaster.c

 @brief This file contains the Simple BLE Broadcaster sample application for
        use with the CC2650 Bluetooth Low Energy Protocol Stack.

 Group: WCS, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
 Copyright (c) 2011-2016, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: ble_sdk_2_02_00_31
 Release Date: 2016-06-16 18:57:29
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"

#include "broadcaster.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "icall_apimsg.h"

#ifdef USE_RCOSC
#include "rcosc_calibration.h"
#endif //USE_RCOSC

#include "util.h"
#include <ti/mw/display/Display.h>
#include "Board.h"
#include "opt3001.h"
#include <tmp100.h>
#include "adxl362.h"

#include "simple_broadcaster.h"
#include <ti/drivers/SPI.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
//#define DEFAULT_ADVERTISING_INTERVAL          160

#define DEFAULT_ADVERTISING_INTERVAL          3200 //2s
// Task configuration
#define SBB_TASK_PRIORITY                     1

#ifndef SBB_TASK_STACK_SIZE
#define SBB_TASK_STACK_SIZE                   660
#endif
  
#define BEACON_FEATURE

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr; // Event header.
} sbbEvt_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

// Display Interface
Display_Handle dispHandle = NULL;

I2C_Handle boardI2c;
SPI_Handle boardSpi;
Clock_Struct beaconUpdateClock;


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore sem;

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// Task configuration
Task_Struct sbbTask;
Char sbbTaskStack[SBB_TASK_STACK_SIZE];



// GAP - SCAN RSP data (max size = 31 bytes)
static uint8 scanRspData[] =
{
  // complete name
  0x0D,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'A',
  'c',
  't',
  'i',
  'v',
  'e',
  ' ',
  'S',
  'e',
  'n',
  's',
  'e',

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm  
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8 advertData[] = 
{ 
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
  
#ifndef BEACON_FEATURE
  
  // three-byte broadcast of the data "1 2 3"
  0x04,   // length of this data including the data type byte
  GAP_ADTYPE_MANUFACTURER_SPECIFIC, // manufacturer specific adv data type
  1,
  2,
  3

#else
    
  // 25 byte beacon advertisement data
  // Preamble: Company ID - 0x000D for TI, refer to https://www.bluetooth.org/en-us/specification/assigned-numbers/company-identifiers
  // Data type: Beacon (0x02)
  // Data length: 0x15
  // UUID: 00000000-0000-0000-0000-000000000000 (null beacon)
  // Major: 1 (0x0001)
  // Minor: 1 (0x0001)
  // Measured Power: -59 (0xc5)
  0x1A, // length of this data including the data type byte
  GAP_ADTYPE_MANUFACTURER_SPECIFIC, // manufacturer specific adv data type
  0x0D, // Company ID - Fixed
  0x00, // Company ID - Fixed
  0x02, // Data Type - Fixed
  0x15, // Data Length - Fixed
  0x00, // UUID - Variable based on different use cases/applications
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // UUID
  0x00, // Major
  0x01, // Major
  0x00, // Minor
  0x01, // Minor
  0xc5  // Power - The 2's complement of the calibrated Tx Power
    
#endif // !BEACON_FEATURE
};

static PIN_Config aLedPin[] = {
                               Board_RLED   | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN,         /* LED initially off             */
                               PIN_TERMINATE
};

static PIN_State hLedPin;
/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void SimpleBLEBroadcaster_init(void);
static void SimpleBLEBroadcaster_taskFxn(UArg a0, UArg a1);

static void SimpleBLEBroadcaster_processStackMsg(ICall_Hdr *pMsg);
static void SimpleBLEBroadcaster_processAppMsg(sbbEvt_t *pMsg);
static void SimpleBLEBroadcaster_processStateChangeEvt(gaprole_States_t newState);

static void SimpleBLEBroadcaster_stateChangeCB(gaprole_States_t newState);

static void TmrBeaconUpadte(UArg arg);
static void enableBeacon(uint8_t enable);
/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t simpleBLEBroadcaster_BroadcasterCBs =
{
  SimpleBLEBroadcaster_stateChangeCB   // Profile State Change Callbacks
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleBLEBroadcaster_createTask
 *
 * @brief   Task creation function for the Simple BLE Broadcaster.
 *
 * @param   none
 *
 * @return  none
 */
void SimpleBLEBroadcaster_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = sbbTaskStack;
  taskParams.stackSize = SBB_TASK_STACK_SIZE;
  taskParams.priority = SBB_TASK_PRIORITY;
  
  Task_construct(&sbbTask, SimpleBLEBroadcaster_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      SimpleBLEBroadcaster_init
 *
 * @brief   Initialization function for the Simple BLE Broadcaster App
 *          Task. This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification ...).
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLEBroadcaster_init(void)
{

    Clock_Params  beacClkParam;
    I2C_Params      i2cParams;

    I2C_init();
    SPI_init();

    // ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);
    
  // Hard code the DB Address till CC2650 board gets its own IEEE address
  //uint8 bdAddress[B_ADDR_LEN] = { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };
  //HCI_EXT_SetBDADDRCmd(bdAddress);
  
#ifdef USE_RCOSC
  RCOSC_enableCalibration();
#endif // USE_RCOSC
  
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);

  // Open LCD
//  dispHandle = Display_open(Display_Type_LCD, NULL);
  
  // Setup the GAP Broadcaster Role Profile
  {
    // For all hardware platforms, device starts advertising upon initialization
    uint8_t initial_advertising_enable = TRUE;

    // By setting this to zero, the device will go into the waiting state after
    // being discoverable for 30.72 second, and will not being advertising again
    // until the enabler is set back to TRUE
    uint16_t gapRole_AdvertOffTime = 0;
      
#ifndef BEACON_FEATURE
    uint8_t advType = GAP_ADTYPE_ADV_SCAN_IND; // use scannable undirected adv
#else
    uint8_t advType = GAP_ADTYPE_ADV_NONCONN_IND; // use non-connectable adv
#endif // !BEACON_FEATURE
    
    // Set the GAP Role Parameters
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), 
                         &initial_advertising_enable);
    GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16_t), 
                         &gapRole_AdvertOffTime);
    
    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof (scanRspData), 
                         scanRspData);
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);

    GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8_t), &advType);
  }

  // Set advertising interval
  {
    uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;

    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
  }

  // Start the Device
  VOID GAPRole_StartDevice(&simpleBLEBroadcaster_BroadcasterCBs);
  

  Display_print0(dispHandle, 0, 0, "BLE Broadcaster");

  PIN_open(&hLedPin, aLedPin);

   I2C_Params_init(&i2cParams);
   i2cParams.bitRate = I2C_100kHz;
   i2cParams.transferMode = I2C_MODE_BLOCKING;

   boardI2c = I2C_open(Board_I2C, &i2cParams);

   if(!boardI2c){
       while(1);
   }
  //Initialize devices
  OPT3001_initialize(&boardI2c);
  OPT3001_startMeasureContinious();

  TMP100_init(&boardI2c);
  //TMP100_getCurrentTemp();


  SPI_Params spiParams;
  SPI_Params_init(&spiParams);
  spiParams.bitRate = 2000000;           //2MHz
  spiParams.mode = SPI_MASTER;
  spiParams.frameFormat = SPI_POL0_PHA0;
  spiParams.transferMode = SPI_MODE_BLOCKING;

  boardSpi = SPI_open(Board_SPI, &spiParams);
  ADXL362_initialize(&boardSpi);
  PIN_setOutputValue(&hLedPin, Board_RLED, Board_LED_OFF);

  //Start the Beacon update timer

  Clock_Params_init(&beacClkParam);
  beacClkParam.period = BEACON_UPDATE_INTERVAL * (1000 / Clock_tickPeriod);
  beacClkParam.startFlag = FALSE;

//  beaconUpdateClock = Clock_create(TmrBeaconUpadte, 0, &beacClkParam, NULL);

  Clock_construct(&beaconUpdateClock, TmrBeaconUpadte, 0, &beacClkParam);
}

/*********************************************************************
 * @fn      SimpleBLEBroadcaster_processEvent
 *
 * @brief   Application task entry point for the Simple BLE Broadcaster.
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLEBroadcaster_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  SimpleBLEBroadcaster_init();
  
  // Application main loop
  for (;;)
  {
    // Get the ticks since startup
   // uint32_t tickStart = Clock_getTicks();

    // Waits for a signal to the semaphore associated with the calling thread.
    // Note that the semaphore associated with a thread is signaled when a
    // message is queued to the message receive queue of the thread or when
    // ICall_signal() function is called onto the semaphore.
    ICall_Errno errno = ICall_wait(ICALL_TIMEOUT_FOREVER);

    if (errno == ICALL_ERRNO_SUCCESS)
    {
      ICall_EntityID dest;
      ICall_ServiceEnum src;
      ICall_HciExtEvt *pMsg = NULL;
      
      if (ICall_fetchServiceMsg(&src, &dest, 
                                (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
      {
        if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == selfEntity))
        {
          // Process inter-task message
          SimpleBLEBroadcaster_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        sbbEvt_t *pMsg = (sbbEvt_t *)Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          SimpleBLEBroadcaster_processAppMsg(pMsg);
          
          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }
  }
}

/*********************************************************************
 * @fn      SimpleBLEBroadcaster_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void SimpleBLEBroadcaster_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    default:
      // do nothing
      break;
  }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */                       
static void SimpleBLEBroadcaster_processAppMsg(sbbEvt_t *pMsg)
{
    uint16_t curLux;
    uint16_t curTemp;
    uint8_t adxlStatus;
  switch (pMsg->hdr.event)
  {
    case SBB_STATE_CHANGE_EVT:
      SimpleBLEBroadcaster_processStateChangeEvt((gaprole_States_t)pMsg->
                                                 hdr.state);
      break;
      
    case SBB_ADV_CHANGE_EVT:
        curLux = (uint16_t) OPT3001_getCurrnetLux();
        advertData[9] = (curLux >> 8) & 0xFF;
        advertData[10] = curLux & 0xFF;
        curTemp = (uint16_t) TMP100_getCurrentTemp();
        advertData[11] = (curTemp >> 8) & 0xFF;
        advertData[12] = curTemp & 0xFF;
        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);
//        readAll();
        break;
    case SBB_OPT_INT_EVT:
        //Got an interrupt from optical sensor. Call the interrput
        OPT3001_intHandler();
        break;
    case SBB_ADXL_INT_EVT:
        if( adxl362_isAwake() ){
           // PIN_setOutputValue(&hLedPin, Board_RLED, Board_LED_ON);
            enableBeacon(TRUE);
            OPT3001_startMeasureContinious();

            Clock_start( &beaconUpdateClock);

        } else {
            //PIN_setOutputValue(&hLedPin, Board_RLED, Board_LED_OFF);
            Clock_stop(&beaconUpdateClock);
            OPT3001_stopMeasure();
            enableBeacon(FALSE);
        }

        if(adxl362_isDataReady()){
            get_accleration();
        }
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      SimpleBLEBroadcaster_stateChangeCB
 *
 * @brief   Callback from GAP Role indicating a role state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void SimpleBLEBroadcaster_stateChangeCB(gaprole_States_t newState)
{
  sbbEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if ((pMsg = ICall_malloc(sizeof(sbbEvt_t))))
  {
    pMsg->hdr.event = SBB_STATE_CHANGE_EVT;
    pMsg->hdr.state = newState;
    
    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, sem, (uint8*)pMsg);
  }
}

/*********************************************************************
 * @fn      SimpleBLEBroadcaster_processStateChangeEvt
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void SimpleBLEBroadcaster_processStateChangeEvt(gaprole_States_t newState)
{
  switch (newState)
  {
    case GAPROLE_STARTED:
      {    
        uint8 ownAddress[B_ADDR_LEN];
        
        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);
        
        // Display device address 
        Display_print0(dispHandle, 1, 0, Util_convertBdAddr2Str(ownAddress));
        Display_print0(dispHandle, 2, 0, "Initialized");
      }
      break;
      
    case GAPROLE_ADVERTISING:
      {
        Display_print0(dispHandle, 2, 0, "Advertising");        
      }
      break;

    case GAPROLE_WAITING:
      {
        Display_print0(dispHandle, 2, 0, "Waiting");       
      }
      break;          

    case GAPROLE_ERROR:
      {
        Display_print0(dispHandle, 2, 0, "Error");         
      }
      break;      
      
    default:
      {
        Display_clearLine(dispHandle, 2);
      }
      break; 
  }
}




/*********************************************************************
*********************************************************************/

void AppEnqueuEvent(uint16_t evt)
{
    sbbEvt_t *pMsg;

    if ((pMsg = ICall_malloc(sizeof(sbbEvt_t))))
        {
          pMsg->hdr.event = evt;
          pMsg->hdr.state = 1;

          // Enqueue the message.
          Util_enqueueMsg(appMsgQueue, sem, (uint8*)pMsg);
        }

}

static void TmrBeaconUpadte(UArg arg)
{

    sbbEvt_t *pMsg;



    // Create dynamic pointer to message.
    if ((pMsg = ICall_malloc(sizeof(sbbEvt_t))))
    {
      pMsg->hdr.event = SBB_ADV_CHANGE_EVT;
      pMsg->hdr.state = 1;

      // Enqueue the message.
      Util_enqueueMsg(appMsgQueue, sem, (uint8*)pMsg);
    }
}



static void enableBeacon(uint8_t enable)
{
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                         &enable);

}
