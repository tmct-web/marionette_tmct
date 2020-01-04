/*---------------------------------------------------------------------------*/
//  MARIONETTE system : SLAVE device : Boot keyboard sample
//
//  Filename      : mn_slave_bootkb.ino
//  Target system : MARIONETTE system
//  Target device : ATMEGA32U4
//
//  [Revision history]
//  Date        Author                Detail
//  -------------------------------------------------------------------------
//  2018.08.04  TMC                   Initial release.
/*---------------------------------------------------------------------------*/
#include <HID-Project.h>
#include <HID-Settings.h>


/*---------------------------------------------------------------------------*/
//  CONST definition
/*---------------------------------------------------------------------------*/
const int portMasterDetect = 5;
const int portIO8_PB4 = 8;
const int portIO9_PB5 = 9;
const int portIO10_PB6 = 10;
const int portIO11_PB7 = 11;
const int portD3_PD0 = 3;
const int portD2_PD1 = 2;
const int portD0_PD2 = 0;
const int portD1_PD3 = 1;
const int portD4_PD4 = 4;
const int portD12_PD6 = 12;
const int portD6_PD7 = 6;
const int portD7_PE6 = 7;
const int portA5_PF0 = 23;
const int portA4_PF1 = 22;
const int portA3_PF4 = 21;
const int portA2_PF5 = 20;
const int portA1_PF6 = 19;
const int portA0_PF7 = 18;


/*---------------------------------------------------------------------------*/
//  Setup function
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    Arduino setup function
/*---------------------------------------------------------------------------*/
void setup() {
  
  /*-------------------------------------------------------------------------*/
  //  Port initialization
  /*-------------------------------------------------------------------------*/
  pinMode(portMasterDetect, INPUT_PULLUP);
  pinMode(portIO8_PB4, INPUT_PULLUP);
  pinMode(portIO9_PB5, INPUT_PULLUP);
  pinMode(portIO10_PB6, INPUT_PULLUP);
  pinMode(portIO11_PB7, INPUT_PULLUP);
  pinMode(portD3_PD0, INPUT_PULLUP);
  pinMode(portD2_PD1, INPUT_PULLUP);
  pinMode(portD0_PD2, INPUT_PULLUP);
  pinMode(portD1_PD3, INPUT_PULLUP);
  pinMode(portD4_PD4, INPUT_PULLUP);
  pinMode(portD12_PD6, INPUT_PULLUP);
  pinMode(portD6_PD7, INPUT_PULLUP);
  pinMode(portD7_PE6, INPUT_PULLUP);
  pinMode(portA5_PF0, INPUT_PULLUP);
  pinMode(portA4_PF1, INPUT_PULLUP);
  pinMode(portA3_PF4, INPUT_PULLUP);
  pinMode(portA2_PF5, INPUT_PULLUP);
  pinMode(portA1_PF6, INPUT_PULLUP);
  pinMode(portA0_PF7, INPUT_PULLUP);
  TXLED0;
  RXLED0;
  
  /*-------------------------------------------------------------------------*/
  //  Master and Slave detection
  /*-------------------------------------------------------------------------*/
  if (digitalRead(portMasterDetect) != LOW) {
      while (1) {
        TXLED1;
        RXLED0;
        delay(300);
        TXLED0;
        RXLED1;
        delay(300);    
    }
  }

  /*-------------------------------------------------------------------------*/
  //  Serial initialization
  /*-------------------------------------------------------------------------*/
  Serial1.begin(9600, SERIAL_8N1);  // for Master communication
  Serial1.setTimeout(500);
  
  /*-------------------------------------------------------------------------*/
  //  Sends a clean report to the host
  /*-------------------------------------------------------------------------*/
  BootKeyboard.begin();
  
}

/*---------------------------------------------------------------------------*/
//  Loop function
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    Arduino loop function
/*---------------------------------------------------------------------------*/
void loop() {

  if (Serial1.available() != 0) {
    BootKeyboard.write(Serial1.read());
  }

}
