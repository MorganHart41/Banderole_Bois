#include "wrapper.h"

Wrapper::Wrapper(){}

/*********************************************************
*                 WRAPPER SPECIFIC METHODS               *
**********************************************************/

//Method Used To Check If We Are At The Final Step
bool Wrapper::checkFinalStep(){
    if(tm.getSampleCount() == 18.0){
        printMessage("At 18th Point");
        return true;
    }
    return false;
}

//Method Used For Updating The Desired Sub Position To The Next One
void Wrapper::updateDesiredSubPosition(){
    SubPosition * nextPosition;
    nextPosition = allSubPositions.back();
    desiredPositionX = nextPosition->getPositionX();
    desiredPositionY = nextPosition->getPositionY();
    desiredPositionZ = nextPosition->getPositionZ();
    allSubPositions.pop_back();
}

//Method Used To Compare If The Arduino Coordinated Are Within 1 Degree Of The Desired Sub Position
bool Wrapper::comparePointValues(){
    if(desiredPositionX+.5 > currentPositionX && desiredPositionX-.5 < currentPositionX && desiredPositionY+.5 > currentPositionY && desiredPositionY-.5 < currentPositionY && desiredPositionZ+.5 > currentPositionZ && desiredPositionZ-.5 < currentPositionZ){
        return true;
    }
    return false;
}

//Method For Calibration
bool Wrapper::calibrationInit(char userChoice){
    if(initTraxSettings()){
        delayBy(2);
        if(startTraxCalibration()){
            delayBy(2);
            return true;
        }
    }
    return false;
}

//Method For Delay In Seconds
void Wrapper::delayBy(int amount){
    printMessage("DELAY");
    for(int i = 0; i < amount; i++){ //Sleep For 30 Seconds
        usleep(1000000); //One Second Delay
    }
}

void Wrapper::safteyDelay(){
    printMessage("STEP BACK FROM TABLE: Acknowledge By Hitting Enter");
    enterButtonDelay();
    printMessage("STEP BACK");
    for(int i = 0; i < 5; i++){
        delayBy(1);
        //std::cout << ".";
}
    std::cout << std::endl;
}

//Method For Debug Print Statements
void Wrapper::printMessage(std::string s){
    std::cout << s << std::endl;
}

//Method To Delay Until Enter Button Is Hit
void Wrapper::enterButtonDelay() {
    printMessage("USER: Press Enter");
    while(1){
        if(std::cin.get() == '\n'){
            break;
        }
    }
}

//Method For Selecting Automated Or Manual Mode
char Wrapper::automatedOrManual(){
    printMessage("Please Enter 'a' For Automated Calibration or 'm' For Manual Calibration");
    std::string tempString;
    while(true){
        tempString = std::cin.get();
        if(tempString == "a"){
            printMessage("Automated Calibration Selected");
            return 'a';
        }
        else if (tempString == "m"){
            printMessage("Manual Calibration Selected");
            return 'm';
        }
        else{
            printMessage("Please Type Either 'm' or 'a'");
        }
    }
}

//Method For Automated Calibration Logic
void Wrapper::automatedLogic(){
    safteyDelay();
    calibrationInit('o');
    updateDesiredSubPosition();
    while(1){
        printMessage("********************************************************");
        setAllArduino();    // set position data from wrapper to local mail
        lm.sendTX();        // send command
        delayBy(1);
        lm.readRX();        // read command
        getAllArduinoXYZ();     // set new position data from mail to wrapper
        printArduinoData();

        //Update Variables To Wrapper Level
        if(comparePointValues()){
            if(takeTraxPoint('a')){
                if(checkFinalStep()){
                    printMessage("********************************************************");
                    if(getTraxCalScore()){
                        if(setDefaultTraxSettings()){
                            printMessage("Successfully Calibrated!!!");
                        }
                    }
                    else{
                        printMessage("Unsuccesfully Calibrated...");
                    }
                }
            }
            updateDesiredSubPosition();
        }
        else{
            printMessage("FALSE");
        }
    }
}

char Wrapper::accelOrMagOrAccelMag(){
    printMessage("Enter 'a' For Accel Calibration 'm' For Mag Calibration or 'am' For AccelMag Calibration");
    std::string tempString;
    while(true){
        tempString = std::cin.get();
        if(tempString == "a"){
            printMessage("Accel Calibration Selected");
            return 'a';
        }
        else if (tempString == "m"){
            printMessage("Mag Calibration Selected");
            return 'm';
        }
        else if (tempString == "am"){
            printMessage("AccelMag Calibration Selected");
            return 'o';
        }
        else{
            printMessage("Please Type Either 'm' or 'a' or 'am'");
        }
    }
}

void Wrapper::accelCalibration(){
    while(1){
        printMessage("********************************************************");
        enterButtonDelay();
        if(takeTraxPoint('a')){ //18
            if(checkFinalStep()){ //How Many Nums Here?
                printMessage("********************************************************");
                if(getTraxCalScore()){ //Change To Grab Variable?
                    if(setDefaultTraxSettings()){
                        std::cout << "Successfully Calibrated!!!" << std::endl;
                        break;
                    }
                }
                else{
                    std::cout << "Unsuccesfully Calibrated..." << std::endl;
                    break;
                }
            }
        }
    }
}

void Wrapper::magCalibration(){
    while(1){
        printMessage("********************************************************");
        enterButtonDelay();
        if(takeTraxPoint('m')){ //12
            if(tm.getSampleCount() == 12){
                printMessage("********************************************************");
                if(getTraxCalScore()){ //Change To Grab Variable?
                    if(setDefaultTraxSettings()){
                        std::cout << "Successfully MAG Calibrated!!!" << std::endl;
                        break;
                    }
                }
                else{
                    std::cout << "Unsuccesfully MAG Calibrated..." << std::endl;
                    break;
                }
            }
        }
    }
}

void Wrapper:: accelMagCalibration(){
    while(1){
        printMessage("********************************************************");
        enterButtonDelay();
        if(takeTraxPoint('a')){ //18
            if(checkFinalStep()){
                printMessage("********************************************************");
                if(getTraxCalScore()){ //Change To Grab Variable?
                    if(setDefaultTraxSettings()){
                        std::cout << "Successfully accelMAG Calibrated!!!" << std::endl;
                        break;
                    }
                }
                else{
                    std::cout << "Unsuccesfully accelMAG Calibrated..." << std::endl;
                    break;
                }
            }
        }
    }
}

bool Wrapper::CSVInput(){
    std::fstream fin;
    printMessage("Uploading Data");
    fin.open("/home/morgan/Desktop/BrickLayer"
             "/inputtedData.csv");
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
    printMessage("Exiting CSV");
    return true;
}

void Wrapper::printArduinoData(){
    printMessage("********************************************************");
    printf("Desired X, Y, Z Position: %f, %f, %f\n",desiredPositionX,desiredPositionY,desiredPositionZ);
    std::cout << "Current X Position:" << currentPositionX << std::endl;
    std::cout << "Current Y Position:" << currentPositionY << std::endl;
    std::cout << "Current Z Position:" << currentPositionZ << std::endl;

}

/*********************************************************
*                   TRAX USE METHODS                     *
**********************************************************/

//Method Used To Initialize Trax Settings And Handle Errors Thrown
bool Wrapper::initTraxSettings(){
    printMessage("********************************************************");
    printMessage("initCal: ");
    int results = tm.initCal();
    if(results == -1){
        std::cout << "Error: Init Of Calibration Failed" << std::endl;
        return false;
    }
    return true;
}

//Method Used For Starting The Trax Calibration Procedure
bool Wrapper::startTraxCalibration(){
    printMessage("********************************************************");
    printMessage("startCal: ");
    try{ //Try A startCal
        tm.startCal();
        return true; //Positive Return
    }
    catch(...){
        std::cout << "Error: Start Calibration Failed" << std::endl;
        return false; //Negative Return
    }
}

//Method For Taking A Point For The Trax
bool Wrapper::takeTraxPoint(char passedCal){
    int results = tm.takePoint();
    for(int i = 0; i < 10; i++){ //Loop For Attempting 10 Times
        if(results == 0){ //Check If Pass
            std::cout << "Taken Point: " << tm.getSampleCount() << std::endl;
            return true; //Positive Return
        }
    }
    std::cout << "Error: Take Point Failed (Attempted 10 Times)" << std::endl;
    return false; //Negative Return
}

//Method For Getting The Trax Calibration Score (Good Or Bad Eh)
bool Wrapper::getTraxCalScore(){
    if(tm.getFinalCalScore() == false){ //Check For Fail
        std::cout << "Error: Calibration Failed" << std::endl;
        return false;
    }
    return true;
}


//Will Reset The Default Settings For The Trax
bool Wrapper::setDefaultTraxSettings(){
    int results = tm.setDefaultSettings();
    if(results == -1){
        std::cout << "Error: Set Default Settings Failed" << std::endl;
        return false;
    }
    return true;
}

/*********************************************************
*                   LOCAL MAILBOX METHODS                *
**********************************************************/

//Method For Getting Arduino X Variable From LocalMailbox
void Wrapper::getArduinoX(){
    currentPositionX = lm.getRXArduinoX();
}

//Method For Getting Arduino Y Variable From LocalMailbox
void Wrapper::getArduinoY(){
    currentPositionY = lm.getRXArduinoY();
}

//Method For Getting Arduino Z Variable From LocalMailbox
void Wrapper::getArduinoZ(){
    currentPositionZ = lm.getRXArduinoZ();
}

//Method For Getting All Arduino XYZ
void Wrapper::getAllArduinoXYZ(){
    getArduinoX();
    getArduinoY();
    getArduinoZ();
}

//Method For Setting The ArduinoX Desired Variable
void Wrapper::setArduinoX(float newArduinoX){
    lm.setTXArduinoX(newArduinoX);
}

//Method For Setting The ArduinoY Desired Variable
void Wrapper::setArduinoY(float newArduinoY){
    lm.setTXArduinoY(newArduinoY);
}

//Method For Setting The ArduinoZ Desired Variable
void Wrapper::setArduinoZ(float newArduinoZ){
    lm.setTXArduinoZ(newArduinoZ);
}

void Wrapper::setAllArduino(){
    setArduinoX(desiredPositionX);
    setArduinoY(desiredPositionY);
    setArduinoZ(desiredPositionZ);
    lm.setTXVelocityX(60.0);
    lm.setTXVelocityY(60.0);
    lm.setTXVelocityZ(60.0);
}
