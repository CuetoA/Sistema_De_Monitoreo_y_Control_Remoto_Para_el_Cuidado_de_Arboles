/*
  Modbus RTU Server LED
  This sketch creates a Modbus RTU Server with a simulated coil.
  The value of the simulated coil is set on the LED
  Circuit:
   - MKR board
   - MKR 485 shield
     - ISO GND connected to GND of the Modbus RTU server
     - Y connected to A/Y of the Modbus RTU client
     - Z connected to B/Z of the Modbus RTU client
     - Jumper positions
       - FULL set to OFF
       - Z \/\/ Y set to OFF
  created 16 July 2018
  by Sandeep Mistry
*/

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

const int ledPin = 8;
const int RegMod = 0x64;
const int RegMod2 = 0x65;
const int RegMod3 = 0x66;
const int Id = 13;

long coilValue, coilValue2;
//ArduinoModbus ModbusRTUServer;
void setup() {
  Serial.begin(9600);

  Serial.println("Modbus RTU Server LED");

  // start the Modbus RTU server, with (slave) id 1
  if (!ModbusRTUServer.begin(Id, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // configure the LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // configure a single coil at address 0x00
  ModbusRTUServer.configureHoldingRegisters(RegMod, 3);
}

void loop() {
  // poll for Modbus RTU requests
  ModbusRTUServer.poll();
  long a;
  
  readHoldingRegisterValues();
  delay(60);  //Delay c
  writeHoldingRegisterValues(coilValue,coilValue2);
  
  //delay(50);
}

long readHoldingRegisterValues() {
    // read the current value of the coil
    //ModbusRTUServer.coilRead(RegMod2) = Serial.read();
    coilValue = ModbusRTUServer.holdingRegisterRead(RegMod);
    Serial.print(coilValue);
    Serial.print("  ");
    delay(55);  //delay a
    coilValue2 = ModbusRTUServer.holdingRegisterRead(RegMod2);
    Serial.print(coilValue2);
    Serial.println(" = ");
    delay(55);  //delay b
    //return coilValue, coilValue2;    
}

void writeHoldingRegisterValues(long coilValue,long coilValue2) {
  long a;
  //ModbusRTUServer.beginTransmission(HOLDING_REGISTERS, RegMod3, 1);
    if (coilValue==1 && coilValue2==1) {
      a=532;
      ModbusRTUServer.holdingRegisterWrite(RegMod3,a);
    }
    if (coilValue==0 && coilValue2==1) {
      a=1865;
      ModbusRTUServer.holdingRegisterWrite(RegMod3,a);
    }
    if (coilValue==1 && coilValue2==0) {
      a=47;
      ModbusRTUServer.holdingRegisterWrite(RegMod3,a);
    }
    else {
      a=14760;
      ModbusRTUServer.holdingRegisterWrite(RegMod3,a);
    }
    
    Serial.println(a);
    delay(10);  //delay d
   //int endTransmission();
  return a;
}
