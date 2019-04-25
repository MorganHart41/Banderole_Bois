#include <QCoreApplication>
#include "localmailbox.h"
#include "wrapper.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Wrapper w;
    int loop = 0;
    int loop2 = 0;
    int i = 0;
    bool success = true;
    do{
        char userChoice = w.automatedOrManual();

            loop = 0;
        if(userChoice == 'a'){
            w.CSVInput();
            w.automatedLogic();
        }
        else if(userChoice == 'm'){
            do{
                loop2 = 0;
                char userChoice2 = w.accelOrMagOrAccelMag();

                if(userChoice2 == 'a'){
                    w.printMessage("SELECTED: Accel Only Calibration");
                    do{
                        i++;
                        if(i >= 10){
                            w.printMessage("The TRAX has failed initialization 10 times. Please power cycle the TRAX and try again");
                            return 0;
                        }
                        success = w.calibrationInit(userChoice2);
                    }while(!success);

                    w.accelCalibration();
                }
                else if(userChoice2 == 'm'){
                    w.printMessage("SELECTED: Mag Only Calibration");
                    do{
                        i++;
                        if(i >= 10){
                            w.printMessage("The TRAX has failed initialization 10 times. Please power cycle the TRAX and try again");
                            return 0;
                        }
                        success = w.calibrationInit(userChoice2);

                    }while(!success);
                    w.magCalibration();
                }
                else if(userChoice2 == 'o'){
                    w.printMessage("SELECTED: AccelMag Calibration");
                    do{
                        i++;
                        if(i >= 10){
                            w.printMessage("The TRAX has failed initialization 10 times. Please power cycle the TRAX and try again");
                            return 0;
                        }
                        success = w.calibrationInit(userChoice2);
                    }while(!success);
                    w.accelMagCalibration();
                }
                else
                {
                    loop2 = 1;
                    w.printMessage("That is not an accepted response");
                }
            }while(loop2);
        }
        else{
            w.printMessage("That is not an accepted response");
            loop = 1;
        }
    }while(loop);
}


