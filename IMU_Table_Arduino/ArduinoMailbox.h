#ifndef ARDUINOMAILBOX_H
#define ARDUINOMAILBOX_H

#include <string.h>
#include <Arduino_FreeRTOS.h>
#include "Mailbox.h"

class ArduinoMailbox : MailBox
{
public:

//Public Functions
ArduinoMailbox();

SemaphoreHandle_t RX_Event()  { return semRX_Event; }
SemaphoreHandle_t RX_Ready()  { return semRX_Ready; }
SemaphoreHandle_t TX_Ready()  { return semTX_Ready; }

void Initialize();

void Task_Main(void * vParameters);
void Task_RX(void * vParameters);
void Task_TX(void * vParameters);

private:

    //Private Functions
    int RX_USB(Letter_T & lLetter); //Subroutine for receiving data over USB

    void RX_Specific(Letter_T & lLetter);   //Overload of pure virtual RX function within Mailbox
    void TX_Specific(Letter_T & lLetter);   //Overload of pure virtual TX function within Mailbox
    void TX_USB(Letter_T & Letter);         //Subroutine for sending data over USB

    //Private Members
    SemaphoreHandle_t semRX_Event, semRX_Ready, semRX_Updated, semTX_Ready, semTX_Updated;
};

#endif