#ifndef WRAPPER_H
#define WRAPPER_H
/*********************************************************
*                      FILE INCLUDES                     *
**********************************************************/
#include "subposition.h"
#include "traxmailbox.h"
#include "localmailbox.h"

/*********************************************************
*                    LIBRARY INCLUDES                    *
**********************************************************/
#include <iostream>
#include <list>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fstream>

class Wrapper
{
public:
/*********************************************************
*                      CONSTRUCTORS                      *
**********************************************************/
    Wrapper();

/*********************************************************
*                         SETTERS                        *
**********************************************************/
    void setDesiredPositionX(float newDesiredPositionX){desiredPositionX = newDesiredPositionX;}
    void setDesiredPositionY(float newDesiredPositionY){desiredPositionY = newDesiredPositionY;}
    void setDesiredPositionZ(float newDesiredPositionZ){desiredPositionZ = newDesiredPositionZ;}
    void setArduinoPositionX(float newArduinoPositionX){currentPositionX = newArduinoPositionX;}
    void setArduinoPositionY(float newArduinoPositionY){currentPositionY = newArduinoPositionY;}
    void setArduinoPositionZ(float newArduinoPositionZ){currentPositionZ = newArduinoPositionZ;}
    void setStep(float newStep){step = newStep;}

/*********************************************************
*                         GETTERS                        *
**********************************************************/
    float getDesiredPositionX(){return desiredPositionX;}
    float getDesiredPositionY(){return desiredPositionY;}
    float getDesiredPositionZ(){return desiredPositionZ;}
    float getArduinoPositionX(){return currentPositionX;}
    float getArduinoPositionY(){return currentPositionY;}
    float getArduinoPositionZ(){return currentPositionZ;}
    float getStep(){return step;}

/*********************************************************
*                 WRAPPER SPECIFIC METHODS               *
**********************************************************/
    bool checkFinalStep(char userChoice);
    void updateDesiredSubPosition();
    bool comparePointValues();
    bool calibrationInit(char userChoice);
    void delayBy(int amount);
    void safteyDelay();
    void printMessage(std::string s);
    char automatedOrManual();
    void automatedLogic();
    bool CSVInput();
    void printArduinoData();
    void enterButtonDelay();
    void magCalibration();
    void accelCalibration();
    char accelOrMagOrAccelMag();
    void accelMagCalibration();

/*********************************************************
*                   TRAX USE METHODS                     *
**********************************************************/
    bool initTraxSettings(char userChoice);
    bool startTraxCalibration(char userChoice);
    bool takeTraxPoint(char passedCal);
    bool getTraxCalScore();
    bool setDefaultTraxSettings();

/*********************************************************
*                   LOCAL MAILBOX METHODS                *
**********************************************************/
    void getArduinoX();
    void getArduinoY();
    void getArduinoZ();
    void getAllArduinoXYZ();
    void setArduinoX(float newArduinoX);
    void setArduinoY(float newArduinoY);
    void setArduinoZ(float newArduinoZ);
    void setAllArduino();


private:
    //Used For Desired Sub Position
    float desiredPositionX, desiredPositionY, desiredPositionZ;

    //Used For Current Arduino Positioning
    float currentPositionX, currentPositionY, currentPositionZ;

    //Used For Desired Velocity
    float desiredVelocityX, desiredVelocityY, desiredVelocityZ;

    //Used For Current Velocity
    float velocityX, velocityY, velocityZ;

    //Current Step Variable
    float step;

    //Data Structure To Hold All Sub Positions
    std::list <SubPosition *> allSubPositions;

    //Private TraxMailbox Object Named tm
    TraxMailbox tm;

    //Private Mailbox Object Named lm
    LocalMailbox lm;

};

#endif // WRAPPER_H
