#ifndef SENDIT_H
#define SENDIT_H

#include "serial.h"
#include "subposition.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <list>

class LocalMailbox
{
public:

    LocalMailbox():
        serPort1("/dev/ttyACM0", 38400, serial::Timeout::simpleTimeout(80)){}

    /*********************************************************
    *                         GETTERS                        *
    **********************************************************/
    float getRXArduinoX(){return RXArduinoX;}
    float getRXArduinoY(){return RXArduinoY;}
    float getRXArduinoZ(){return RXArduinoZ;}
    float getRXVelocityX(){return RXVelocityX;}
    float getRXVelocityY(){return RXVelocityY;}
    float getRXVelocityZ(){return RXVelocityZ;}


    /*********************************************************
    *                         SETTERS                        *
    **********************************************************/
    void setTXArduinoX(float newArduinoX){TXArduinoX = newArduinoX;}
    void setTXArduinoY(float newArduinoY){TXArduinoY = newArduinoY;}
    void setTXArduinoZ(float newArduinoZ){TXArduinoZ = newArduinoZ;}
    void setTXVelocityX(float newVelocityX){TXVelocityX = newVelocityX;}
    void setTXVelocityY(float newVelocityY){TXVelocityY = newVelocityY;}
    void setTXVelocityZ(float newVelocityZ){TXVelocityZ = newVelocityZ;}

    /*********************************************************
    *                       OTHER MISC                       *
    **********************************************************/
    bool sendTX();
    bool readRX();
    void computeSequence(uint8_t sequenceArray[4]);
    void floatToBytes(float passedFloat, uint8_t bytes[4]);
    float bytesToFloat(uint8_t bytes[4]);
    void delayBy(int amount);

private:
    serial::Serial serPort1;  // Serial Port Decleration
    float TXArduinoX, TXArduinoY, TXArduinoZ; //For Send
    float RXArduinoX, RXArduinoY, RXArduinoZ; //For Received
    float TXVelocityX, TXVelocityY, TXVelocityZ;
    float RXVelocityX, RXVelocityY, RXVelocityZ;
    float sequenceNum;
    std::list <SubPosition *> allSubPositions;
};

#endif // SENDIT_H
