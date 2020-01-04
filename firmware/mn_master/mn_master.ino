/*---------------------------------------------------------------------------*/
//  MARIONETTE system : MASTER device firmware
//
//  Filename      : mn_master.ino
//  Target system : MARIONETTE system
//  Target device : ATMEGA32U4
//
//  [Revision history]
//  Date        Version   Author            Detail
//  -------------------------------------------------------------------------
//  2018.07.15  1.00      TMC               Initial release.
/*---------------------------------------------------------------------------*/
#define receiveBufferLength 17
#define terminationCode     '\r'
#define promptCode          '$'
#define resultCode          '#'

/*---------------------------------------------------------------------------*/
//  Function prototype definition
/*---------------------------------------------------------------------------*/
void setup();
void loop();
void commandDigitalDirection();
void commandDigitalIn();
void commandDigitalOut();
void commandAnalogIn();
void commandSendToSlave();


/*---------------------------------------------------------------------------*/
//  CONST definition
/*---------------------------------------------------------------------------*/
const char messageFirmwareName[] = "MARIONETTE system firmware";
const char messageFirmwareVersion[] = "1.00";

const int portMasterDetect = 5;
const int portDipSw0 = 8;
const int portDipSw1 = 9;
const int portDipSw2 = 10;
const int portDipSw3 = 11;
const int portD3_PD0 = 3;
const int portD2_PD1 = 2;
const int portD0_PD2 = 0;
const int portD1_PD3 = 1;
const int portD4_PD4 = 4;
const int portD12_PD6 = 12;
const int portD6_PD7 = 6;
const int portD13_PC7 = 13;
const int portD7_PE6 = 7;
const int portA5_PF0 = 23;
const int portA4_PF1 = 22;
const int portA3_PF4 = 21;
const int portA2_PF5 = 20;
const int portA1_PF6 = 19;
const int portA0_PF7 = 18;


/*---------------------------------------------------------------------------*/
//  Global variable definition
/*---------------------------------------------------------------------------*/
unsigned char receiveBuffer[receiveBufferLength];
char receiveBytes;
char boardID;


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
  analogReference(DEFAULT);
  pinMode(portMasterDetect, INPUT_PULLUP);
  pinMode(portDipSw0, INPUT_PULLUP);
  pinMode(portDipSw1, INPUT_PULLUP);
  pinMode(portDipSw2, INPUT_PULLUP);
  pinMode(portDipSw3, INPUT_PULLUP);
  pinMode(portD3_PD0, INPUT_PULLUP);
  pinMode(portD2_PD1, INPUT_PULLUP);
  pinMode(portD0_PD2, INPUT_PULLUP);
  pinMode(portD1_PD3, INPUT_PULLUP);
  pinMode(portD4_PD4, INPUT_PULLUP);
  pinMode(portD12_PD6, INPUT_PULLUP);
  pinMode(portD6_PD7, INPUT_PULLUP);
  pinMode(portD13_PC7, INPUT_PULLUP);
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
  if (digitalRead(portMasterDetect) != HIGH) {
    /*----- Wrong firmware -----*/
    while (1) {
      // Dead loop
      TXLED1;
      RXLED0;
      delay(300);
      TXLED0;
      RXLED1;
      delay(300);    
    }
  }
  else
  {
    /*----- Right firmware -----*/
    TXLED1;
    RXLED1;
    delay(3000);
    TXLED0;
    RXLED0;
  }

  /*-------------------------------------------------------------------------*/
  //  Board ID Recognition
  /*-------------------------------------------------------------------------*/
  if (digitalRead(portDipSw3) == LOW) boardID = 8; else boardID = 0;
  if (digitalRead(portDipSw2) == LOW) boardID |= 4;
  if (digitalRead(portDipSw1) == LOW) boardID |= 2;
  if (digitalRead(portDipSw0) == LOW) boardID |= 1;
  
  /*-------------------------------------------------------------------------*/
  //  Serial initialization
  /*-------------------------------------------------------------------------*/
  Serial.begin(9600, SERIAL_8N1);   // for Host communication(USB-CDC)
  Serial1.begin(9600, SERIAL_8N1);  // for Slave communication
  Serial1.setTimeout(500);

  /*-------------------------------------------------------------------------*/
  //  Display initial message
  /*-------------------------------------------------------------------------*/
  Serial.println("");
  Serial.print("== ");
  Serial.print(messageFirmwareName);
  Serial.println(" ==");
  Serial.print("Version ");
  Serial.print(messageFirmwareVersion);
  Serial.print("   build "); Serial.print(__DATE__);
  Serial.print(" "); Serial.println(__TIME__);  
  
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

  unsigned char i;
  unsigned char terminated;
  int receiveCode;

  // d03ip   Direction
  // i03     Input
  // o03h    Output
  // a00     Analog input
  // s      Send slave

  /*-------------------------------------------------------------------------*/
  //  Monitor loop
  /*-------------------------------------------------------------------------*/
  /*----- Variable initialization -----*/
  receiveBytes = 0;
  terminated = 0;
  receiveCode = 0;
  for (i = 0; i < receiveBufferLength; i++) receiveBuffer[i] = '\0';

  /*----- Loop until receiving the termination code -----*/
  Serial.print(boardID, HEX);
  Serial.print(promptCode);
  while (terminated == 0) {
    if (Serial.available() != 0) {
      receiveCode = Serial.read();
      if (receiveCode == terminationCode) {
        Serial.println("");
        terminated = 1;
        break;
      }
      receiveBuffer[receiveBytes] = (unsigned char)receiveCode;
      if (receiveBytes < receiveBufferLength - 2) receiveBytes++;
      if (((0x00 <= receiveCode) && (receiveCode <= 0x1f)) || (receiveCode == 0x7f)) {
        Serial.print('^');
        Serial.print(receiveCode, DEC);
      }
      else
      {
        Serial.write(receiveCode);
      }
    }
  }

  /*----- Branch to command -----*/
  switch (receiveBuffer[0]) {
    case 'D':
    case 'd':
      commandDigitalDirection();
      break;
    case 'I':
    case 'i':
      commandDigitalIn();
      break;
    case 'O':
    case 'o':
      commandDigitalOut();
      break;
    case 'A':
    case 'a':
      commandAnalogIn();
      break;
    case 'S':
    case 's':
      commandSendToSlave();
      break;
    default:
      break;
  }
  Serial.print("Terminated. length = ");
  Serial.println(receiveBytes, DEC);
  for (i = 0; i < receiveBufferLength; i++) {
    Serial.println(receiveBuffer[i], HEX);
  }
  Serial.println("");
  
}


/*---------------------------------------------------------------------------*/
//  Processing of digital port direction setting command
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    None
/*---------------------------------------------------------------------------*/
void commandDigitalDirection() {

  char portMode;
  char portNumberChar[3];
  char portNumber;

  /*-------------------------------------------------------------------------*/
  //  Interpret buffer contents
  /*-------------------------------------------------------------------------*/
  /*----- Direction -----*/
  if ((receiveBuffer[3] == 'o') || (receiveBuffer[3] == 'O')) {
    portMode = OUTPUT;
  }
  else {
    if ((receiveBuffer[4] == 'p') || (receiveBuffer[3] == 'P')) portMode = INPUT_PULLUP; else portMode = INPUT;
  }

  /*----- Port number -----*/
  portNumberChar[0] = receiveBuffer[1];
  portNumberChar[1] = receiveBuffer[2];
  portNumberChar[2] = '\0';
  portNumber = atoi(portNumberChar);

  /*-------------------------------------------------------------------------*/
  //  Switch port mode
  /*-------------------------------------------------------------------------*/
  switch (portNumber) {
    case 2:
      pinMode(portD2_PD1, portMode);
      break;
    case 3:
      pinMode(portD3_PD0, portMode);
      break;
    case 4:
      pinMode(portD4_PD4, portMode);
      break;
    case 6:
      pinMode(portD6_PD7, portMode);
      break;
    case 7:
      pinMode(portD7_PE6, portMode);
      break;
    case 12:
      pinMode(portD12_PD6, portMode);
      break;
    case 13:
      pinMode(portD13_PC7, portMode);
      break;
    case 18:
      pinMode(portA0_PF7, portMode);
      break;
    case 19:
      pinMode(portA1_PF6, portMode);
      break;
    case 20:
      pinMode(portA2_PF5, portMode);
      break;
    case 21:
      pinMode(portA3_PF4, portMode);
      break;
    case 22:
      pinMode(portA4_PF1, portMode);
      break;
    case 23:
      pinMode(portA5_PF0, portMode);
      break;
    default:
      break;
  }

  /*-------------------------------------------------------------------------*/
  //  Display result
  /*-------------------------------------------------------------------------*/
  Serial.print(resultCode);
  Serial.print(portNumber, DEC);
  Serial.print(",");
  Serial.println(portMode, DEC);
  
}


/*---------------------------------------------------------------------------*/
//  Processing digital port input command
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    None
/*---------------------------------------------------------------------------*/
void commandDigitalIn() {

  int readResult;
  char portNumberChar[3];
  char portNumber;

  /*-------------------------------------------------------------------------*/
  //  Interpret buffer contents
  /*-------------------------------------------------------------------------*/
  /*----- Port number -----*/
  portNumberChar[0] = receiveBuffer[1];
  portNumberChar[1] = receiveBuffer[2];
  portNumberChar[2] = '\0';
  portNumber = atoi(portNumberChar);

  /*-------------------------------------------------------------------------*/
  //  Digital read
  /*-------------------------------------------------------------------------*/
  switch (portNumber) {
    case 2:
      readResult = digitalRead(portD2_PD1);
      break;
    case 3:
      readResult = digitalRead(portD3_PD0);
      break;
    case 4:
      readResult = digitalRead(portD4_PD4);
      break;
    case 6:
      readResult = digitalRead(portD6_PD7);
      break;
    case 7:
      readResult = digitalRead(portD7_PE6);
      break;
    case 12:
      readResult = digitalRead(portD12_PD6);
      break;
    case 13:
      readResult = digitalRead(portD13_PC7);
      break;
    case 18:
      readResult = digitalRead(portA0_PF7);
      break;
    case 19:
      readResult = digitalRead(portA1_PF6);
      break;
    case 20:
      readResult = digitalRead(portA2_PF5);
      break;
    case 21:
      readResult = digitalRead(portA3_PF4);
      break;
    case 22:
      readResult = digitalRead(portA4_PF1);
      break;
    case 23:
      readResult = digitalRead(portA5_PF0);
      break;
    default:
      break;
  }

  /*-------------------------------------------------------------------------*/
  //  Display result
  /*-------------------------------------------------------------------------*/
  Serial.print(resultCode);
  Serial.print(portNumber, DEC);
  Serial.print(",");
  if (readResult == HIGH) Serial.println("H"); else Serial.println("L");
  
}


/*---------------------------------------------------------------------------*/
//  Processing digital port output command
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    None
/*---------------------------------------------------------------------------*/
void commandDigitalOut() {

  int writeValue;
  char portNumberChar[3];
  char portNumber;

  /*-------------------------------------------------------------------------*/
  //  Interpret buffer contents
  /*-------------------------------------------------------------------------*/
  /*----- Port number -----*/
  portNumberChar[0] = receiveBuffer[1];
  portNumberChar[1] = receiveBuffer[2];
  portNumberChar[2] = '\0';
  portNumber = atoi(portNumberChar);
  /*----- Port logic -----*/
  if ((receiveBuffer[3] == 'h') || (receiveBuffer[3] == 'H')) writeValue = HIGH; else writeValue = LOW;

  /*-------------------------------------------------------------------------*/
  //  Digital write
  /*-------------------------------------------------------------------------*/
  switch (portNumber) {
    case 2:
      digitalWrite(portD2_PD1, writeValue);
      break;
    case 3:
      digitalWrite(portD3_PD0, writeValue);
      break;
    case 4:
      digitalWrite(portD4_PD4, writeValue);
      break;
    case 6:
      digitalWrite(portD6_PD7, writeValue);
      break;
    case 7:
      digitalWrite(portD7_PE6, writeValue);
      break;
    case 12:
      digitalWrite(portD12_PD6, writeValue);
      break;
    case 13:
      digitalWrite(portD13_PC7, writeValue);
      break;
    case 18:
      digitalWrite(portA0_PF7, writeValue);
      break;
    case 19:
      digitalWrite(portA1_PF6, writeValue);
      break;
    case 20:
      digitalWrite(portA2_PF5, writeValue);
      break;
    case 21:
      digitalWrite(portA3_PF4, writeValue);
      break;
    case 22:
      digitalWrite(portA4_PF1, writeValue);
      break;
    case 23:
      digitalWrite(portA5_PF0, writeValue);
      break;
    default:
      break;
  }

  /*-------------------------------------------------------------------------*/
  //  Display result
  /*-------------------------------------------------------------------------*/
  Serial.print(resultCode);
  Serial.print(portNumber, DEC);
  Serial.print(",");
  Serial.println(writeValue, DEC);
  
}


/*---------------------------------------------------------------------------*/
//  Processing analog port input command
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    None
/*---------------------------------------------------------------------------*/
void commandAnalogIn() {

  int readResult;
  char portNumberChar[3];
  char portNumber;

  /*-------------------------------------------------------------------------*/
  //  Interpret buffer contents
  /*-------------------------------------------------------------------------*/
  /*----- Port number -----*/
  portNumberChar[0] = receiveBuffer[1];
  portNumberChar[1] = receiveBuffer[2];
  portNumberChar[2] = '\0';
  portNumber = atoi(portNumberChar);

  /*-------------------------------------------------------------------------*/
  //  Analog read
  /*-------------------------------------------------------------------------*/
  switch (portNumber) {
    case 0:
      readResult = analogRead(0);
      break;
    case 1:
      readResult = analogRead(1);
      break;
    case 2:
      readResult = analogRead(2);
      break;
    case 3:
      readResult = analogRead(3);
      break;
    case 4:
      readResult = analogRead(4);
      break;
    case 5:
      readResult = analogRead(5);
      break;
    case 6:
      readResult = analogRead(6);
      break;
    case 7:
      readResult = analogRead(7);
      break;
    case 8:
      readResult = analogRead(8);
      break;
    case 9:
      readResult = analogRead(9);
      break;
    case 10:
      readResult = analogRead(10);
      break;
    case 11:
      readResult = analogRead(11);
      break;
    default:
      break;
  }

  /*-------------------------------------------------------------------------*/
  //  Display result
  /*-------------------------------------------------------------------------*/
  Serial.print(resultCode);
  Serial.print(portNumber, DEC);
  Serial.print(",");
  Serial.println(readResult, DEC);
  
}


/*---------------------------------------------------------------------------*/
//  Processing of slave device transmission command
/*---------------------------------------------------------------------------*/
//  [Arguments]
//    None
//  [Returns]
//    None
//  [Note]
//    None
/*---------------------------------------------------------------------------*/
void commandSendToSlave() {

  int i;

  Serial.print(resultCode);
  
  for (i = 1; i <= receiveBufferLength - 1; i++) {
    if (receiveBuffer[i] != '\0') {
      Serial1.write(receiveBuffer[i]);
      Serial.print('^');
      Serial.print(receiveBuffer[i], DEC);
    }
    else break;
  }
  Serial.println("");
  
}

