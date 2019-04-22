#include "wrapper.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    Wrapper w;
    w.CSVInput();

    char userChoice = w.automatedOrManual();

    if(userChoice == 'a'){
        w.automatedLogic();
    }
    else{
        w.manualLogic();
    }


    // test of sequence converter
    //LocalMailbox l;
    //while(1){
        //l.sendTX();
        //l.readRX();
    //}
}
