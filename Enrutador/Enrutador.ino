#include <Separador.h>
#include <TimeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
Separador s;

// cueto
String valor = " "; // for incoming serial data
String mensaje0 = "Arduino ha recibido el siguiente mensaje: ";
String numeroConexion = "13";
String arreglo = " ";
int posicion1 = 0;
int posicion2 = 0;
int datosDisponibles = 1;
int idArduino = 3;
int numeroRegistros = 23;
String arregloDatos[23]={};


// grecia
int Led_V = 11;
int Led_S = 12;
int AlimentacionS = 10;

long Med_D;
long Rie_D;
long Horas_Del_Dia = 24;
int hor_M;  //Cada cuantas horas se mide
int hor_R;  //Minutos que se le suman a las horas en que se mide
int min_M;  //Cada cuantas horas se riega
int min_R;  //Minutos que se le suman a las horas en que se riega
int i = 0;
int valorHumedad1;
int valorHumedad2;
int valorHumedad3;
int SensorT1;
int SensorT2;
int SensorT3;
long Prom_SensorT;
int SensorH1_Porc;
int SensorH2_Porc;
int SensorH3_Porc;
long Prom_SensorH;

//Sensores
int pinDatosT = 12;
OneWire oneWireObjeto(pinDatosT);
DallasTemperature sensorDS18B20(&oneWireObjeto);
time_t fecha,Fecha_C, Fecha_R, Fecha_M;

#define sensorH1 A1  // Pin analogico donde se conecta el sensor humedad 1 
#define sensorH2 A2  // Pin analogico donde se conecta el sensor humedad 2
#define sensorH3 A3  // Pin analogico donde se conecta el sensor humedad 3

//Control 
//Constantes-------------

int lectura_SF1 =2;     //Pin lectura de sensor de flujo 1 Agua INT0
int lectura_SF2 =3;     //Pin lectura de sensor de flujo 2 Fertilizante INT1

  //--AJUSTAR--  
int alim_SF1 =7;       //Pin alimentación de sensor de flujo 1
int alim_SF2 =8;       //Pin alimentación de sensor de flujo 2
  //Salida del control
int VS1 =6;       //Pin alimentación de válvula solenoide 1 PWM
int VS2 =5;        //Pin alimentación de válvula solnoide 2 PWM

//Variables--------------

float volumen_a=0, volumen_a_final=0, volumen_f=0, volumen_f_final=0;    
//Para el volumen de fertilizante (Por lo mientras recibida en serial)


volatile int NumPulsosA=0, NumPulsosF=0; //variable para la cantidad de pulsos recibidos

//Calibración<<<<<
float factor_conversion1 = 6.9, factor_conversion2 = 7.0;             //De acuerdo a calibración de sensor realizada previamente<<<<<<
long dt=0;                  //variación de tiempo por cada bucle
long t0=0;                  //tiempo en millis() del bucle anterior

char modo;
char prueba_val;

//Bandera de riego
int band_R = 0;


//---Funciones que se ejecuta en interrupción---------------
void ContarPulsosAgua()
{ 
  NumPulsosA++;  //incrementamos la variable de pulsos
}

 void ContarPulsosFert ()
{ 
  NumPulsosF++;  //incrementamos la variable de pulsos
} 
//---------------------------------------------------------


//--Función para la definición de intervalos de acciones--
void own_delay(int time_d){
  interrupts();
  delay(time_d);
  noInterrupts();
  }


void Intervalos(){
  /*
  Serial.print("Mediciones al día: ");
  while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
  if(Serial.available()){
    Med_D = Serial.parseInt();          
    Serial.println(Med_D);
     }
  Serial.print("Riego al día:");
  while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
   if(Serial.available()){
    Rie_D = Serial.parseFloat();
    Serial.println(Rie_D);          
     }
  */
  Med_D = arregloDatos[15].toInt();
  Rie_D = arregloDatos[13].toInt();
  //Intervalos de medicion
  hor_M = Horas_Del_Dia/Med_D;
  long Sob_hor_M = Horas_Del_Dia%Med_D;
  if( Sob_hor_M != 0){
    min_M = (Sob_hor_M * 60)/Med_D; 
  }
  else{
    min_M = 0;
  }

  //Intervalos de riego
  hor_R = Horas_Del_Dia/Rie_D;
  long Sob_hor_R = Horas_Del_Dia%Rie_D;
  if( Sob_hor_R != 0){
    min_R = (Sob_hor_R * 60)/Rie_D; 
  }
  else{
    min_R = 0;
  }
}
//--------------------------------------------------------------

//---Función para rutinas, conversión de horas, ajuste de días------------------
int Rutina(int Fecha_Accion, int hor_Accion, int min_Accion,time_t fecha_accion, time_t fecha){
int accion;
int Rut_Dia;
int Horas = hour(fecha_accion) + hor_Accion;
long Minutos =  minute(fecha_accion) + min_Accion;

  int Add_Horas = Minutos/60;
  long min_sob = Minutos%60;
  //Serial.println(min_sob);
  //int Rut_min = min_sob*60;
  int Rut_min = min_sob;
int Rut_horas = Horas + Add_Horas;

if(Rut_horas>24){
  int Rut_horas_24 = Rut_horas/24;
  long Rut_horas_sob = Rut_horas-24;
  Rut_Dia = day(fecha_accion) + Rut_horas_24;
  Rut_horas = Rut_horas_sob;
  
}
else{
  Rut_Dia = day(fecha_accion);
}

// If de Medicion
Serial.print("Fecha a llegar  \t");
Serial.println(Rut_Dia);
Serial.print("\t");
Serial.print(Rut_horas);
Serial.print(":");
Serial.println(Rut_min);

Serial.print("Fecha de ahora  \t");
Serial.print(day(fecha));
Serial.print("\t");
Serial.print(hour(fecha));
Serial.print(":");
Serial.println(minute(fecha));


if(Rut_Dia <= day(fecha) && Rut_horas <= hour(fecha) && Rut_min <= minute(fecha)){
  accion = 1;
  fecha_accion = now();}
else{
  accion = 0;
}
return accion;

}
//-------------------------------------------------------

//---Funciones para el control----------------------------


//----Modo de riego----
void ModoRiego()
{
  //¿Ciclo de riego o prueba?
//  Serial.println("S5 Control y distribución de suministros");
//  Serial.println("Seleccione el modo de riego:");
//  Serial.println("0- Prueba de riego \n1-Ciclo de riego");

}


//----- Función para prueba de apertura de válvulas-----
void Prueba()
{  
  //Mandar 1 a salidas PWM
              while(true){
            while(Serial.available()==0)
              {
                //No hagas nada si no recibes nada
              }
          prueba_val = Serial.read();
          //Serial.println(prueba_val);
            switch(prueba_val)
            {
              case '0': //Serial.println("Abriendo válvula 1"); 
                        digitalWrite(VS1,HIGH);
                        own_delay(1000);
                        digitalWrite(VS1,LOW);
                        //Abrir válvula y cerrar con un delay para prueba. LED para observar
                        break;
              case '1': //Serial.println("Abriendo válvula 2"); 
                        digitalWrite(VS2,HIGH);
                        own_delay(1000);
                        digitalWrite(VS2,LOW);
                        //Abrir válvula y cerrar con un delay para prueba. LED para observar
                        break;
              case 'm': ModoRiego();
                        return;
            }
            }
}


float VolumenCaptura_A(){
  Serial.print("Ingresa el volumen de agua requerido[L]:");
       while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
     if(Serial.available() > 0 ){
    float volumen_a_final= Serial.parseFloat();          
    
    return volumen_a_final;
     }
  }


  float VolumenCaptura_F(){
  Serial.print("Ingresa el volumen de fertilizante requerido[L]:");
       while(Serial.available()==0)
  {
    //No hagas nada si no recibes nada
  }
     if(Serial.available() > 0 ){
    float volumen_f_final= Serial.parseFloat();          
    
    return volumen_f_final;
     }
  }


//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuecia() 
{
  int frecuencia_a;
  NumPulsosA = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(500);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia_a= NumPulsosA; //Hz(pulsos por segundo)
//  Serial.print
//  Serial.println(frecuencia_a);
  return frecuencia_a;
}


int ObtenerFrecueciaF() 
{
  int frecuencia_f;
  NumPulsosF = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(500);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  //Hz(pulsos por segundo)
  frecuencia_f= NumPulsosF;
  return frecuencia_f;
}
//----------------------------------------------------



void CicloRiego()
{
  //Serial.println("Entrando a ciclo de riego");
  //Obtenemos datos de sensores
  //---Agua---
  float frecuencia_a=ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m_a=frecuencia_a/factor_conversion1; //calculamos el caudal en L/m
  float caudal_L_h_a=caudal_L_m_a*60; //calculamos el caudal en L/h
  //---Fertilizante---
  float frecuencia_f=ObtenerFrecueciaF();             //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m_f=frecuencia_f/factor_conversion2;  //calculamos el caudal en L/m
  float caudal_L_h_f=caudal_L_m_f*60;                 //calculamos el caudal en L/h

  dt=millis()-t0; //calculamos la variación de tiempo
  t0=millis();

  
  //Conteo de volumen
  volumen_a=volumen_a+(caudal_L_m_a/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)
  volumen_f=volumen_f+(caudal_L_m_f/60)*(dt/1000); 

  //Condicional para abrir/cerrar válvulas
  //---AGUA----
  if(volumen_a<volumen_a_final){

    //Serial.println("V_agua Abierta");
    digitalWrite(VS1,HIGH);
  }
  else if(volumen_a>=volumen_a_final){
      
      digitalWrite(VS1,LOW);
      
    //Serial.println("V_agua Cerrada");
      
      }
 
      
  //---FERTILIZANTE---
  if(volumen_f<volumen_f_final){
    digitalWrite(VS2,HIGH);
  
    //Serial.println("V_fer Abierta");
  } 
  else if(volumen_f>=volumen_f_final){
      
      digitalWrite(VS2,LOW);
      
    //Serial.println("V_fert Cerrada");
      }

  
  Serial.print    ("Numero de Pulsos Agua = "); 
  Serial.print    (NumPulsosA); 
  Serial.print    ("\t Numero de Pulsos Fertilizante = ");
  Serial.println  (NumPulsosF);
//  Serial.println  ("Caudal agua: \t\t\t Caudal fertilizante:"); 
//  Serial.print    (caudal_L_m_a,3);
//  Serial.print    ("L/min\t\t\t"); 
//  Serial.print    (caudal_L_m_f,3);
//  Serial.println  ("L/min");
//  Serial.print    (caudal_L_h_a,3); 
//  Serial.print    ("L/h\t\t\t");   
//  Serial.print    (caudal_L_h_f,3);
//  Serial.println  ("L/h");
  Serial.println  ("Volumen agua: \t\t\t Volumen fertilizante:");
  Serial.print    (volumen_a,3); 
  Serial.print    (" L\t\t\t\t");
  Serial.print    (volumen_f,3);
  Serial.println  ("L");
  Serial.print    (volumen_a_final);
  Serial.print    (" L\t\t\t\t");
  Serial.print    (volumen_f_final);
  Serial.println  (" L");
//  Serial.print("Volumen_Fi \t");
//  Serial.print(volumen_f);
//  Serial.print("Volumen_Ai \t");
//  Serial.println(volumen_a);
//  Serial.print("Volumen_Ff \t");
//  Serial.print(volumen_f_final);
//  Serial.print("Volumen_Af \t");
//  Serial.println(volumen_a_final);

  if(volumen_f>=volumen_f_final&&volumen_a>=volumen_a_final){
      band_R=0;
    }   

}



void medir(){
  digitalWrite(AlimentacionS,HIGH);
  valorHumedad1 = map(analogRead(sensorH1),165, 944, 100, 0);
  valorHumedad2 = map(analogRead(sensorH2),165, 944, 100, 0);
  valorHumedad3 = map(analogRead(sensorH3),165, 944, 100, 0);
  // Adquisicion de la lectura de los sensores de temperatura
  sensorDS18B20.requestTemperatures();
 
  // Leemos y mostramos los datos de los sensores DS18B20
  SensorT1 = sensorDS18B20.getTempCByIndex(0);
  SensorT2 = sensorDS18B20.getTempCByIndex(1);
  SensorT3 = sensorDS18B20.getTempCByIndex(2);
  Prom_SensorT = (SensorT1 + SensorT2 + SensorT3)/3;
  SensorH1_Porc = valorHumedad1;
  SensorH2_Porc = valorHumedad2;
  SensorH3_Porc = valorHumedad3;
  Prom_SensorH = (SensorH1_Porc + SensorH2_Porc + SensorH1_Porc)/3;
  
  Serial.print(sensorDS18B20.getTempCByIndex(0));
  Serial.print(";");
  Serial.print(sensorDS18B20.getTempCByIndex(1));
  Serial.print(";");
  Serial.print(sensorDS18B20.getTempCByIndex(2));
  Serial.print(";");
  Serial.print(valorHumedad1);
  Serial.print("; ");
  Serial.print(analogRead(sensorH1));
  Serial.print("; ");
  Serial.print(valorHumedad2);
  Serial.print(";");
  Serial.print(analogRead(sensorH2));
  Serial.print("; ");
  Serial.print(valorHumedad3);
  Serial.print(";");
  i=i+1;
  Serial.print(analogRead(sensorH3));
  Serial.print(";");
  Serial.println(i);
  Serial.println(" ");

  Serial.print("Promedio T, H: "); 
  Serial.print(Prom_SensorT); 
  Serial.print("\t");
  Serial.println(Prom_SensorH);
}

void regar(){
  //---Alimentación del sensor de flujo
  Serial.println("Entro");
  digitalWrite(alim_SF1,HIGH);
  digitalWrite(alim_SF2,HIGH);
  CicloRiego(); 

}




//--------------------------------------------------------






//////////////////



void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(Led_V, OUTPUT);
  pinMode(Led_S, OUTPUT);
  pinMode(AlimentacionS, OUTPUT);  
  pinMode(sensorH1, INPUT);  
  pinMode(sensorH2, INPUT);
  pinMode(sensorH3, INPUT);
  //---pinMode--- INPUT's
  pinMode(lectura_SF1, INPUT);
  pinMode(lectura_SF2, INPUT);
  //---pinMode---- OUTPUT's
  pinMode(alim_SF1, OUTPUT);  
  pinMode(alim_SF2, OUTPUT);
  pinMode(VS1, OUTPUT);
  pinMode(VS2, OUTPUT);

  //Iniciamos cerradas las válvulas
  digitalWrite(VS1,LOW);
  digitalWrite(VS2,LOW);
  
  // Iniciamos el bus 1-Wire
  sensorDS18B20.begin();


  //Definición de interrupciones
  attachInterrupt(0,ContarPulsosAgua,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  attachInterrupt(1,ContarPulsosFert,RISING);//(Interrupción 1(Pin3),función,Flanco de subida)
  t0=millis();
    
  //setTime(hora,minutos,segundos,dia,mes,anyo);
  setTime(1, 15 , 0, 14, 12, 2021);                 //---------------------------------------------MODIFICACIÓN DE FECHA-------------------------------------------------------
  fecha = now();
  Fecha_C = fecha;
  Fecha_R = fecha;
  Fecha_M = fecha;
  Serial.print(hour(fecha));
  Serial.print(":");
  Serial.println(minute(fecha));
  //Intervalos();

  /*
  volumen_a_final = VolumenCaptura_A();
  Serial.println(volumen_a_final);
  volumen_f_final = VolumenCaptura_F();  
  Serial.println(volumen_f_final); */
}


void loop() {
  // send data only when you receive data:
  //interrupts();
  //fecha = now();
  
  while(Serial.available()==0){//No hagas nada si no recibes nada
    }
  if (Serial.available() > 0) {
    valor = Serial.readString();  
    posicion1 = valor.indexOf("CMD0001 ");
    posicion2 = valor.indexOf("CMD0002 ");
  }
  decidiendoAccion();
}


void decidiendoAccion(){
  if (posicion1 == 0 and datosDisponibles == 1){
    envioDeDatosDeSensores();
  }else if (posicion2 == 0){
    Serial.println("Entrando a recibiendo datos");
    SepararPorComas(numeroRegistros);
    verificarNodo();
  }else{
    Serial.println("Se recibió un valor no reconocido");
  }
}

void envioDeDatosDeSensores(){
  Serial.println("enviando datos");
}

String SepararPorComas(int NumeroDeRegistros){
  String subcadena = valor.substring(8,-1);
  Serial.print("El valor de entrada es: ");
  Serial.println(subcadena);
  
  for(int i=0; i<NumeroDeRegistros; i++){
    arregloDatos[i] = s.separa(subcadena, ',',i);          
  } 
}

void verificarNodo(){
  Serial.println("Entrando a la verificación de Nodo");
  int idArribo = arregloDatos[2].toInt();
  if (idArribo == idArduino){
    verificarModos();
  }
}

void verificarModos(){
  Serial.println("verificando modos");
  int modo = arregloDatos[8].toInt();
  Serial.print("la posición 8 es: ");
  Serial.println(arregloDatos[8]);
  if(modo == 0){
    // Semiautomático
    Serial.println("Entrando al modo semiautomático");
  }else if(modo == 1){
    // Automático
    Intervalos();
    seteoDeVariables();
    automatico();
  } 
}

void seteoDeVariables(){
  volumen_a_final = arregloDatos[17].toInt();
  volumen_a_final = volumen_a_final / 1000;
  //Serial.println(volumen_a_final);
  volumen_f_final = arregloDatos[18].toInt();
  volumen_f_final = volumen_f_final / 1000;
  //Serial.println(volumen_f_final);
//  Serial.print("Med_D: \t");
//  Serial.println(Med_D);
//  Serial.print("Rie_D: \t");
//  Serial.println(Rie_D);
}



void automatico(){
  Serial.println("Automático");
  //Intervalos();
  int Bandera_Automatico = 1 ;
  do{
    fecha = now();  
  Serial.print("Fecha Automatico ");
  Serial.print(hour(fecha));
  Serial.print(":");
  Serial.println(minute(fecha));
  int accion_M = Rutina(day(Fecha_M),hor_M,min_M,Fecha_M,fecha);
  int accion_R = Rutina(day(Fecha_R),hor_R,min_R,Fecha_R,fecha);
  
  
  if (accion_M == 1){
    String Mensaje_Mediciones;
    medir();
    digitalWrite(Led_S,HIGH);
    interrupts();
    delay(500);
    int Prom_SensorT2 = Prom_SensorT * 100;
    int Prom_SensorH2 = Prom_SensorH * 100;
    Mensaje_Mediciones = "RSP00011," +  (String)idArduino + ",0,1,1,0," + (String)day(fecha) + "," + (String)month(fecha) + "," + (String)year(fecha) + "," + (String)hour(fecha) + "," + (String)minute(fecha) + "," + (String)Prom_SensorT2 + "," + (String)Prom_SensorH2 + ",0,0,0,0";
    Serial.println(Mensaje_Mediciones);
    Fecha_M = now();}
    else{
      digitalWrite(AlimentacionS,LOW);
      digitalWrite(Led_S,LOW);
      
    }
   if (accion_R == 1){
    medir();
    digitalWrite(AlimentacionS,LOW);
    band_R=1;
    Serial.println(Prom_SensorT);
    Serial.println(Prom_SensorH);
    if(Prom_SensorT > arregloDatos[19].toInt() && Prom_SensorT< arregloDatos[20].toInt() && Prom_SensorH < arregloDatos[22].toInt()){
      do{ 
      regar();

    }while(band_R==1);
    volumen_f = 0;
    volumen_a = 0;
    interrupts();
    delay(500);
    Fecha_R = now();}
    
    }

    Bandera_Automatico = 1;
  }while(Bandera_Automatico == 1);
}
