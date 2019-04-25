#include "sendit.h"

bool SendIt::readRX(){
    try{

        uint8_t received[57];
        serPort1.read(received,57);
        printf("READING IN: \n");
        printf("seq = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[0], received[1], received[2], received[3]);
        printf("mail/length/stat = [ 0x%x, 0x%x, 0x%x ]\r\n", received[4], received[5], received[6]);
        printf("xcmd = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[7], received[8], received[9], received[10]);
        printf("ycmd = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[11], received[12], received[13], received[14]);
        printf("zcmd = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[15], received[16], received[17], received[18]);
        printf("xat = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[19], received[20], received[21], received[22]);
        printf("yat = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[23], received[24], received[25], received[26]);
        printf("zat = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[27], received[28], received[29], received[30]);
        printf("xvcmd = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[31], received[32], received[33], received[34]);
        printf("yvcmd = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[35], received[36], received[37], received[38]);
        printf("zvcmd = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[39], received[40], received[41], received[42]);
        printf("xvat = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[43], received[44], received[45], received[46]);
        printf("yvat = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[47], received[48], received[49], received[50]);
        printf("zvat = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", received[51], received[52], received[53], received[54]);
        printf("crc = [ 0x%x ]\r\n",received[55]);
        printf("stop = [ 0x%x ]\r\n",received[56]);
        printf("******************************************************************************");

        //Ignore First 19 [18]

        uint8_t tempPos[4];     // array to hold x, y, z pos

        // Put pos x into tempPos
        tempPos[0] = received[19];
        tempPos[1] = received[20];
        tempPos[2] = received[21];
        tempPos[3] = received[22];
        printf("\nX bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        //Convert bytes To Float
        receivedArduinoX = bytesToFloat(tempPos);

        // Put pos y into tempPos
        tempPos[0] = received[23];
        tempPos[1] = received[24];
        tempPos[2] = received[25];
        tempPos[3] = received[26];
        printf("Y bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        //Convert bytes To Float
        receivedArduinoY = bytesToFloat(tempPos);

        // Put pos z into tempPos
        tempPos[0] = received[27];
        tempPos[1] = received[28];
        tempPos[2] = received[29];
        tempPos[3] = received[30];
        printf("Z bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        //Convert bytes To Float
        receivedArduinoZ = bytesToFloat(tempPos);

        printf("X: %f, Y: %f, Z: %f\n",receivedArduinoX,receivedArduinoY,receivedArduinoZ);
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

bool SendIt::sendTX(){
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
        printf("PASSED X = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        // set pos x data
        send[7] = tempPos[0];
        send[8] = tempPos[1];
        send[9] = tempPos[2];
        send[10] = tempPos[3];

        //Convert pos y float to byte
        floatToBytes(arduinoY, tempPos);
        printf("PASSED Y = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
        // set pos y data
        send[11] = tempPos[0];
        send[12] = tempPos[1];
        send[13] = tempPos[2];
        send[14] = tempPos[3];

        //Convert pos z float to byte
        floatToBytes(arduinoZ, tempPos);
        printf("PASSED Z = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", tempPos[0], tempPos[1], tempPos[2], tempPos[3]);
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
        send[31] = 0x0;

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

void SendIt::computeSequence(uint8_t sequenceArray[4]){
    sequenceNum++;
    int tempInt = this->sequenceNum;        // temp int to store current private sequence number

    // Shift all bits into correct section of byte array
    sequenceArray[0] = (tempInt >> 24) & 0xFF;
    sequenceArray[1] = (tempInt >> 16) & 0xFF;
    sequenceArray[2] = (tempInt >> 8) & 0xFF;
    sequenceArray[3] = tempInt & 0xFF;

    //printf("%x %x %x %x\n", sequenceArray[0], sequenceArray[1], sequenceArray[2], sequenceArray[3]);
}

float SendIt::bytesToFloat(uint8_t bytes[4]) {
    float tempFloat = 0;
    // type cast byte array into float
    tempFloat = *(float*)(bytes);
    //printf("tempFloat = %f\r\n", tempFloat);
    return tempFloat;
}

void SendIt::floatToBytes(float passedFloat, uint8_t bytes[4]) {
    *(float*)(bytes) = passedFloat;
    //printf("bytes = [ 0x%x, 0x%x, 0x%x, 0x%x ]\r\n", bytes[0], bytes[1], bytes[2], bytes[3]);
}

//Method For Delay In Seconds
void SendIt::delayBy(int amount){
    printf("DELAY");
    for(int i = 0; i < amount; i++){ //Sleep For 30 Seconds
        usleep(800000); //One Second Delay
    }
}



void SendIt::updateDesiredSubPosition(){
    SubPosition * nextPosition;
    nextPosition = allSubPositions.back();
    arduinoX = nextPosition->getPositionX();
    arduinoY = nextPosition->getPositionY();
    arduinoZ = nextPosition->getPositionZ();
    allSubPositions.pop_back();
}

bool SendIt::CSVInput(){
    std::fstream fin;
    printf("Uploading Data\n");
    float tempStep, tempX, tempY, tempZ;
    fin.open("/home/morgan/Desktop/Smoker/inputtedData.csv");
    int eof = fin.eof();

    std::string tempStr[4] = {""};
    while(eof != 1) {
        for (int i = 0; i < 4; i++) {
            if(i == 3) {
                getline(fin, tempStr[i], '\n');
            }
            else  {
                getline(fin, tempStr[i], ',');
            }
        }
        if(tempStr[0] != ""){
            SubPosition * step = new SubPosition(std::stof(tempStr[0]),std::stof(tempStr[1]),std::stof(tempStr[2]),std::stof(tempStr[3]));
            allSubPositions.push_front(step);
        }
        eof = fin.eof();
    }
    fin.close();
    printf("Exiting CSV\n");
    return true;
}

bool SendIt::comparePointValues(){
    if(arduinoX+.5 > receivedArduinoX && arduinoX-.5 < receivedArduinoX && arduinoY+.5 > receivedArduinoY && arduinoY-.5 < receivedArduinoY && arduinoZ+.5 > receivedArduinoZ && arduinoZ-.5 < receivedArduinoZ){
        return true;
    }
    return false;
}
