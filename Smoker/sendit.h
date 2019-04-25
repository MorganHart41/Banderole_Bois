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

class SendIt
{
public:
    SendIt():
        serPort1("/dev/ttyACM0", 38400, serial::Timeout::simpleTimeout(80)){}
    bool sendTX();
    bool readRX();
    void computeSequence(uint8_t sequenceArray[4]);
    void floatToBytes(float passedFloat, uint8_t bytes[4]);
    float bytesToFloat(uint8_t bytes[4]);
    void delayBy(int amount);
    void updateDesiredSubPosition();
    bool CSVInput();
    bool comparePointValues();
private:
    serial::Serial serPort1;  // Check port path matches if move systems
    float arduinoX, arduinoY, arduinoZ; //For Send
    float receivedArduinoX, receivedArduinoY, receivedArduinoZ; //For Received
    float velocityX = 10.0;
    float velocityY = 10.0;
    float velocityZ = 10.0;
    float receivedVelocityX, receivedVelocityY, receivedVelocityZ;
    float sequenceNum;
    std::list <SubPosition *> allSubPositions;
};

#endif // SENDIT_H
