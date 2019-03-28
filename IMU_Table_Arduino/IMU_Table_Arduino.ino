#include <Arduino_FreeRTOS.h>
#include "Mailbox.h"

//  Global Definitions  /////////////////////////////////////////////////////////////

MailBox g_Mailbox;
RX_Message g_RX;
TX_Message g_TX;

//  Task Definitions  ///////////////////////////////////////////////////////////////

void Task_Main(void * pvParameters)
{
  //Infinite Loop
  while(1)
  {
    //Check for serial availability
    if (Serial.available())
      g_Mailbox.Set_RX_Event();
  
    //Record new command message if available
    if (g_Mailbox.RX_Ready())
    {
      g_RX = g_Mailbox.RX();
  
      //TODO
      //Update command data to the system as appropriate
    }
  
    //TODO
    //Once all sensor data has been updated, compile TX message
  
    //Update mailbox with new TX message
    g_Mailbox.Set_TX(g_TX);
  }
}

void Task_Mailbox(void * pvParameters)
{
  //Infinite Loop
  while(1)
  {
    //Run Mailbox frame
    MailBox::runFrame_USB(g_Mailbox);
  }
}

void Task_MotorControl(void * pvParameters)
{
  
}

//  Startup ////////////////////////////////////////////////////////////////////////

void setup() 
{
  //Start serial
  Serial.begin(9600);
  
  //Launch tasks
  xTaskCreate(Task_Main,  //Main Thread
              "Main",     //English name for humans
              100,        //Stack depth of 100 layers
              NULL,       //No parameters passed in
              1,          //Priority of 1
              NULL);      //No handle for this task

  xTaskCreate(Task_Mailbox, //Mailbox Thread
              "Mailbox",    //English name for humans
              100,          //Stack depth of 100 layers
              NULL,
              2,            //Priority of 2
              NULL);

  //Start scheduler to begin task execution
  vTaskStartScheduler();
}

void loop() 
{
  //Keep empty
  //Operations are completed by tasks
}
