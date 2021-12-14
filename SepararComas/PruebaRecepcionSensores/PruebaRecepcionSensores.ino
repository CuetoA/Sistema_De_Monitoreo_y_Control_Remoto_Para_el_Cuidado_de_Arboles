#include <TimeLib.h>

void setup() {
  // put your setup code here, to run once:
  setTime(4, 53, 0, 12, 12, 2021);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int idArduino = 3;
  time_t fecha = now();
  int Prom_SensorT = 50;
  int Prom_SensorH = 76;
  String Mensaje_Mediciones = "RSP00011," +  (String)idArduino + ",1,0,1,0," + (String)day(fecha) + "," + (String)month(fecha) + "," + (String)year(fecha) + "," + (String)hour(fecha) + "," + (String)minute(fecha) + "," + (String)Prom_SensorT + "," + (String)Prom_SensorH + ",0,0,0,0";
  Serial.println(Mensaje_Mediciones);
  delay(2000);
}
