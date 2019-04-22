#ifndef LOCALMAILBOX_H
#define LOCALMAILBOX_H

#include "serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class LocalMailbox
{
public:
    LocalMailbox():
        serPort1("/dev/ttyACM0", 38400, serial::Timeout::simpleTimeout(80)){}

    /*********************************************************
    *                         SETTERS                        *
    **********************************************************/
    void setArduinoX(float newArduinoX){arduinoX = newArduinoX;}
    void setArduinoY(float newArduinoY){arduinoY = newArduinoY;}
    void setArduinoZ(float newArduinoZ){arduinoZ = newArduinoZ;}
    void setVelocityX(float newVelocityX){velocityX = newVelocityX;}
    void setVelocityY(float newVelocityY){velocityY = newVelocityY;}
    void setVelocityZ(float newVelocityZ){velocityZ = newVelocityZ;}

    /*********************************************************
    *                         GETTERS                        *
    **********************************************************/
    float getArduinoX(){return receivedArduinoX;}
    float getArduinoY(){return receivedArduinoY;}
    float getArduinoZ(){return receivedArduinoZ;}
    float getVelocityX(){return receivedVelocityX;}
    float getVelocityY(){return receivedVelocityY;}
    float getVelocityZ(){return receivedVelocityZ;}
    float getSequenceNum(){return sequenceNum;}


    /*********************************************************
    *                 LOCAL MAILBOX SPECIFIC                 *
    **********************************************************/
    bool sendTX();
    bool readRX();
    void computeSequence(uint8_t sequenceArray[4]);
private:
    serial::Serial serPort1;  // Check port path matches if move systems
    float arduinoX, arduinoY, arduinoZ; //For Send
    float receivedArduinoX, receivedArduinoY, receivedArduinoZ; //For Received
    float velocityX, velocityY, velocityZ;
    float receivedVelocityX, receivedVelocityY, receivedVelocityZ;
    float sequenceNum;
};

void floatToBytes(float passedFloat, uint8_t bytes[4]);
float bytesToFloat(uint8_t bytes[4]);

#endif // LOCALMAILBOX_H
