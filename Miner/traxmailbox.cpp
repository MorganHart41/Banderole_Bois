#include "traxmailbox.h"
#include <unistd.h>


/**
* Deconstructor for Mailbox - closes serial port
*
*/
TraxMailbox::~TraxMailbox() {
    this->serPort.close();
}

bool TraxMailbox::getFinalCalScore(){
    return this->calSuccess;
}

int TraxMailbox::getSampleCount() {
    return this->sampleCount;
}

float TraxMailbox::getHeading() {
    return this->heading;
}

float TraxMailbox::getPitch() {
    return this->pitch;
}

float TraxMailbox::getRoll() {
    return this->roll;
}

/**
* setter for sample count
* @param: count value to set sampleCount to
*/
void TraxMailbox::setSampleCount(int count) {
    this->sampleCount = count;
}

/**
* Writes a command to the PNI TRAX using PNI Binary protocol.
*
* @param cmd The command to write to the sensor.
* @param payload The payload of the command to write.
* @param payload_len The length of data in bytes pointed to by the payload.
*
* @param Return zero upon success and -1 upon error.
*/
int TraxMailbox::write_command(const Command cmd, const uint8_t *payload, const uint16_t payload_len)
{
    //printf("%x\n", payload[1]);
    /*
    * Construct the PNI Binary packet protocol.
    */
    int ret = 0;
    uint16_t packet_len = payload_len + 5;
    uint8_t *packet = new uint8_t[packet_len];
    uint8_t *packet1 = &packet[1];
    uint8_t *packet2 = &packet[2];
    uint8_t *packet3 = &packet[3];
    uint8_t *packet4 = &packet[4];
    /*
    * Write the byte length into the first two bytes of the packet, and then
    * write the command FRAME_ID byte.
    */
    const uint16_t packet_len_ne = htons(packet_len);
    memcpy(packet, &packet_len_ne, 2);
    packet[2] = static_cast<uint8_t>(cmd);

    /*
    * Follow the packet data with the payload for the command.
    */
    memcpy(&packet[3], payload, payload_len);

    /*
    * Calculate the CRC-16 for the packet and append it to the end.
    */
    const uint16_t crc = htons(crc16(packet, packet_len - 2));
    memcpy(&packet[packet_len - 2], &crc, 2);

    /*
    * Write the data down the serial port.
    */

    // Trying to put data into array? - BS
    //uint8_t write_data[] = {*packet1, *packet2, *packet3, *packet4};
    // is this only sending first packet - packet[0] = 00? - BS

    serPort.write(packet, packet_len);      // Could add error checking?

    uint8_t packetContents[20];
    for (int i = 0; i < 20; i++)
    {
        packetContents[i] = packet[i];
        //printf("%x\n", packetContents[i]);
    }
    delete[] packet;

    return ret;
}


/**
* Reads a command from the sensor with a PNI Binary format.
*
* @param[out] resp The location to store the resulting response.
* @param[out] payload The location to store the payload of the command.
* @param max_payload_length The maximum data that can be written to payload.
*
* @return Zero upon success and -1 upon error.
*/
int TraxMailbox::read_command(Command &resp, uint8_t *payload, const uint16_t max_payload_length, const size_t responseSize)
{
    /*
    * Read the packet header (packet length stored as uint16_t)
    */
    uint8_t data[4096] = {0};
//    if (serial_port.Read(data, 2) != 2)
//    {
//        return -1;
//    }

    size_t test = serPort.read(data, responseSize);
    std::cout << "Debug: Read Attempt, Might Take Some Time" << std::endl;
    int i = 1;
    while (test == 0){
        std::cout << "Debug: Re-reading Attempt " << i << std::endl;
        usleep(100000); //0
        test = serPort.read(data, responseSize);
        i++;
    }
    std::cout << "Read In Size: " << test << std::endl;
    uint16_t packet_len;
    memcpy(&packet_len, &data, 2);
    packet_len = ntohs(packet_len);

    /*
    * Packets are limited to 4096 bytes in length.
    */

    if (packet_len > 4096)
    {
        std::cout << "READ_COMMAND: Fail 1" << std::endl;
        return -1;
    }

    /*
    * Read the remainder of the packet.
    */
//    if (serial_port.Read(&data[2], packet_len - 2) != packet_len - 2)
//    {
//        return -1;
//    }

    /*
    * Calculate and check the crc16 on the packet.
    */

    uint16_t crc;
    memcpy(&crc, &(data[packet_len - 2]), 2);
    crc = ntohs(crc);

    if (crc != crc16(data, packet_len - 2))
    {
        std::cout << "READ_COMMAND: Fail 2" << std::endl;
        return -1;
    }

    /*
    * Ensure that the data will fit within the provided buffer.
    */
    if (packet_len - 5 > max_payload_length)
    {
        std::cout << "READ_COMMAND: Fail 3" << std::endl;
        return -1;
    }

    /*
    * Copy the fields into the user-provided buffers.
    */
    memcpy(&resp, &data[2], 1);
    memcpy(payload, &data[3], packet_len - 5);

    // print what was just read / check if everything got put into the propper variables
//    printf("%x \n", resp);
//    for(int i = 0; i < packet_len - 5; i++) {
//        printf("%x \n", payload[i]);
//    }
//    printf("%x \n", crc);

    return 0;
}


/**
* CRC16 CCITT Calculator.
*
* @note Code was taken from:
* https://github.com/gtrafimenkov/pycrc16/blob/master/src/_crc16module.c
*
* @param data A pointer to the buffer to be summed.
* @param length The length of data in bytes pointed to by data.
*
* @return The final checksum value.
*/
static const uint16_t crc_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

uint16_t TraxMailbox::crc16(uint8_t *data, int length)
{
    uint16_t crc = 0x0000;
    for (int i = 0; i < length; ++i)
    {
        crc = ((crc << 8) & 0xFF00) ^ (crc_table[((crc >> 8) & 0xFF)
            ^ data[i]]);
    }

    return crc & 0xFFFF;
}


/**
* Copies a float from network endianness to host endianness.
*
* @param data A float stored in network endianness.
*
* @return A float stored in host endianness.
*/
float TraxMailbox::ntohf(float data)
{
    float result;

    /*
    * Check to see if host system is big-endian. If so, memcpy is functional.
    */
    if (htonl(47) == 47)
    {
        memcpy(&result, &data, 4);
    }
    else
    {
        char *result_ptr = reinterpret_cast<char *>(&result);
        char *data_ptr = reinterpret_cast<char *>(&data);
        result_ptr[0] = data_ptr[3];
        result_ptr[1] = data_ptr[2];
        result_ptr[2] = data_ptr[1];
        result_ptr[3] = data_ptr[0];
    }
    return result;
}

/**
  * Puts the TRAX into compas mode, sets filter taps to 0, set to no autosampling,
  * set mag and accell coeff set to 0, set number of cal points to 18 then save
  * setting.
  *
  * @return: 0 upon success, -1 upon failure
  *
 */
int TraxMailbox::initCal() {
    // Commands to set and get functional mode to compas
    Command SetFunctMode = kSetFunctionalMode;
    Command getFunctMode = kGetFunctionalMode;
    uint8_t functModePayload[1] = {0x0};    // payload to set functional mode to compas

    // Commands to set and get FIR filters
    Command setFIR = ksetFIRFilters;
    Command checkFIR = kSetFIRFiltersDone;
    uint8_t firPayloadSet[3] = {0x3, 0x1, 0x0};  // payload to set FIR filtersto 0

    // Commands to set and get config - autosampling
    Command setConfig = kSetConfig;
    Command checkConfig = kSetConfigDone;
    uint8_t autoPayloadSet[2] = {0xD, 0x0};    // payload for turning off autosampling

    // set and get mag and accel coeff set
    uint8_t magCoeffPayloadSet[5] = {0x12, 0x0, 0x0, 0x0, 0x0}; // payload to save to first coefff set
    uint8_t accelCoeffPayloadSet[5] ={0x13, 0x0, 0x0, 0x0, 0x0};  // payload to save to first coeff set

    // set and get total cal points
    uint8_t calPointPayloadSet[5] = {0xC, 0x0, 0x0, 0x0, 0x12};  // payload to set cal points to 18

    // Commands to set and get data components
    Command setDataComp = kSetDataComponents;
    uint8_t dataCompSet[4] ={0x3, 0x5, 0x18, 0x19}; // payload to set getData to heading, pitch and roll data

    // Commands to getModInfo
    Command getMod = kGetModInfo;

    // Commands to set aquisition modes
    Command setAcq = kSetAcqParams;
    Command checkAcq = kSetAcqParamsDone;
    uint8_t setAcqPayloadSet[10] = {0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 ,0x0, 0x0};
    Command getAcq = kGetAcqParams;
    uint8_t payloadgetAcq[10] = {0};

    // Commands to set heading, pitch and roll output during calibration off
    uint8_t HPRPayloadSet[2] = {0x10, 0x0};

    // command to stop continious mode
    Command stopCont = kStopContinuousMode;

    // Structures for read to fill with data
    Command readResp;         // read populates with frame ID of message just read
    uint8_t payloadRead[1] = {0};     // read function populates with response payload

    int success = 0;                                    // success variale for error handling

    // set functional mode to compas mode
    write_command(SetFunctMode, functModePayload, 1);
    write_command(getFunctMode, NULL, 0);
    success = read_command(readResp, payloadRead, 1, 6);  // Ensure that functional mode is set to compas (0)
    if (payloadRead[0] != 0) {
        success = -1;
    }

    // Set FIR filters to 0
    write_command(setFIR, firPayloadSet, 3);
    success = read_command(readResp, payloadRead, 0, 5);  // Ensure that setFIRFilters command finished
    if(readResp != checkFIR) {
        success = -1;
    }

    // set autosampling to false
    write_command(setConfig, autoPayloadSet, 2);
    success = read_command(readResp, payloadRead, 0, 5);  // Ensure setConfig command finished
    if(readResp != checkConfig) {
        success = -1;
    }

    // Set mag coeff to 0
    write_command(setConfig, magCoeffPayloadSet, 5);
    success = read_command(readResp, payloadRead, 0, 5);      // Ensure setConfig finished
    if(readResp != checkConfig) {
        success = -1;
    }

    // set accel coeff to 0
    write_command(setConfig, accelCoeffPayloadSet, 5);
    success = read_command(readResp, payloadRead, 0, 5);      // Ensure setConfig finished
    if(readResp != checkConfig) {
        success = -1;
    }

    // set total cal points to 18
    write_command(setConfig, calPointPayloadSet, 5);
    success = read_command(readResp, payloadRead, 0, 5);      // Ensure setConfig finished
    if(readResp != checkConfig) {
        success = -1;
    }

    // Set data components to heading, pitch, roll
    write_command(setDataComp, dataCompSet, 4);

    // Set to polled mode
    write_command(setAcq, setAcqPayloadSet, 10);
    success = read_command(readResp, payloadRead, 0, 5);
    if (readResp != checkAcq) {
        success = -1;
    }

    // set HPR during call off
    write_command(setConfig, HPRPayloadSet, 2);
    success = read_command(readResp, payloadRead, 0, 5);  // Ensure setConfig command finished
    if(readResp != checkConfig) {
        success = -1;
    }

    // turn off cont mode
    write_command(stopCont, NULL, 0);

    // DEBUG ****
//    uint8_t payloadTest[4096];
//    read_command(readResp, payloadTest, 100, 105);

    return success;
}

/**
  * Tells TRAX to save settings and calibration coefficients to non volatile memory
 */
int TraxMailbox::save() {
    // Commands to save data to non volitile mem
    Command save = kSave;
    Command saveDone = kSaveDone;

    // Structures for read to fill with data
    Command readResp;         // read populates with frame ID of message just read
    uint8_t payloadRead[1] = {0};     // read function populates with response payload

    int success = 0;

    // Send save command
    write_command(save, NULL, 0);
    read_command(readResp, payloadRead, 1, 6);
    // if save command sends back error, return -1
    if(payloadRead[0] == -1) {
        success = -1;
    }
    return success;
}

/**
  * Prompts TRAX to begin calibratoin procedure by sending the start cal command
 */
void TraxMailbox::startCal() {
    Command beginCal = kStartCal;   // command to start cal
    // payload to start cal in mag and accel mode
    uint8_t startCalPayload[4] = {0x0, 0x0, 0x0, 0x6E};

    uint8_t payloadRead[4] = {0};             // read function populates with response payload
    Command readResp;                       // read populates with frame ID of message just read

    uint8_t payloadTest[4096] = {0};

    // write start cal
    write_command(beginCal, startCalPayload, 4);

    read_command(readResp, payloadRead, 4, 9);  // (Should have payload of 4 and total length of 9)
//    read_command(readResp, payloadTest, 100, 105);

    //read_command(readResp, payloadRead, 4, 9); // testing

    // add in error checking if needed
}

/**
  * Commands TRAX to stop calibration
 */
void TraxMailbox::abortCal() {
    Command stopCal = kStopCal; // command to abort cal

    // send command to abort cal
    write_command(stopCal, NULL, 0);
}

/**
  * Commands TRAX to take a calibration point and check to ensure
  * that point was successfully taken
  *
  * @return: 0 upon success, -1 upon failure
  *
 */
int TraxMailbox::takePoint() {
    Command takeCalPoint = kTakeUserCalSample;  // command to take point
    Command getSampleCount = kUserCalSampleCount;

    Command readResp;                       // read populates with frame ID of message just read
    uint8_t payloadRead[4] = {0};             // read function populates with response payload

    int success = 0;

    // write command to take calibration point
    write_command(takeCalPoint, NULL, 0);

    while(readResp != getSampleCount){
        success = read_command(readResp, payloadRead, 4, 9);  // Ensure cal point was taken
    }
    // Read back user cal sample count and ensure it is one higher then the last point taken
    if((int)payloadRead[3] == (this->sampleCount + 1)) {
        this->sampleCount++;
        if(this->sampleCount == 18) {
            // All 18 calibration points taken so read for cal score
            success = getCalScore();
        }
    } else {
        success = -1;   // cal point was not taken
    }

    return success;
}

int TraxMailbox::getCalScore(){
    std::cout << "********************************************************" << std::endl;
    std::cout << "Entering getCalScore" << std::endl;
    Command calScore = kUserCalScore;      // get call score command

    Command readResp;                      // read populates with frame ID of message just read
    uint8_t payloadRead[24] = {0};             // read function populates with response payload

    int success = 0;

    success = read_command(readResp, payloadRead, 24, 29);   // ** LOOK AT THIS LATER - 6 should be 24? ***

    uint8_t accelScore[4];
    uint8_t magScore[4];
    // call split cal score funtion to parse cal score
    splitCalScore(payloadRead, accelScore, magScore);
    float accelFloat = createFloat(accelScore);     // convert accel score data to float
    float magFloat = createFloat(magScore);         // convert mag score data to float


    std::cout << "Accel Float Score: " << accelFloat << std::endl;
    std::cout << "Mag Float Score: " << magFloat <<std::endl;
    // update calSuccess bool based on success of calibration
    if(accelFloat <= 2.0 && magFloat <= 2.0) {
        this->calSuccess = true;
    }
    else {
        this->calSuccess = false;
    }

    return success;
}

/**
  * Requests data in form of heading, pitch and roll from TRAX
  * then calls helper function to parse data into ascii
  *
  *@return: 0 upon success, -1 upon failure
  *
 */
int TraxMailbox::getPosition() {

    // Commands to set and get data components
    Command getData = kGetData;

    Command readResp;                      // read populates with frame ID of message just read
    uint8_t payloadRead[16] = {0};             // read function populates with response payload

    int success = 0;       // variable to track success of data read

    // request heading, pitch and roll data from TRAX
    write_command(getData, NULL, 0);
    success = read_command(readResp, payloadRead, 16, 21);
    // if read works, parse data
    if(success != -1) {
        // Create arrays to hold hex heading pitch and roll data
        uint8_t heading[4];
        uint8_t pitch[4];
        uint8_t roll[4];

        splitGetData(payloadRead, heading, pitch, roll);  // get heading, pitch and roll from payload

        printf("%x \n", heading[0]);
        printf("%x \n", heading[1]);
        printf("%x \n", heading[2]);
        printf("%x \n", heading[3]);

        printf("%x \n", pitch[0]);
        printf("%x \n", pitch[1]);
        printf("%x \n", pitch[2]);
        printf("%x \n", pitch[3]);

        this->heading = createFloat(heading);             // update heading value
        this->pitch = createFloat(pitch);                 // update pitch value
        this->roll = createFloat(roll);                   // update roll value
    }

    return success;
}


/**
  * Sets all TRAX settings back to their default settings
  *
  * @return: 0 upon success, -1 upon failure
  *
 */
int TraxMailbox::setDefaultSettings() {
    // Commands to set and get functional mode to compas
    Command SetFunctMode = kSetFunctionalMode;
    Command getFunctMode = kGetFunctionalMode;
    uint8_t functModePayload[1] = {0x1};    // payload to set functional mode to compas

    // Commands to set and get FIR filters
    Command setFIR = ksetFIRFilters;
    Command checkFIR = kSetFIRFiltersDone;
    uint8_t firPayloadSet[3] = {0x3, 0x1, 0x0};  // payload to set FIR filtersto 0

    // Commands to save data to non volitile mem
    Command save = kSave;
    Command saveDone = kSaveDone;

    // Structures for read to fill with data
    Command readResp;         // read populates with frame ID of message just read
    uint8_t payloadRead[1] = {0};     // read function populates with response payload

    int success = 0;                                    // success variale for error handling

    // set functional mode to compas mode
    write_command(SetFunctMode, functModePayload, 1);
    write_command(getFunctMode, NULL, 0);
    success = read_command(readResp, payloadRead, 1, 6);  // Ensure that functional mode is set to AHRS mode (1)
    if (payloadRead[0] != 1) {
        success = -1;
    }

    // Set FIR filters to 0
    write_command(setFIR, firPayloadSet, 3);
    success = read_command(readResp, payloadRead, 0, 5);  // Ensure that setFIRFilters command finished
    if(readResp != checkFIR) {
        success = -1;
    }

    Command readResp2 = kGetModInfo;
    // Save settings to non volatile memory
    write_command(save, NULL, 0);
    success = read_command(readResp2, payloadRead, 2, 7);

    // if save command sends back error, return -1
    if(payloadRead[0] == 1) {
        success = -1;
    }

    return success;
}



float createFloat(uint8_t data[])
{
   std::bitset<8> byte1(data[0]); //putting the data recieved into 8 bit arrays
   std::bitset<8> byte2(data[1]);
   std::bitset<8> byte3(data[2]);
   std::bitset<8> byte4(data[3]);

   int s = byte1[7];

   int exponentBits[8] = {byte2[7], byte1[0], byte1[1], byte1[2], byte1[3], byte1[4], byte1[5], byte1[6]};
   float exponent = BitToDec(exponentBits, 8); //used to convert from bits to a decimal value
   //std::cout << exponent << std::endl;

   int mantissaBits[23] = {byte4[0],byte4[1],byte4[2],byte4[3],byte4[4],byte4[5],byte4[6],byte4[7],byte3[0],byte3[1],byte3[2],byte3[3],byte3[4],byte3[5],byte3[6],byte3[7],byte2[0],byte2[1],byte2[2],byte2[3],byte2[4],byte2[5],byte2[6]};
   float mantissa = createMantissa(mantissaBits);
   //std::cout << mantissa << std::endl;
   //float OnePointMantissa = shrinkMantissa(mantissa); //used to get mantissa down to a value less than 1
   //std::cout << OnePointMantissa << std::endl;

   float actualFloatValue = ((pow(2,(exponent - 127)))*(mantissa));

   if(s)
   {
       actualFloatValue = (actualFloatValue*(-1));
   }
   //std::cout << byte1[1] << std::endl;


   //std::cout << actualFloatValue << std::endl;

   return actualFloatValue;
   //printf("%d\n", s);
   //float number = ((-1)(data[0]))
}


float createMantissa(int *mantissa)
{
   float number = 0.000000000;
   for(int i = 0; i < 23; i++)
   {
       number += mantissa[22-i]*(1/pow(2, i+1));
   }
   number += 1;
   return number;
}

void splitCalScore(uint8_t data[], uint8_t * AccelScore, uint8_t *MagScore)
{
    int a = 0;
    for(int i = 0; i < 4; i++)
    {
        MagScore[i] = data[a];
        a++;
    }
    a = 8;
    for(int i = 0; i < 4; i++)
    {
        AccelScore[i] = data[a];
        a++;
    }
}


float BitToDec(int data[], int length)
{
    float sum = 0;
    for(int i = 0; i < length; i++)
    {
        sum += (float)(pow(2, i)*data[i]);
    }

    return sum;
}


void splitGetData(uint8_t data[], uint8_t *heading, uint8_t *pitch, uint8_t *roll)
{
    int a = 1;
    //heading[] = {0,0,0,0}
    //uint8_t pitch[4] = {0,0,0,0};
    //uint8_t roll[4] = {0,0,0,0};
    while(a < 15)
    {
        switch (data[a]) {
        case 5:


            for(int i = 0; i < 4; i++)
            {
                a++;
                heading[i] = data[a];
            }
            a++;
            break;
        case 24:

            for(int i = 0; i < 4; i++)
            {
                a++;
                pitch[i] = data[a];

            }
            a++;
            break;

        case 25:

            for(int i = 0; i < 4; i++)
            {
                a++;
                roll[i] = data[a];

            }
            a++;
            break;
        default:
            break;
        }
    }
}

int combine(int a, int b) {
   int times = 1;
   while (times <= b)
      times *= 10;
   return a*times + b;
}

int combineData(uint8_t data[])
{
    int first = combine(data[0],data[1]);
    int second = combine(data[2], data[3]);
    int final = combine(first, second);
    return final;
}
