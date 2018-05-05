/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u32 G_u32AntApiCurrentMessageTimeStamp;                            // From ant_api.c
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            // From ant_api.c
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  // From ant_api.c
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                // From ant_api.c

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u32 UserApp1_u32Timeout = 0;
static AntAssignChannelInfoType UserApp1_sMasterChannel;
static AntAssignChannelInfoType UserApp1_sSlaveChannel1;
static AntAssignChannelInfoType UserApp1_sSlaveChannel2;
static u8 UserApp1_au8LcdStartLine1[] = "Hide and Go Seek";
static u8 UserApp1_au8LcdStartLine2[] = "Push B0 to search";
static u8 UserApp1_au8MasterMessage[8] = {0,0,0,0,0,0,0,0};
static s8 s8RssiChannel0 = -99;
static s8 s8RssiChannel1 = -99;
static s8 s8RssiChannel2 = -99;
static s8 s8StrongestRssi = -99;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  LedOff(RED);
  LedOff(ORANGE);
  LedOff(YELLOW);
  LedOff(GREEN);
  LedOff(CYAN);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(WHITE);
  LedOn(LCD_GREEN);
  LedOff(LCD_BLUE);
  LedOff(LCD_RED);
  
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 i = 0; i < 100000; i++);
  
  LCDMessage(LINE1_START_ADDR, UserApp1_au8LcdStartLine1);
  LCDMessage(LINE2_START_ADDR, UserApp1_au8LcdStartLine2);
  /* Set up the two ANT channels that will be used for the task */
  
  /* Slave1 (Channel 0) */
  UserApp1_sSlaveChannel1.AntChannel = ANT_CHANNEL_0;
  UserApp1_sSlaveChannel1.AntChannelType = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel1.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT;
  UserApp1_sSlaveChannel1.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;
  
  UserApp1_sSlaveChannel1.AntDeviceIdHi = 0x00;
  UserApp1_sSlaveChannel1.AntDeviceIdLo = 0x22;
  UserApp1_sSlaveChannel1.AntDeviceType = 1;
  UserApp1_sSlaveChannel1.AntTransmissionType = 1;
  
  UserApp1_sSlaveChannel1.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sSlaveChannel1.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sSlaveChannel1.AntNetwork = ANT_NETWORK_DEFAULT;
  
  /* Master (Channel 1) */
  UserApp1_sMasterChannel.AntChannel = ANT_CHANNEL_1;
  UserApp1_sMasterChannel.AntChannelType = CHANNEL_TYPE_MASTER;
  UserApp1_sMasterChannel.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT;
  UserApp1_sMasterChannel.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;
  
  UserApp1_sMasterChannel.AntDeviceIdHi = 0x00;
  UserApp1_sMasterChannel.AntDeviceIdLo = 0x22;
  UserApp1_sMasterChannel.AntDeviceType = 1;
  UserApp1_sMasterChannel.AntTransmissionType = 1;
  
  UserApp1_sMasterChannel.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sMasterChannel.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sMasterChannel.AntNetwork = ANT_NETWORK_DEFAULT;
  
  /* Slave2 (Channel 2) */
  UserApp1_sSlaveChannel2.AntChannel = ANT_CHANNEL_2;
  UserApp1_sSlaveChannel2.AntChannelType = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel2.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT;
  UserApp1_sSlaveChannel2.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;
  
  UserApp1_sSlaveChannel2.AntDeviceIdHi = 0x00;
  UserApp1_sSlaveChannel2.AntDeviceIdLo = 0x22;
  UserApp1_sSlaveChannel2.AntDeviceType = 1;
  UserApp1_sSlaveChannel2.AntTransmissionType = 1;
  
  UserApp1_sSlaveChannel2.AntFrequency = ANT_FREQUENCY_DEFAULT;
  UserApp1_sSlaveChannel2.AntTxPower = ANT_TX_POWER_DEFAULT;
  UserApp1_sSlaveChannel2.AntNetwork = ANT_NETWORK_DEFAULT;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_sMasterChannel.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    UserApp1_sSlaveChannel1.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    UserApp1_sSlaveChannel2.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  if(AntAssignChannel(&UserApp1_sMasterChannel))
  {
    UserApp1_StateMachine = UserApp1SM_AntConfigureMaster;
  }
  else
  {
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_AntConfigureMaster(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_CONFIGURED)
  {
    AntAssignChannel(&UserApp1_sSlaveChannel1);
    UserApp1_StateMachine = UserApp1SM_AntConfigureSlave1;
  }
  
   /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Master config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}
static void UserApp1SM_AntConfigureSlave1(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED)
  {
    AntAssignChannel(&UserApp1_sSlaveChannel2);
    UserApp1_StateMachine = UserApp1SM_AntConfigureSlave2; 
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}
static void UserApp1SM_AntConfigureSlave2(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_CONFIGURED)
  {
    //AntQueueBroadcastMessage(ANT_CHANNEL_0, UserApp1_au8MasterMessage);
    //AntQueueBroadcastMessage(ANT_CHANNEL_1, UserApp1_au8MasterMessage);
    //AntQueueBroadcastMessage(ANT_CHANNEL_2, UserApp1_au8MasterMessage);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}
static void UserApp1SM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    
    /* Queue the Channel Open messages and then go to wait state */
    AntOpenChannelNumber(ANT_CHANNEL_0);
    //AntOpenChannelNumber(ANT_CHANNEL_1);
    AntOpenChannelNumber(ANT_CHANNEL_2);
    
    UserApp1_StateMachine = UserApp1SM_OpeningChannels;    
  }
} /* end UserApp1SM_Idle() */
static void UserApp1SM_OpeningChannels(void)
{
  /* Ensure that both channels have opened */
  if( (AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_OPEN) &&
      (AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_OPEN)   )
      
  {
    UserApp1_StateMachine = UserApp1SM_SlaveActive; 
  }
  
  /* Check for timeout */
      if( IsTimeUp(&UserApp1_u32Timeout, ANT_CONFIGURE_TIMEOUT_MS) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Channel open failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}
static void UserApp1SM_SlaveActive(void)
{
  LedNumberType aeLedDisplayLevels[] = {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE};
  s8 as8dBmLevels[] = {DBM_LEVEL1, DBM_LEVEL2, DBM_LEVEL3, DBM_LEVEL4, 
                       DBM_LEVEL5, DBM_LEVEL6, DBM_LEVEL7, DBM_LEVEL8};
  
  
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 0)
      {
        s8RssiChannel0 = G_sAntApiCurrentMessageExtData.s8RSSI;
        
        for(u8 i=0;i<DBM_NUMBER;i++)
        {
          if(s8RssiChannel0>as8dBmLevels[i])
          {
            LedOn(aeLedDisplayLevels[i]);
          }
        }
        s8StrongestRssi = s8RssiChannel0;
        if(s8StrongestRssi>DBM_LEVEL7 && s8StrongestRssi<DBM_LEVEL8)
        {
          LedBlink(RED,5);
          LedBlink(ORANGE,5);
          LedBlink(YELLOW,5);
          LedBlink(GREEN,5);
          LedBlink(CYAN,5);
          LedBlink(BLUE,5);
          LedBlink(PURPLE,5);
          LedBlink(WHITE,5);
          
          //PWMAudioSetFrequency(BUZZER1,);
          
          //AntOpenChannelNumber(ANT_CHANNEL_1);
          UserApp1_au8MasterMessage[0] = 1;
          AntQueueAcknowledgedMessage(ANT_CHANNEL_0,UserApp1_au8MasterMessage);
          //UserApp1_StateMachine = UserApp1SM_MasterActive;
          
        }
      }
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        s8RssiChannel2 = G_sAntApiCurrentMessageExtData.s8RSSI;
        
        for(u8 i=0;i<DBM_NUMBER;i++)
        {
          if(s8RssiChannel2>as8dBmLevels[i])
          {
            LedOn(aeLedDisplayLevels[i]);
          }
        }
        s8StrongestRssi = s8RssiChannel2;
        if(s8StrongestRssi>DBM_LEVEL7 && s8StrongestRssi<DBM_LEVEL8)
        {
          LedBlink(RED,5);
          LedBlink(ORANGE,5);
          LedBlink(YELLOW,5);
          LedBlink(GREEN,5);
          LedBlink(CYAN,5);
          LedBlink(BLUE,5);
          LedBlink(PURPLE,5);
          LedBlink(WHITE,5);
          
          //PWMAudioSetFrequency(BUZZER1,);
          
          UserApp1_au8MasterMessage[0] = 2;
          AntQueueAcknowledgedMessage(ANT_CHANNEL_2,UserApp1_au8MasterMessage);
          //UserApp1_StateMachine = UserApp1SM_MasterActive;
      }
      
      //AntGetdBmAscii(s8RssiChannel0, &UserApp1_au8LcdInformationMessage[INDEX_MASTER_DBM]);
 
      }
    }
  }
}
static void UserApp1SM_MasterActive(void)
{
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      s8RssiChannel1 = G_sAntApiCurrentMessageExtData.s8RSSI;
    }
    if(s8RssiChannel1>DBM_LEVEL7 && s8RssiChannel1<DBM_LEVEL8)
    {
      UserApp1_StateMachine = UserApp1SM_SlaveActive;
    }
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
