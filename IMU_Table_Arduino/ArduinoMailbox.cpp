#include "ArduinoMailbox.h"

//Semaphores for ArduinoMailbox task synchronization
static SemaphoreHandle_t semRX_Event, semRX_Ready, semRX_Updated, semTX_Ready, semTX_Updated;

//Global Arduino Mailbox
ArduinoMailbox g_Mailbox;

//  ArduinoMailbox -- Public Functions ////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    Constructor -- Default
*/

ArduinoMailbox::ArduinoMailbox()
{
}

/*-------------------------------------------------------------------------------------------------
    RX_Event

    Returns the semaphore handler for semRX_Event within the ArduinoMailbox.

    INPUT   --  NONE

    OUTPUT (void)
*/
static SemaphoreHandle_t ArduinoMailbox::RX_Event()
{
  return semRX_Event;
}

/*-------------------------------------------------------------------------------------------------
    RX_Ready

    Returns the semaphore handler for semRX_Ready within the ArduinoMailbox.

    INPUT   --  NONE

    OUTPUT (void)
*/
static SemaphoreHandle_t ArduinoMailbox::RX_Ready()
{
  return semRX_Ready;
}

/*-------------------------------------------------------------------------------------------------
    TX_Ready

    Returns the semaphore handler for semTX_Ready within the ArduinoMailbox.

    INPUT   --  NONE

    OUTPUT (void)
*/
static SemaphoreHandle_t ArduinoMailbox::TX_Ready()
{
  return semTX_Ready;
}

static RX_Message & ArduinoMailbox::Get_RX()
{
  return g_Mailbox.Get_RX();
}

static void ArduinoMailbox::Set_TX(TX_Message & oTX)
{
  g_Mailbox.Set_TX(oTX);
}

/*-------------------------------------------------------------------------------------------------
    Initialize

    Launches the three mailbox tasks.

    INPUT   --  NONE

    OUTPUT (void)
*/
static void ArduinoMailbox::Initialize()
{
    //Initialize semaphores
    semRX_Event = xSemaphoreCreateBinary();
    semRX_Ready = xSemaphoreCreateBinary();
    semRX_Updated = xSemaphoreCreateBinary();
    semTX_Ready = xSemaphoreCreateBinary();
    semTX_Updated = xSemaphoreCreateBinary();
  
    //Launch tasks
    xTaskCreate(Task_Main,      //Mailbox main thread
                "Mailbox_Main", //English name for humans
                0,              //Stack depth of minimum size allowed
                NULL,           //No parameters passed in
                3,              //Priority of 3
                NULL);   //No handler for this task

    xTaskCreate(Task_RX,        //Mailbox RX thread
                "Mailbox_RX",   //English name for humans
                100,            //Stack depth of 100 layers
                NULL,           //No parameters passed in
                2,              //Priority of 2
                NULL);     //No handler for this task

    xTaskCreate(Task_TX,        //Mailbox TX thread
                "Mailbox_TX",   //English name for humans
                100,            //Stack depth of 100 layers
                NULL,           //No parameters passed in
                2,              //Priority of 2
                NULL);     //No handler for this task
}

/*-------------------------------------------------------------------------------------------------
    Task_Main

    Main task for the mailbox. Handles the central statemachine.
        -   Pends upon a new RX message to update the state machine. Thus, this task is only executed
            once every RX/TX frame.

    INPUT (void *)  --  vParameters
                        Parameters to be passed to the task upon launch. Unused for this task.  

    OUTPUT (void)
*/
static void ArduinoMailbox::Task_Main(void * vParameters)
{
    //Infinite loop
    while(1)
    {
        //Pend on RX_Updated semaphore
        xSemaphoreTake(semRX_Updated, 0);

        //Update the mailbox statemachine
        g_Mailbox.updateStateMachine();

        //Post the TX_Updated semaphore
        xSemaphoreGive(semTX_Updated);
    }
}

/*-------------------------------------------------------------------------------------------------
    Task_RX

    RX task for the mailbox. Handles receiving incoming messages from the other device.
        -   Pends upon a new RX message to read-in and process the data. Thus, this task is only executed
            once every RX/TX frame.

    INPUT (void *)  --  vParameters
                        Parameters to be passed to the task upon launch. Unused for this task.

    OUTPUT (void)
*/
static void ArduinoMailbox::Task_RX(void * vParameters)
{
    //Infinite loop
    while(1)
    {
        //Pend on the RX_Event semaphore
        if(xSemaphoreTake(semRX_Event, pdMS_TO_TICKS(_LOC_TIMEOUT_MS)) == pdPASS)   //If the semaphore was grabbed
        {
            //RX message and process
            g_Mailbox.RX();
            g_Mailbox.Process_RX();

            xSemaphoreGive(semRX_Ready);
			      xSemaphoreGive(semRX_Updated);
        }
        else
            g_Mailbox.Induce_LOC();   //If we haven't received a message in 100ms, induce Loss of Coms
    }
}

/*-------------------------------------------------------------------------------------------------
    Task_TX

    TX task for the mailbox. Handles transmitting outgoing messages to the other device.
        -   Pends upon the state machine updating the TX state. Thus, this task is only executed once
            the mailbox main task has completed updating the state machine and will occur once per 
            RX/TX frame.
        -   Also pends upon the state of the TX message. The message will not send until it has been
            submitted to the mailbox and the TX_Ready semaphore is posted.
    
    INPUT (void *)  --  vParameters
                        Parameters to be passed to the task upon launch. Unused for this task.
    
    OUTPUT (void)
*/
static void ArduinoMailbox::Task_TX(void * vParameters)
{
    //Infinite loop
    while(1)
    {
        //Pend on the TX-Updated and TX_Ready semaphores. Order matters.
        xSemaphoreTake(semTX_Updated, 0);
        xSemaphoreTake(semTX_Ready, 0);

        //Process and transmit message
        g_Mailbox.Process_TX();
        g_Mailbox.TX();
    }
}

//  ArduinoMailbox -- Private Functions ///////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    RX_USB

    Subroutine for receiving a message from the other device via the Arduino's USB port.

    INPUT (Letter_T &)  --  lLetter
        An empty letter structure ready to be populated. Passed by reference.

    OUTPUT (int)
        Returns the number of characters that was read in through the serial event.
*/
int ArduinoMailbox::RX_USB(Letter_T & lLetter)
{
    //Else continue to fill up the buffer
    int i = 0;
    char cData[_RX_MESSAGE_LENGTH_NORMAL];

    //Grab letter headers
    while(Serial.available() && (i<2))
    {
        cData[i] = Serial.read();
        i++;
    }

    //If the header was read in correctly
    if(i == 2)
    {
        lLetter.cMessageType = (uint8_t)cData[0];
        lLetter.nMessageLength = (uint8_t)cData[1];
    }
    else
        return 0;

    //Read in cData and the tail of the letter
    while(Serial.available() && (i < lLetter.nMessageLength) && cData[i] != lLetter.nStopByte)
    {
        cData[i] = Serial.read();
        i++;
    }

    memcpy(lLetter.cData, &cData[2], nRX_Message_Length);       //Copy cData to the letter's cData
    lLetter.nCRC = (MERGE_16(cData[i-3], cData[i-2]) & 0x7FFF); //Load in the 15 bit CRC

    //Return the number of bytes read in
    return i;
}

/*-------------------------------------------------------------------------------------------------
    TX_USB

    Subroutine for transmitting a message to the other device via the Arduino's USB port.

    INPUT (Letter_T)  --  lLetter
        A populated letter structure that needs to be transmitted. Passed by reference.

    OUPUT (void)
*/
int ArduinoMailbox::TX_USB(Letter_T & lLetter)
{
    Serial.write(lLetter.cMessageType);
    Serial.write(lLetter.nMessageLength);
    Serial.write(lLetter.cData);
    Serial.write(lLetter.nCRC);
    Serial.write(lLetter.nStopByte);

    //Return the size of the letter plus the length of the cData string minus the size of the cData poiner
    return sizeof(lLetter) + strlen(lLetter.cData) - sizeof(lLetter.cData);
}

/*-------------------------------------------------------------------------------------------------
    RX_Specific

    Overloaded pure virtual function from Mailbox. Follows the specific protocols for receiving a
    message from the other device on the current operating platform.

    INPUT (Letter_T &)  --  lLetter
                            Unpopulated letter ready to be filled with the incoming data. Passed by
                            reference.

    OUTPUT (void)
*/
void ArduinoMailbox::RX_Specific(Letter_T & lLetter)
{
    nBytesReceived = RX_USB(lLetter);
}

/*-------------------------------------------------------------------------------------------------
    TX_Specific

    Overloaded pure virtual function from Mailbox. Follows the specific protocols for transmitting 
    a message to the other device on the current operating platform.

    INPUT (Letter_T &)  --  lLetter
                            Populated letter ready for transmission. Passed by reference.
    
    OUTPUT (void)
*/
void ArduinoMailbox::TX_Specific(Letter_T & lLetter)
{
    nBytesSent = TX_USB(lLetter);
}
