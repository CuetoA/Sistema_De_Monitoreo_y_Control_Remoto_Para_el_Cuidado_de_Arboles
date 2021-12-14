#include <Separador.h>

Separador s;
int NumeroDeRegistrosDeSSF = 3;
String el1[3]={};
//String el1[3] = {} ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Ingresa cadena: ");
  
}

void loop() {
  while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
  if(Serial.available()){
    SepararPorComas(NumeroDeRegistrosDeSSF);
  }
  for(int i=0;i<NumeroDeRegistrosDeSSF;i++){
   Serial.println(el1[i]); 
  }
}

String SepararPorComas(int NumeroDeRegistros){
  String Med_D = Serial.readString();
  //String arreglo[NumeroDeRegistros] = {};
  for(int i=0; i<NumeroDeRegistros; i++){
    el1[i] = s.separa(Med_D, ',',i);          
  } 
  //return arreglo;  
}
