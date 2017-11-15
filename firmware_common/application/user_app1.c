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


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 UserApp_CursorPosition;

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
  LCDMessage(LINE1_START_ADDR, "Please press :");
  LCDClearChars(LINE1_START_ADDR + 14, 4);
  LCDMessage(LINE2_START_ADDR,"B0  B1  B3");
  LCDCommand(LCD_HOME_CMD);
  UserApp_CursorPosition = LINE1_START_ADDR;

 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
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
static void UserApp1SM_Idle(void)
{
  extern u8 G_au8DebugScanfBuffer[];  
  extern u8 G_u8DebugScanfCharCount;
  static u8 au8UserInputBuffer[100];
  static u8 au8UserInput[100];
  static u8 au8Message[100];
  static u8 u8counter1 = 0;
  static u8 u8counter2 = 4;
  static u8 u8mode = 0;
  static u8 u8index = 0;
  static u8 u8counter = 0;
  static bool bstate1 = TRUE;
  static bool bstate2 = TRUE;
  static bool benterpressed = FALSE;
  
  if(WasButtonPressed(BUTTON0) & (bstate1 == TRUE))
    {
      ButtonAcknowledge(BUTTON0);
      LedOn(WHITE);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(UserApp_CursorPosition, "Chenlong");
      LCDMessage(UserApp_CursorPosition+20, "Chenlong");
      u8mode = 1;
      bstate1 = FALSE;
      bstate2 = FALSE;
    }
  /*enter meun1*/
  if(u8mode == 1)
  {
    if(WasButtonPressed(BUTTON0) & (bstate1 == FALSE))
    {
      ButtonAcknowledge(BUTTON0);
      u8counter1++;
      u8counter2 = 4;
      if(UserApp_CursorPosition == LINE1_END_ADDR)
      {
        UserApp_CursorPosition=LINE2_START_ADDR;
      }
      else if((UserApp_CursorPosition+7) == LINE2_END_ADDR)
      {
        PWMAudioSetFrequency(BUZZER1,500);
        PWMAudioOn(BUZZER1);
        UserApp_CursorPosition = LINE2_END_ADDR-7;
      }
      else
      {
        UserApp_CursorPosition++;
      }
      if(UserApp_CursorPosition != LINE2_END_ADDR-7)
      {
        PWMAudioOff(BUZZER1);
      }

      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(UserApp_CursorPosition, "Chenlong");
      LCDMessage(UserApp_CursorPosition+20, "Chenlong");
    }
    /* move to the right one*/
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      u8counter2--;
      u8counter1 = 0;
      if(UserApp_CursorPosition == LINE2_START_ADDR)
      {
        UserApp_CursorPosition=LINE1_END_ADDR;
      }
      else if(UserApp_CursorPosition == LINE1_START_ADDR)
      {
        PWMAudioSetFrequency(BUZZER1,500);
        PWMAudioOn(BUZZER1);
        UserApp_CursorPosition = LINE1_START_ADDR;
      }
      else
      {
        UserApp_CursorPosition--;
      }
      if(UserApp_CursorPosition != LINE1_START_ADDR)
      {
        PWMAudioOff(BUZZER1);
      }

      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(UserApp_CursorPosition, "Chenlong");
      LCDMessage(UserApp_CursorPosition+20, "Chenlong");
    }/*move to the left one*/
    
    if(u8counter1 == 4)
    {
      u8counter1=1;
    }
    if(u8counter2 == 0)
    {
      u8counter2=3;
    }
    if((UserApp_CursorPosition == LINE1_START_ADDR) || ((UserApp_CursorPosition+7) == LINE2_END_ADDR))
    {
      u8counter1=0;
      u8counter2=4;
      LedOff(RED);
      LedOff(GREEN);
      LedOff(BLUE);
    }
         
    if(u8counter1 == 1 || u8counter2 == 1)
    {
      PWMAudioSetFrequency(BUZZER1,262);
      PWMAudioOn(BUZZER1);
      LedOn(RED);
      LedOff(GREEN);
      LedOff(BLUE);
      LedOff(WHITE);
    }
    if(u8counter1 == 2 || u8counter2 == 2)
    {
      PWMAudioSetFrequency(BUZZER1,294);
      PWMAudioOn(BUZZER1);
      LedOn(GREEN);
      LedOff(RED);
      LedOff(BLUE);
      LedOff(WHITE);
    }
    if(u8counter1 == 3 || u8counter2 == 3)
    {
      PWMAudioSetFrequency(BUZZER1,330);
      PWMAudioOn(BUZZER1);
      LedOn(BLUE);
      LedOff(GREEN);
      LedOff(RED);
      LedOff(WHITE);
    }
  }/*LEDs and Buzzer's function*/
  
  
  
  if(WasButtonPressed(BUTTON1) & (bstate2 == TRUE))
    {
      ButtonAcknowledge(BUTTON1);
      LCDCommand(LCD_CLEAR_CMD);
      bstate2 = FALSE;
      u8mode = 2;
    }/* enter the meun2 */
  if(u8mode == 2)
  {
    if(G_u8DebugScanfCharCount>0)
    {
      DebugScanf(au8UserInputBuffer);
      au8UserInput[u8index]=au8UserInputBuffer[0];
      u8index++;
    
      if(UserApp_CursorPosition != LINE2_END_ADDR+1)
      {
        LCDMessage(UserApp_CursorPosition,au8UserInputBuffer);
        LCDMessage(UserApp_CursorPosition+20,au8UserInputBuffer);
        UserApp_CursorPosition++;
      }
      if((benterpressed == FALSE) && (UserApp_CursorPosition == LINE2_END_ADDR+1))
      {
        u8counter = u8index-40;
        for(u8 i=0;i<40;i++)
        {
          au8Message[i] = au8UserInput[u8counter+i];
        }
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR,au8Message);
        LCDMessage(LINE1_START_ADDR+20,au8Message);
      }/* do not enter newline */
  
      if(au8UserInputBuffer[0] == 0x0d)
      {
        UserApp_CursorPosition = LINE2_START_ADDR;
        benterpressed = TRUE;
      }
      if((benterpressed == TRUE) && (UserApp_CursorPosition == LINE2_END_ADDR+1))
      {
        u8counter = u8index-20;
        for(u8 n=0;n<20;n++)
        {
          au8Message[n] = au8UserInput[u8counter+n];
        }
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE2_START_ADDR,au8Message);
        LCDMessage(LINE2_START_ADDR+20,au8Message);
      }/*enter the second line*/
    }  
  }
  
  
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Please press :");
    LCDMessage(LINE2_START_ADDR,"B0  B1  B3");
    u8counter1 = 0;
    u8counter2 = 4;
    u8mode = 0;
    u8index = 0;
    u8counter = 0;
    bstate1 = TRUE;
    bstate2 = TRUE;
    benterpressed = FALSE;
    UserApp_CursorPosition = LINE1_START_ADDR;
    for(u8 m=0;m<100;m++)
    {
      au8Message[m]=0;
      au8UserInput[m]=0;
    }
    
  }/*return the primary meun*/


} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
