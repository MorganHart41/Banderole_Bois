#ifndef TRAXMAILBOX_H
#define TRAXMAILBOX_H

#endif // TRAXMAILBOX_H

#pragma comment(lib, "Ws2_32.lib")

//#include <Winsock2.h>
//#include <Winsock.h>

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <termios.h>
//#include <Windows.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <string>

//#include <libserialport.h>  // Found serial library online

/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "serial.h"
#include <math.h>
#include <bitset>

// testing
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

// Define TraxMailbox class
class TraxMailbox {
public:
    TraxMailbox() :
        serPort("/dev/ttyUSB0", 38400, serial::Timeout::simpleTimeout(80))
    {
        this->sampleCount = 0;
        this->calSuccess = false;
    }
    ~TraxMailbox();

    int initCal();              // Set TRAX settings in order to calibrate
    void startCal();            // Send the start calibration command
    void abortCal();            // Send the abort calibration command
    int takePoint();            // Take cal point
    int getCalScore();          // Request cal score from TRAX and set calSucess
    int getPosition();          // Request position data (heading, pitch, roll)
    int save();                 // Send save command
    int setDefaultSettings();   // Set TRAX settings back to defaults

    // Getters
    int getSampleCount();
    float getHeading();
    float getPitch();
    float getRoll();
    bool getFinalCalScore();
    // Setters
    void setSampleCount(int count);

private:
    enum Command : uint8_t
    {
        kGetModInfo = 0x01,
        kGetModInfoResp = 0x02,
        kSetDataComponents = 0x03,
        kGetData = 0x04,
        kGetDataResp = 0x05,
        kSetConfig = 0x06,
        kGetConfig = 0x07,
        kGetConfigResp = 0x08,
        kSave = 0x09,
        kStartCal = 0x0A,
        kStopCal = 0x0B,
        ksetFIRFilters = 0x0C,
        kGetFIRFilters = 0x0D,
        kGetFIRFiltersResp = 0x0E,
        kPowerDown = 0x0F,
        kSaveDone = 0x10,
        kUserCalSampleCount = 0x11,
        kUserCalScore = 0x12,
        kSetConfigDone = 0x13,
        kSetFIRFiltersDone = 0x14,
        kStartContinuousMode = 0x15,
        kStopContinuousMode = 0x16,
        kPowerUpDone = 0x17,
        kSetAcqParams = 0x18,
        kGetAcqParams = 0x19,
        kSetAcqParamsDone = 0x1A,
        kGetAcqParamsResp = 0x1B,
        kPowerDownDone = 0x1C,
        kFactoryMagCoeff = 0x1D,
        kFactoryMagCoeffDone = 0x1E,
        kTakeUserCalSample = 0x1F,
        kFactoryAccelCoeff = 0x24,
        kFactoryAccelCoeffDone = 0x25,
        kSetFunctionalMode = 0x4F,
        kGetFunctionalMode = 0x50,
        kGetFunctionalModeResp = 0x51,
        kSetResetRef = 0x6E,
        kSetMagThruthMethod = 0x77,
        kGetMagThruthMethod = 0x78,
        kGetMagThruthMethodResp = 0x79
    };

    serial::Serial serPort;  // Check port path matches if move systems
    int sampleCount;            // Variable to track the number of sample points taken
    bool calSuccess;            // Variable to track success of calibration
    float heading;           // array to hold heading value
    float pitch;             // array to hold pitch data
    float roll;              // array to hold roll data

    int write_command(const Command cmd, const uint8_t *payload, const uint16_t payload_len);
    int read_command(Command &resp, uint8_t *payload, const uint16_t max_payload_length, const size_t responseSize);
    uint16_t crc16(uint8_t *data, int length);
    float ntohf(float data);

};

float createFloat(uint8_t data[]);
void splitCalScore(uint8_t data[], uint8_t * AccelScore, uint8_t *MagScore);
float createMantissa(int *mantissa);
float BitToDec(int data[], int length);
void splitGetData(uint8_t data[], uint8_t *heading, uint8_t *pitch, uint8_t *roll);
int combine(int a, int b);
int combineData(uint8_t data[]);
void manualCal();
