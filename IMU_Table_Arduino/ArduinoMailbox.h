#ifndef ARDUINOMAILBOX_H
#define ARDUINOMAILBOX_H

#include <string.h>

#include "src/Arduino_freeRTOS/Arduino_FreeRTOS.h"
#include "src/Arduino_freeRTOS/task.h"
#include "src/Arduino_freeRTOS/semphr.h"

#include "src/Mailbox/Mailbox.h"

class ArduinoMailbox : MailBox
{
public:

//Public Functions
ArduinoMailbox();

static RX_Message & Get_RX();

static void Set_TX(TX_Message & oTX);

static SemaphoreHandle_t RX_Event();
static SemaphoreHandle_t RX_Ready();
static SemaphoreHandle_t TX_Ready();

static void Initialize();

//Tasks
static void Task_Main(void * vParameters);
static void Task_RX(void * vParameters);
static void Task_TX(void * vParameters);

private:

    //Private Functions
    int RX_USB(Letter_T & lLetter); //Subroutine for receiving data over USB
    int TX_USB(Letter_T & Letter);  //Subroutine for sending data over USB

    void RX_Specific(Letter_T & lLetter);   //Overload of pure virtual RX function within Mailbox
    void TX_Specific(Letter_T & lLetter);   //Overload of pure virtual TX function within Mailbox

    //Private Members
   
};

#endif
