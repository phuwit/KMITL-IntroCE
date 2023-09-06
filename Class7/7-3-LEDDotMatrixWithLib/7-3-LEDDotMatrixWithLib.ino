#include "LedControl.h"

LedControl lc=LedControl(11,13,10,1);  // DIN,CLK,CS,Number of LED Module

unsigned long delaytime=20;  // time to updates of the display

void setup() {
  int devices=lc.getDeviceCount();  // find no of LED modules
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {  // set up each device 
    lc.shutdown(address,false);
    lc.setIntensity(address,1);
    lc.clearDisplay(address);
  }
}

void loop() { 
  int devices=lc.getDeviceCount();  // find no of LED modules
  int address=0;
    
    for(int col=0;col<8;col++) {
        lc.setLed(address,0,col,true);
        delay(delaytime);
        lc.setLed(address,0,col,false);
        delay(delaytime);
    }

    for(int row=0;row<8;row++) {
        lc.setLed(address,row,7,true);
        delay(delaytime);
        lc.setLed(address,row,7,false);
        delay(delaytime);
    }

    for(int col=7;col>=0;col--) {
        lc.setLed(address,7,col,true);
        delay(delaytime);
        lc.setLed(address,7,col,false);
        delay(delaytime);
    }

    for(int row=8;row>=0;row--) {
        lc.setLed(address,row,0,true);
        delay(delaytime);
        lc.setLed(address,row,0,false);
        delay(delaytime);
    }

    // for(int col=0;col<8;col++) {
    // for(int address=0;address<devices;address++) {
    // delay(delaytime);
    // lc.setLed(address,row,col,true);
    // delay(delaytime);
    // lc.setLed(address,row,col,false);
    // }
    // }
}
