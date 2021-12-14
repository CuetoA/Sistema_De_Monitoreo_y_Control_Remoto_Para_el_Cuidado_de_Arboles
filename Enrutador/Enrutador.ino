String valor = " "; // for incoming serial data
String mensaje0 = "Arduino ha recibido el siguiente mensaje: ";
String numeroConexion = "13";
char numeroConexionFalso = "3";
int posicion1 = 0;
int posicion2 = 0;
int datosDisponibles = 1;


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
  String arreglo = subcadena;
  //separarPorComas(subcadena);
  Serial.println(arreglo[2]);

 // En esta parte deberemos cambiar 
  String caracter = String(arreglo[2]);
  int caracter2 = caracter.toInt();
  
  if ( caracter2 == 3 ){
    decidiendoAccion();
  }
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
  
}
