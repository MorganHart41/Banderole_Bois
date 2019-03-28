#include <Arduino_FreeRTOS.h>
#include "ArduinoMailbox.h"

//  Task Definitions  ///////////////////////////////////////////////////////////////

void Task_Main(void * pvParameters)
{
  //Mailbox task handler and message objects
  RX_Message rx;
  TX_Message tx;
  ArduinoMailbox mailbox;
  
  //Launch the mailbox task
  mailbox.Initialize();
  
  //Infinite Loop
  while(1)
  {
    //Check for serial availability
    if (Serial.available())                //If the serial RX flag has been set
      xSemaphoreGive(mailbox.RX_Event());    //Post the RX_Event semaphore for mailbox to receive the message
  
    //Check for new processed RX message
    if (xSemaphoreTake(mailbox.RX_Ready, 0) == pdPASS)  //If the RX_Ready semaphore for mailbox is posted
    {
      rx = g_Mailbox.RX();
  
      //TODO
      //Update state data to the system
      //Update command data to the system as appropriate
    }
  
    //TODO
    //Once all sensor and axis data has been updated, compile TX message
  
    //Update mailbox with new TX message, then post the TX_Ready semaphore for mailbox
    mailbox.Set_TX(tx);
    xSemaphoreGive(mailbox.TX_Ready());
  }
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

  //Start scheduler to begin task execution
  vTaskStartScheduler();
}

void loop() 
{
  //Keep empty
  //Operations are completed by tasks
}
