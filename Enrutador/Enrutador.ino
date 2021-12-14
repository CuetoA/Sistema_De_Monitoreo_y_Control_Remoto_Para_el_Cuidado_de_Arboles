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
String arregloSsfASismyc[3]={};
String arregloSismycASsf[3]={};


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
  if(posicion1 == 0 or posicion2 == 0){
    procesamientoDeDatos();
  }
}


void procesamientoDeDatos(){
  String subcadena = valor.substring(8,-1);
  arreglo = subcadena;
  String* arreglo = SepararPorComas(3);
  String arreglo2 = 
  Serial.print("El arreglo de arribo es: ");
  Serial.println(arreglo);
  String idMensaje = String(arreglo[2]);
  int idMensajeStr = idMensaje.toInt();
  Serial.print("El id de arribo es: ");
  Serial.println(idMensajeStr);

 // En esta parte deberemos cambiar 
  //String caracter = String(arreglo[2]);
  //int caracter2 = caracter.toInt();
  // Después deberemos cambiar el caracter2 a un elemento del arreglo
  if ( idMensaje == idArduino ){
    decidiendoAccion();
  }
}


String* SepararPorComas(int NumeroDeRegistros){
  String Med_D = Serial.readString();
  String arreglo[NumeroDeRegistros] = {};
  for(int i=0; i<NumeroDeRegistros; i++){
    arreglo[i] = s.separa(Med_D, ',',i);          
  } 
  return arreglo;  
}



void decidiendoAccion(){
  if (posicion1 == 0 and datosDisponibles == 1){
    envioDeDatosDeSensores();
  }else if (posicion2 == 0){
    verificarModos();
  }else{
    Serial.println("Se recibió un valor no reconocido");
  }
}


void envioDeDatosDeSensores(){
  Serial.println("enviando datos");
}


void verificarModos(){
  Serial.println("verificando modos");
  if(arreglo[8] == 0){
    // Semiautomático
  }else if(arreglo[8] == 1){
    // Automático
  }
  
}
