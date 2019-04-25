#include <QCoreApplication>
#include "sendit.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SendIt s;
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
    }
}
