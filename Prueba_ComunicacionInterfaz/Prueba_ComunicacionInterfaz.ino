
//Librerias de comunicación Modbus
#include <ArduinoRS485.h> 
#include <ArduinoModbus.h>
//Librerias de reloj
//#include <TimeLib.h>
//Librerias de sensor de Temperatura
//#include <OneWire.h>
//#include <DallasTemperature.h>

//Clases de OneWire y DallasTemperature
//OneWire oneW  ireObjeto(pinDatosT);
//DallasTemperature sensorDS18B20(&oneWireObjeto);

//Definicion de caracteristicas de esclavo
const int Id = 13;
const int Baud = 9600;

//Definicion de registros del SSF a SISMyC
const int SSF_NoConexion = 0x01;
const int SSF_EstadoAct = 0x02;
const int SSF_EstadoAnt = 0x03;
const int SSF_Modo = 0x04;
const int SSF_Confirmacion = 0x05;
const int SSF_Fechadd = 0x06;
const int SSF_Fechamm = 0x07;
const int SSF_Fechaaa = 0x08;
const int SSF_Horahh = 0x09;
const int SSF_Horamm = 0x0A;
const int SSF_Temperatura = 0x0B; //
const int SSF_Humedad = 0x0C;
const int SSF_NutrientesN = 0x0D; //
const int SSF_NutrientesP = 0x0E; //
const int SSF_NutrientesK = 0x0F; //
const int SSF_pH = 0x10;  //
//Copia de registros
int copy_SSF_NoConexion;
int copy_SSF_EstadoAct;
int copy_SSF_EstadoAnt;
int copy_SSF_Modo;
int copy_SSF_Confirmacion;
int copy_SSF_Fechadd;
int copy_SSF_Fechamm;
int copy_SSF_Fechaaa;
int copy_SSF_Horahh;
int copy_SSF_Horamm;
int copy_SSF_Temperatura; //
int copy_SSF_Humedad;
int copy_SSF_NutrientesN; //
int copy_SSF_NutrientesP; //
int copy_SSF_NutrientesK; //
int copy_SSF_pH;  //



//Definicion de registros del SISMyc a SSF
const int SISMyC_NoConexion = 0x11;
const int SISMyC_EstadoAct = 0x12;
const int SISMyC_EstadoAnt = 0x13;
const int SISMyC_Modo = 0x14;
const int SISMyC_Identificacion = 0x15;
const int SISMyC_Regar = 0x16;
const int SISMyC_Medir = 0x17;
const int SISMyC_Fertilizar = 0x18;
const int SISMyC_CantCiclosRiego = 0x19;
const int SISMyC_UnidadCiclosRiego = 0x1A;
const int SISMyC_CantCiclosMedicion = 0x1B;
const int SISMyC_UnidadCiclosMedicion = 0x1C;
const int SISMyC_CantAgua = 0x1D;
const int SISMyC_CantFertilizante = 0x1E;
const int SISMyC_RangoMinimoTemperatura = 0x1F;
const int SISMyC_RangoMaximoTemperatura = 0x20;
const int SISMyC_RangoMinimoHumedad = 0x21;
const int SISMyC_RangoMaximoHumedad = 0x22;
//Copias de Registros
int copy_SISMyC_NoConexion;
int copy_SISMyC_EstadoAct;
int copy_SISMyC_EstadoAnt;
int copy_SISMyC_Modo;
int copy_SISMyC_Identificacion;
int copy_SISMyC_Regar;
int copy_SISMyC_Medir;
int copy_SISMyC_Fertilizar;
int copy_SISMyC_CantCiclosRiego;
int copy_SISMyC_UnidadCiclosRiego;
int copy_SISMyC_CantCiclosMedicion;
int copy_SISMyC_UnidadCiclosMedicion;
int copy_SISMyC_CantAgua;
int copy_SISMyC_CantFertilizante;
int copy_SISMyC_RangoMinimoTemperatura;
int copy_SISMyC_RangoMaximoTemperatura;
int copy_SISMyC_RangoMinimoHumedad;
int copy_SISMyC_RangoMaximoHumedad;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(Baud);
  //setTime(20,0,0,19,3,2021);   //hr,mm,s,d,m,y
  if (!ModbusRTUServer.begin(Id, Baud)) { //Fallo de comunicacion
    while (1);
  }
  ModbusRTUServer.configureHoldingRegisters(SSF_NoConexion, 34);
//  sensorDS18B20.begin();   
}

void loop() {
  ModbusRTUServer.poll(); //Iniciar servidor
  delay(30);
  recibiendoConfiguracionesDeSSF();
  delay(30);
  impresion(); 
  //delay(60);
  //if(copy_SISMyC_NoConexion != 0){enviarDatosPrueba();}
}

int recibiendoConfiguracionesDeSSF() {  
    delay(110);
    copy_SISMyC_NoConexion = ModbusRTUServer.holdingRegisterRead(SISMyC_NoConexion);
    de
}

void impresion(){
  Serial.print(copy_SISMyC_NoConexion);
  Serial.print("\t");
}

void enviarDatosPrueba(){
  copy_SSF_NoConexion = Id; 
    Envio_Datos_Sensores();
    Limpieza_Datos(); 
}


int Envio_Datos_Sensores() {   
    delay(10);
    ModbusRTUServer.holdingRegisterWrite(SSF_NoConexion,copy_SSF_NoConexion); 
}


int Limpieza_Datos(){  
    delay(110);
    copy_SSF_NoConexion = ModbusRTUServer.holdingRegisterRead(SSF_NoConexion);
    delay(50)
}
