/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
  DebugPrintf("LED Programming Interface");
  DebugLineFeed();
  DebugPrintf("Press 1 to program LED command sequence");
  DebugLineFeed();
  DebugPrintf("Press 2 to show current USER program");
  
  /* Turn off the Debug task command processor and announce the task is ready */

  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
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
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  extern u8 G_au8DebugScanfBuffer[];
  extern u8 G_u8DebugScanfCharCount;
  static u32 u32ontime = 0;
  static u32 u32offtime = 0;
  static u8 u8index1,u8index2,u8index3,u8index4,u8index5 = 0;
  static u8 u8color = 0;
  static u8 u8mode = 0;
  static u8 u8digit1,u8digit2,u8position = 0;
  static u8 u8counter = 0;
  static u8 u8counter1 = 0;
  static u8 u8counter2 = 1;  
  static u8 au8UserInputBuffer[10];
  static u16 au16UserInput[1000];
  static u16 au16Time[1000];
  static bool bflag1,bflag2,bflag3,bflag4,bflag5=FALSE;
  static bool bstate=TRUE;
  static bool bstart=TRUE;
  static LedCommandType command1;
  static LedCommandType command2;
  static u8 au8ColorMessage1[]={'W','P','B','C','G','Y','O','R'};
  static u8 au8ColorMessage2[]={WHITE,PURPLE,BLUE,CYAN,GREEN,YELLOW,ORANGE,RED};
  
  if(G_u8DebugScanfCharCount>0)
  {
    DebugScanf(au8UserInputBuffer);
    au16UserInput[u8index1]=au8UserInputBuffer[0];
    u8index1++;
    /*correct inaccurate action, just 1 and 2*/
    if(bstart)
    {
      if((au16UserInput[u8index1-1] == 0x0d) && ((au16UserInput[0]!='1') || (au16UserInput[0]!='2')))
      {
        for(u8index2=0;u8index2<u8index4;u8index2++)
          {
            au16UserInput[u8index2]='0';
          }
        DebugPrintf("Press 1 to program LED command sequence");
        DebugLineFeed();
        DebugPrintf("Press 2 to show current USER program");
        DebugLineFeed();
      }
    }
    
    /*execute operation1: LEDs function*/
    if(au16UserInput[0]=='1')
    {
      if(bstate)
      {
        DebugLineFeed(); 
        DebugPrintf("Enter commands as LED-ONTIME-OFFTIME and press Enter");
        DebugLineFeed(); 
        DebugPrintf("Time is in milliseconds, max 100 commands");
        DebugLineFeed(); 
        DebugPrintf("LED colors:R, O, Y, G, C, B, P, W");
        DebugLineFeed();
        DebugPrintf("Example: R-100-200 (Red on at 100ms and off at 200ms)");
        DebugLineFeed();
        DebugPrintf("Press Enter on blank line to end");
        DebugLineFeed();
        DebugPrintNumber(u8counter2);
        DebugPrintf(":");
        bstate=FALSE;
        bstart=FALSE;
      }
    
    
      /*the order 'enter' of exectioning operation*/
      if(au16UserInput[u8index1-1]==0x0d)
      {
            u8mode=1;
            DebugLineFeed();
      }
 
    /*determine if the command is correct*/
    if(u8mode == 1)
    {
      for(u8index2=0;u8index2<8;u8index2++)
      {
        if(au8ColorMessage1[u8index2]==au16UserInput[1])
        {
          bflag1=TRUE;
          u8color=u8index2;
        }
      }
        
      if(bflag1)
      {
        if(au16UserInput[2]=='-')
        {
          bflag2=TRUE;
        }
      }
      
      if(bflag2)
      {
        for(u8index2=3;u8index2<u8index1;u8index2++)
        {
          if(au16UserInput[u8index2]=='-')
          {
            u8position=u8index2;/*record the second'-' position*/
            u8digit1=u8position-3;/*record the first figure's digit*/
          }
        }

        for(u8index2=3;u8index2<u8position;u8index2++)
        {
          if((au16UserInput[u8index2]<=57)&&(au16UserInput[u8index2]>=48))
          {
            u8counter1++;
          }
          if(u8counter1==u8digit1)
          {
            u8counter1=0;
            bflag3=TRUE;
          }
        }
      }
      
      if(bflag3)
      {
        u8digit2=u8index1-u8position-2;/*record the second figure's digit*/
        for(u8index2=u8position+1;u8index2<u8index1-1;u8index2++)
        {
          if((au16UserInput[u8index2]<=57)&&(au16UserInput[u8index2]>=48))
          {
            u8counter1++;
          }
          if(u8counter1==u8digit2)
          {
            u8counter1=0;
            bflag4=TRUE;
          }
        }
      }
    
      /*calculate the ontime and offtime*/
      if(bflag4)
      {
        for(u8index2=3;u8index2<u8position;u8index2++)
        {
          au16Time[u8index2]=au16UserInput[u8index2]-48;
          for(u8index3=u8digit1-1;u8index3>0;u8index3--)
          {
            au16Time[u8index2]=au16Time[u8index2]*10;
          }
          u32ontime=u32ontime+au16Time[u8index2];
          u8digit1--;
        }
        
        for(u8index2=u8position+1;u8index2<u8index1-1;u8index2++)
        {
          au16Time[u8index2]=au16UserInput[u8index2]-48;
          for(u8index3=u8digit2-1;u8index3>0;u8index3--)
          {
            au16Time[u8index2]=au16Time[u8index2]*10;
          }
          u32offtime=u32offtime+au16Time[u8index2];
          u8digit2--;
        }
        
        if(u32offtime > u32ontime)
        {
          bflag5=TRUE;
        }
      }
      
      if(bflag5)
      {
        command1.eLED=au8ColorMessage2[u8color];
        command1.u32Time=u32ontime;
        command1.bOn=TRUE;
        command1.eCurrentRate=LED_PWM_50;
        
        command2.eLED=au8ColorMessage2[u8color];
        command2.u32Time=u32offtime;
        command2.bOn=FALSE;
        command2.eCurrentRate=LED_PWM_100;
      
        u8counter2++;
        DebugPrintNumber(u8counter2);
        DebugPrintf(":");
        LedDisplayAddCommand(USER_LIST,&command1);
        LedDisplayAddCommand(USER_LIST,&command2);
        u8index5=u8index1;
        u8counter++;
      }
      
      /*the command is wrong*/
      else if(au16UserInput[1]!=0x0d)
      {
        DebugPrintf("Invalid command: incorrect format. Please use L-ONTIME-OFFTIME");
        DebugLineFeed();
        DebugPrintNumber(u8counter2);
        DebugPrintf(":");
        u8counter1=0;
      }
      u8index4=u8index1;
      u32ontime=0;
      u32offtime=0;
      u8index1=1;
      u8mode=0;
      bflag1,bflag2,bflag3,bflag4,bflag5 = FALSE;
    
          
      /*no command and return to the meun*/
      if(au16UserInput[1]==0x0d) 
      { 
        for(u8index2=0;u8index2<u8index4;u8index2++)
        {
          au16UserInput[u8index2]='0';
        }
        bstate=TRUE;
        bstart=TRUE;
        u8index1=0;
        DebugLineFeed();
        DebugPrintf("Press 1 to program LED command sequence");
        DebugLineFeed();
        DebugPrintf("Press 2 to show current USER program");
        DebugLineFeed();
        DebugLineFeed();
      }
    }
  }
        
  /*exection operation2: current USER program*/
  if(au16UserInput[0]=='2')
  {
      DebugLineFeed();
      DebugPrintf("Current USER program");
      DebugLineFeed();
      DebugPrintf("LED  ON TIME  OFF TIME");
      DebugLineFeed();
      DebugPrintf("-----------------------");
      DebugLineFeed();
      u8mode = 2;
        
      for(u8index2=0;u8index2<u8counter;u8index2++)
      {
        LedDisplayPrintListLine(u8index2);
      }
      DebugPrintf("-----------------------");
      
      for(u8index2=0;u8index2<u8index5;u8index2++)
      {
        au16UserInput[u8index2]='0';
      }
  }
  
  /*jump out of the operation2 and return to the meun*/
  if(u8mode == 2)
  {
    if(au16UserInput[1]==0x0d)
    {
      u8index1=0;
      u8mode=0;
      DebugPrintf("Press 1 to program LED command sequence");
      DebugLineFeed();
      DebugPrintf("Press 2 to show current USER program");
      DebugLineFeed();
    }
  }
    
 }
    
  
  
} /* end UserApp1SM_Idle() */
                      
            
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
