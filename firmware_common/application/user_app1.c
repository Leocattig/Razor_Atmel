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
  static bool bresetpassword = FALSE;
  static u8 au8pinnum1[10]={1,2,3,1,2,3};
  static u8 au8pinnum2[10];
  static u8 u8index=0;
  static u8 u8index1=0;
  static u8 u8index2=0;
  static u8 u8index3=0;
  static u8 u8mode=0;
  static u8 u8counter=0;
  static u8 u8counter2=0;
  if(IsButtonHeld(BUTTON3,2000))
    {
      ButtonAcknowledge(BUTTON3);
      bresetpassword = TRUE;
      LedOff(RED);
      LedOff(GREEN);
      LedBlink(RED,LED_2HZ);
      LedBlink(GREEN,LED_2HZ);
    }
  
  if(u8index1==10)
  {
  bresetpassword = FALSE;
  }
  if(bresetpassword)
    {
      if(WasButtonPressed(BUTTON0))
      {
        ButtonAcknowledge(BUTTON0);
        au8pinnum1[u8index1]=1;
        u8index1++;
      }
      if(WasButtonPressed(BUTTON1))
      {
        ButtonAcknowledge(BUTTON1);
        au8pinnum1[u8index1]=2;
        u8index1++;
      }
      if(WasButtonPressed(BUTTON2))
      {
        ButtonAcknowledge(BUTTON2);
        au8pinnum1[u8index1]=3;
        u8index1++;
      }
    }
  
  for(u8index;u8index<sizeof(au8pinnum1);u8index++)
  {
   if(au8pinnum1[u8index]==1 || au8pinnum1[u8index]==2 || au8pinnum1[u8index]==3)
    {
     u8counter2++;
    }
  }   /*calculate the number of array elements*/
  u8index=0;  
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    ButtonAcknowledge(BUTTON2);
    ButtonAcknowledge(BUTTON1);
    ButtonAcknowledge(BUTTON0); 
    bresetpassword = FALSE;
    u8mode++;
    LedOff(GREEN);
    LedOff(RED);
    u8index1=0;
    u8index2=0;
  }
  if(u8mode==1)
  {
    LedOn(RED);
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      au8pinnum2[u8index2]=1;
      u8index2++;
    }
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      au8pinnum2[u8index2]=2;
      u8index2++;
    }
    if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      au8pinnum2[u8index2]=3;
      u8index2++;
    }
  }
  if(u8mode==2)
  {
    for(u8index3;u8index3<6;u8index3++)
    {
      if(au8pinnum2[u8index3]==au8pinnum1[u8index3])
      {
        u8counter++;
      }
    }
    u8index3=0;
    if(u8counter==6)
    {
    LedBlink(GREEN,LED_2HZ);
    }
    else
    {
    LedBlink(RED,LED_2HZ);
    } 
    u8mode=0;
    u8counter=0;
    for(u8index=0;u8index<10;u8index++)
    {
      au8pinnum2[u8index]=0;
    }
    u8index=0;
  }
  
      
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
