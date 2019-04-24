#include <QCoreApplication>
#include "localmailbox.h"
#include "wrapper.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Wrapper w;

    char userChoice = w.automatedOrManual();

    if(userChoice == 'a'){
        w.CSVInput();
        w.automatedLogic();
    }
    else{
        char userChoice2 = w.accelOrMagOrAccelMag();

        if(userChoice2 == 'a'){
            w.printMessage("SELECTED: Accel Only Calibration");
            w.calibrationInit(userChoice2);
            w.accelCalibration();
        }
        else if(userChoice2 == 'm'){
            w.printMessage("SELECTED: Mag Only Calibration");
            w.calibrationInit(userChoice2);
            w.magCalibration();
        }
        else{
            w.printMessage("SELECTED: AccelMag Calibration");
            w.calibrationInit(userChoice2);
            w.accelMagCalibration();
        }
    }
}









/*SendIt s;
s.CSVInput();
s.updateDesiredSubPosition();
while(1){
    s.sendTX();
    s.delayBy(1);
    s.readRX();
    if(s.comparePointValues()){
        s.updateDesiredSubPosition();
    }

    std::cout << "=============================================================================" << std::endl;
}*/
