#include <Separador.h>
Separador s;

String valor = " "; // for incoming serial data
String mensaje0 = "Arduino ha recibido el siguiente mensaje: ";
String numeroConexion = "13";
String arreglo = " ";
int posicion1 = 0;
int posicion2 = 0;
int datosDisponibles = 1;
int idArduino = 3;
int numeroRegistros = 9;
String arregloDatos[9]={};


void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  //Serial.println('Arduino saludando al servidor, cambio');
}


void loop() {
  // send data only when you receive data:
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
  //Serial.println("entrando a decidiendo accion");
  if (posicion1 == 0 and datosDisponibles == 1){
    envioDeDatosDeSensores();
  }else if (posicion2 == 0){
    Serial.println("Entrando a posicion2");
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
  //String Med_D = Serial.readString();
  //String arreglo[NumeroDeRegistros] = {};
  String subcadena = valor.substring(8,-1);
  Serial.print("El valor de entrada es: ");
  Serial.println(subcadena);
  
  for(int i=0; i<NumeroDeRegistros; i++){
    arregloDatos[i] = s.separa(subcadena, ',',i);          
  } 

  Serial.println(arregloDatos[6]);
  Serial.println(arregloDatos[7]);
  Serial.println(arregloDatos[8]);
}

void verificarNodo(){
  int idArribo = arregloDatos[2].toInt();
  if (idArribo == idArduino){
    verificarModos();
    Serial.println("Entrando a la verificación de Nodo");
  }
}

void verificarModos(){
  Serial.println("verificando modos");
  int modo = arregloDatos[8].toInt();
  Serial.println(arregloDatos[8]);
  if(modo == 0){
    // Semiautomático
    Serial.println("semiautomático");
  }else if(modo == 1){
    // Automático
    Serial.println("Automático");
  } 
}
