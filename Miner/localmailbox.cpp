#include "localmailbox.h"


bool LocalMailbox::readRX(){
    try{
        uint8_t received[57];
        serPort1.read(received,57);
        printf("READING IN: ");
        std::cout << received << std::endl;;

        //Ignore First 7 [6]

        uint8_t tempPos[4];     // array to hold x, y, z pos

        // Put pos x into tempPos
        tempPos[0] = received[19];
        tempPos[1] = received[20];
        tempPos[2] = received[21];
        tempPos[3] = received[22];
        printf("bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        //Convert bytes To Float
        receivedArduinoX = bytesToFloat(tempPos);
        // Put pos y into tempPos
        tempPos[0] = received[23];
        tempPos[1] = received[24];
        tempPos[2] = received[25];
        tempPos[3] = received[26];
        printf("bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        //Convert bytes To Float
        receivedArduinoY = bytesToFloat(tempPos);
        // Put pos z into tempPos
        tempPos[0] = received[27];
        tempPos[1] = received[28];
        tempPos[2] = received[29];
        tempPos[3] = received[30];
        printf("bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        //Convert bytes To Float
        receivedArduinoZ = bytesToFloat(tempPos);

//---------------------------VELOCITY------------------------
        // Put vel x into tempPos
        tempPos[0] = received[31];
        tempPos[1] = received[32];
        tempPos[2] = received[33];
        tempPos[3] = received[34];
        //Convert bytes To Float
        receivedVelocityX = bytesToFloat(tempPos);
        // Put vel y into tempPos
        tempPos[0] = received[35];
        tempPos[1] = received[36];
        tempPos[2] = received[37];
        tempPos[3] = received[38];
        //Convert bytes To Float
        receivedVelocityY = bytesToFloat(tempPos);
        // Put vel z into tempPos
        tempPos[0] = received[39];
        tempPos[1] = received[40];
        tempPos[2] = received[41];
        tempPos[3] = received[42];
        //Convert bytes To Float
        receivedVelocityZ = bytesToFloat(tempPos);

        return true;
    }
    catch(...){
        return false;
    }
}

bool LocalMailbox::sendTX(){
    try{

        uint8_t send[33];
        uint8_t tempPos[4];

        //Sequence Number
        uint8_t tempSequence[4];
        computeSequence(tempSequence);
        send[0] = tempSequence[0];
        send[1] = tempSequence[1];
        send[2] = tempSequence[2];
        send[3] = tempSequence[3];

        // Mailbox status - assuming FF (might have to change to F)
        send[4] = 0xFF;

        // total message length
        send[5] = 0x21;

        //stStatus
        send[6] = 0xF;

        //Convert pos x float to byte
        floatToBytes(arduinoX, tempPos);
        // set pos x data
        send[7] = tempPos[0];
        send[8] = tempPos[1];
        send[9] = tempPos[2];
        send[10] = tempPos[3];
        //Convert pos y float to byte
        floatToBytes(arduinoY, tempPos);
        // set pos y data
        send[11] = tempPos[0];
        send[12] = tempPos[1];
        send[13] = tempPos[2];
        send[14] = tempPos[3];
        //Convert pos z float to byte
        floatToBytes(arduinoZ, tempPos);
        // set pos z data
        send[15] = tempPos[0];
        send[16] = tempPos[1];
        send[17] = tempPos[2];
        send[18] = tempPos[3];

        // Convert vel x to bytes
        floatToBytes(velocityX, tempPos);
        // set pos x data
        send[19] = tempPos[0];
        send[20] = tempPos[1];
        send[21] = tempPos[2];
        send[22] = tempPos[3];
        // Convert vel y to bytes
        floatToBytes(velocityY, tempPos);
        // set pos y data
        send[23] = tempPos[0];
        send[24] = tempPos[1];
        send[25] = tempPos[2];
        send[26] = tempPos[3];
        // Convert vel z to bytes
        floatToBytes(velocityZ, tempPos);
        // set pos z data
        send[27] = tempPos[0];
        send[28] = tempPos[1];
        send[29] = tempPos[2];
        send[30] = tempPos[3];

        // CRC (setting to FF for now)
        send[31] = 0xFF;

        // stop bit
        send[32] = 0xE7;

        //Actual Send
        serPort1.write(send,33);
        return true;
    }
    catch(...){
        return false;
    }
}

void LocalMailbox::computeSequence(uint8_t sequenceArray[4]){
    sequenceNum++;
    int tempInt = this->sequenceNum;        // temp int to store current private sequence number

    // Shift all bits into correct section of byte array
    sequenceArray[0] = (tempInt >> 24) & 0xFF;
    sequenceArray[1] = (tempInt >> 16) & 0xFF;
    sequenceArray[2] = (tempInt >> 8) & 0xFF;
    sequenceArray[3] = tempInt & 0xFF;

    //printf("%x %x %x %x\n", sequenceArray[0], sequenceArray[1], sequenceArray[2], sequenceArray[3]);
}

float bytesToFloat(uint8_t bytes[4]) {
    float tempFloat = 0;

    // type cast byte array into float
    tempFloat = *(float*)(bytes);
    //printf("tempFloat = %f\r\n", tempFloat);
    return tempFloat;
}

void floatToBytes(float passedFloat, uint8_t bytes[4]) {
    *(float*)(bytes) = passedFloat;
    //printf("bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", bytes[0], bytes[1], bytes[2], bytes[3]);
}
