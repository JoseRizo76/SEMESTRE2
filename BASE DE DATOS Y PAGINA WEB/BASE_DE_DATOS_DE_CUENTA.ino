// DECLARACION DE LIBRERIA
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;
WebServer server(80);

// INSTANCIA PARA GUARDAR LOS DATOS EN LA NVS DE LA ESP32
Preferences preferences;

// CONFIGURACION DE EL RFID
#define RST_PIN 5
#define SS_PIN 16
MFRC522 mfrc522(SS_PIN, RST_PIN);

// CONFIGURACION DE LA PANTALLA LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// CONFIGURACION DE EL TECLADO MATRICIAL
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {12, 26, 14, 17};
byte colPins[COLS] = {13, 2, 4};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// DECLARACION DE VARIABLES DE MANERA GLOBAL
//VARIABLES DE LA MAQUINA EXPENDEDORA

const byte P1 = 15;
const byte P2 = 25;
const byte P3 = 33;
const byte P4 = 27;
const byte P5 = 32;

//ID DE LA TARJETA DE LA CLAVE MAESTRA
char MASTERCARD [15] = "F150431B";

//VARIABLES DE LOS DIFERENTES TIPOS DE PRODUCTOS
int Producto1 = 2;
int Producto2 = 2;
int Producto3 = 2;
int Producto4 = 2;
byte SelecProducto = 0;
byte precio = 0; 
int montoFinal = 0;
int cantidadProducto = 0;
char cantchar [4];

// VARIABLES DE CLIENTE
unsigned int contador;
byte estado = 0;
boolean fin = false;
int user = 0;
byte i = 0;
byte numCliente = 0;
char VT [30] = "";
boolean VDinero = false;

// VARIABLES DE ALMACENAMIENTO DE TECLA PRESIONADA
byte indice = 0;
char dato[10];
byte indiceD = 0;
char dineroSave[5];
int dineroint = 0;
char idVF[5];
char passVF [5];

//VARIABLES PAGO
boolean cuentaExistente = false;

//VARIABLES DE RETIRO DE PRODUCTO Y VERIFICACION DE LA CUENTA
boolean retiroProducto = false;
byte CTP = 0;

// VARIABLES DE OPCIONES DE MENU
byte opc = 0; // MENU PRINCIPAL
byte subopc = 0;
boolean estado1 = false;
boolean estadoVC = false;
boolean estadowifi = false;
boolean estadotiempo = true;
 

// VARIABLES DE TIEMPO
unsigned long tiempo = 0;
int timea = 0;
int timeb = 0 ;
int segundos = 0;
int tiempo_actual = 0;
int tiempo_anterior = 0;

/// VARIABLE QUE PERMITE GUARDAR DATOS EN LA NVS DE LA ESP32
boolean guardarEnPreferences = false;

// VARIABLES DE TIPO BANDERA
char band = 'f';
char band2 = 'v';
char band3 = 'f';
char band4 = 'f';
char band5 = 'f';
char band6 = 'f';
char band7 = 'f';
char band8 = 'f';
char bandVC = 'g';
char band9 = 'g';
char bandx = 'g';
//verificacion NFC
String idTarjeta1 = "";
String idTarjeta2 = "";

// VARIABLES PARA LA VERIFICACION DEL DUEÑO
char clave[7] = "000000";
boolean Cverificacion = false;
char clavei[7];
char clavePrint[7] = "111111";
boolean CverificacionPrint = false;

// ESTRUCTURA QUE ALMACENA LOS DATOS DEL CLIENTE
struct cliente
{
  int id = 0;
  char password[5] = "";
  int dinero = 100;
  char Tarjeta[30] = "";
  char NFC [30] = "";
} cliente[10]; // MAXIMO DE 10 CLIENTES POR EL MOMENTO

/*
void handleRoot() {
String html = "<!DOCTYPE html><html lang='es'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <script src='https://kit.fontawesome.com/892f23a568.js' crossorigin='anonymous'></script> <title>UNAN MANAGUA</title> <style>footer{position: absolute; bottom: 0; width: 100%; text-align: left;}p{color: white; font-size: 15px;}a[href='https://www.unan.edu.ni']{color: green; text-decoration: none; font-weight: bold;}body{margin: 0; padding: 0; font-family: Arial, sans-serif; background: url('https://drive.google.com/uc?export=download&id=1-OnPX-RrzTE9FEF-PIWnAeAr-MvfKGVk') no-repeat center center fixed; background-size: cover; height: 100vh; display: flex; align-items: center; justify-content: center;}.logo-unan{width: 220px; margin: 5px; border-radius: 10px 0px 0px 0px;}.titulo-container{align-items: center;}.titulo{color: red; letter-spacing: 2px; font-size: 20px; text-align: center; text-transform: uppercase; position: flex; margin-top: 1px; font-weight: bold;}.login-container{background-color: #222222; padding: 10px; border-radius: 20px; box-shadow: 5px 5px 10px 10px rgba(255, 1, 1, 0.285); width: 350px; height: 400px; text-align: center; border-width: 1px; border-color: rgba(255, 1, 1, 0.99); border-style: solid; animation: borderAnimation 0.5s infinite alternate;}@keyframes borderAnimation{0%{border-width: 1px;}100%{border-width: 4px;}}.subtitulo{margin-bottom: 20px; text-transform: uppercase; letter-spacing: 3px; color: #007bff; font-size: 17px;}::placeholder{text-align: center;}.texto-ingreso{width: 65%; padding: 6px; margin-bottom: 10px; margin-left: 3px; border: 1px solid red; border-radius: 10px; box-shadow: 6px 3px 6px 3px rgba(255, 4, 4, 0.7); background-color: #222222; color: white; text-align: center;}.texto-ingreso:focus{box-shadow: 5px 2px 5px 5px #044efa;}.submit{background-color: #171718; color: #fff; width: 65%; padding: 1px; border: 1px solid rgb(255, 0, 13); border-radius: 10px; cursor: pointer; height: 30px; margin-bottom: 10px; margin-left: 3px; text-align: center;}.submit:hover{background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); border-color: white; box-shadow: 5px 2px 5px 5px rgba(255, 4, 4, 0.7);}p{position: absolute; align-items: flex-start;}.fa-cart-shopping{font-size: 25px; color: red; margin-top: 10px;}@media (max-width: 480px){.login-container{width: 80%;}}.fa-user{font-size: 20px; color: rgba(254, 6, 6, 0.751); justify-content: center; margin-right: 4px;}.fa-magnifying-glass-dollar{color: rgba(255, 2, 2, 0.794); font-size: 20px; margin-right: 4px; margin-top: 5px; justify-content: center;}.fa-question{position: absolute; bottom: 0; right: 0; margin: 0; border-radius: 30px; text-align: center; color: #ff0000; font-size: 60px;}.modal{display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background-color: rgba(26, 25, 25, 0.7); z-index: 999;}.modal-contenido{position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); width: 350px; height: 400px; background-color: #020202; padding: 30px; border: 3px solid rgba(8, 206, 255, 0.927); border-radius: 10px; box-shadow: 0 0 10px rgba(8, 206, 255, 0.927);}.modal-contenido h6{color: rgb(0, 255, 217); text-align: center; font-size: 20px; text-indent: 1px;}.cerrar-modal{position: absolute; top: 10px; right: 10px; font-size: 30px; cursor: pointer; color: red;}.error-message:hover{background-color: rgb(4, 4, 4); color:rgb(0, 255, 242); border-color: white; font-size: 10px; box-shadow: 3px 2px 10px 2px #02ffee;}.error-message{background-color: #000000; border: 1px solid #02ffee; color: rgb(0, 255, 242); padding:8px; border-radius: 15px; margin-top: 4px; margin-bottom : 20px; font-size: 9px; margin-left: 10px; margin-right: 10px; letter-spacing: 1px;}@keyframes shake{0%{transform: translateX(0);}25%{transform: translateX(-5px);}75%{transform: translateX(5px);}100%{transform: translateX(0);}}.vibrar{animation: shake 0.3s;}</style> </style></head><body> <div class='login-container'> <div class='titulo-container'> <img class='logo-unan' src='https://drive.google.com/uc?export=download&id=1-o8Q9Ol7JsAEkVuM772RNePLjGVqEpxl' alt='UNAN MANAGUA'> <h1 class='titulo'>SNACK VENDING MACHINE</h1> </div><h2 class='subtitulo'>CONSULTE SU SALDO</h2> <form action='/search' method='get' onsubmit='return validarInput()'> <i class='fa-solid fa-user'></i> <input type='number' id='numero' name='accountId' placeholder='DIGITE ID DE USUARIO' required class='texto-ingreso'> <br><i class='fa-solid fa-magnifying-glass-dollar'></i> <input type='submit' value='Consultar' class='submit'> </form> <i class='fa-solid fa-cart-shopping'></i> <br><i class='fa-solid fa-question' id='icono-ayuda'></i> </div><div id='modal-ayuda' class='modal'> <div class='modal-contenido'> <span class='cerrar-modal' id='cerrar-modal'>&times;</span> <h6>ESTE ES UN MENSAJE DE AYUDA<br><br>GRUPO 4 ING ELECTONICA</h6> </div></div><script>var iconoAyuda=document.getElementById('icono-ayuda'); var modalAyuda=document.getElementById('modal-ayuda'); var cerrarModal=document.getElementById('cerrar-modal'); iconoAyuda.addEventListener('click', function(){modalAyuda.style.display='block';}); cerrarModal.addEventListener('click', function(){modalAyuda.style.display='none';}); function validarInput(){let input=document.getElementById('numero').value; const form=document.querySelector('.login-container form'); let errorElement=document.querySelector('.error-message'); const digitosRegex=/^\\d{4}$/; const numeroInput=document.getElementById('numero'); if (!digitosRegex.test(input)){if (!errorElement){const newErrorElement=document.createElement('div'); newErrorElement.className='error-message'; newErrorElement.innerHTML='ERROR! <br>INGRESE 4 DIGITOS SIN CARACTERES ESPECIALES'; form.insertBefore(newErrorElement, form.firstChild); newErrorElement.classList.add('vibrar');}else{errorElement.classList.remove('vibrar'); void errorElement.offsetWidth; errorElement.classList.add('vibrar');}numeroInput.classList.remove('vibrar'); void numeroInput.offsetWidth; numeroInput.classList.add('vibrar'); return false;}else{if (errorElement){errorElement.parentNode.removeChild(errorElement);}numeroInput.classList.remove('vibrar'); return true;}}</script> <footer> <p>INGENIERA ELECTRONICA <a href='https://www.unan.edu.ni/' target='_blank' rel='noopener noreferrer'>@UNAN MANAGUA</a></p></footer></body></html>";


  server.send(200, "text/html", html);
}*/

/*
void handleSearch() {
  String accountIdStr = server.arg("accountId");
  int accountId = accountIdStr.toInt();

  String response;
  for (int i = 0; i < contador; i++) {
    if (accountId == cliente[i].id) {
      response = "<!DOCTYPE html><html lang='es'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <script src='https://kit.fontawesome.com/892f23a568.js' crossorigin='anonymous'></script> <title>UNAN MANAGUA</title> <style>p{color: white; font-size: 15px;}a[href='https://www.unan.edu.ni']{color: green; text-decoration: none; font-weight: bold;}body{margin: 0; padding: 0; font-family: Arial, sans-serif; background: url('https://drive.google.com/uc?export=download&id=1-OnPX-RrzTE9FEF-PIWnAeAr-MvfKGVk') no-repeat center center fixed; background-size: cover; height: 100vh; display: flex; align-items: center; justify-content: center;}.logo-unan{width: 220px; margin: 5px 5px 5px 5px; border-radius: 10px 0px 0px 0px;}.titulo{color: red; letter-spacing: 2px; font-size: 20px; text-align: center; text-transform: uppercase; position: flex; margin-top: 1px; font-weight: bold;}.login-container{background-color: #222222; padding: 10px; border-radius: 20px; box-shadow: 5px 5px 10px 10px rgba(255, 1, 1, 0.285); width: 350px; height: 400px; text-align: center; border-width: 1px; border-color: rgba(255, 1, 1, 0.99); border-style: solid; animation: borderAnimation 0.5s infinite alternate;}@keyframes borderAnimation{0%{border-width: 1px;}100%{border-width: 4px;}}.subtitulo{margin-bottom: 20px; text-transform: uppercase; letter-spacing: 3px; color: #3fcc26; font-size: 17px;}::placeholder{text-align: center;}.texto-ingreso{background-color: #222222; color: rgb(255, 255, 255);}.dinero{background-color: transparent; color: rgb(255, 255, 255);}.submit:hover{background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); border-color: white; box-shadow: 5px 2px 5px 5px rgba(255, 4, 4, 0.7);}.fa-house{font-size: 45px; color: #000000; padding: 10px; animation: bordertext 0.8s infinite alternate-reverse; margin-bottom: 15px;}@media (max-width: 480px){.login-container{width: 80%;}}@keyframes bordertext{0%{text-shadow: 1px 1px 1px rgb(4, 255, 192);}100%{text-shadow: 3px 1px 3px rgb(6, 255, 56)}}.fa-user{font-size: 20px; color: rgba(254, 6, 6, 0.751); justify-content: center; margin-right: 4px;}.fa-dollar-sign{color: #0eca34f8; font-size: 18px; margin-right: 4px; margin-top: 5px; justify-content: center;}.usuariotxt{color: red; font-size: 18px; font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;}.dinerotxt{color: #0eca34f8; font-size: 18px; font-family: Verdana, Geneva, Tahoma, sans-serif; letter-spacing: 2px;}</style></head><body> <div class='login-container'> <div class='titulo-container'> <img class='logo-unan' src='https://drive.google.com/uc?export=download&id=1-o8Q9Ol7JsAEkVuM772RNePLjGVqEpxl' alt='UNAN MANAGUA'> <h1 class='titulo'>SNACK VENDING MACHINE</h1> </div><h2 class='subtitulo'>CONSULTA DE SALDO</h2> <p class='usuariotxt'><i class='fa-solid fa-user'></i> USUARIO: " + String(cliente[i].id) + "</p><p class='dinerotxt'> SALDO C<i class='fa-solid fa-dollar-sign'></i>" + String(cliente[i].dinero) + "</p><br><a href='/' target='_self' rel='noopener noreferrer'><i class='fa-solid fa-house'></i></a> <br></div></body></html>";
      break;
    } else {
      response = "<!DOCTYPE html><html lang='es'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <script src='https://kit.fontawesome.com/892f23a568.js' crossorigin='anonymous'></script> <title>UNAN MANAGUA</title> <style>body{margin: 0; padding: 0; font-family: Arial, sans-serif; background: url('https://drive.google.com/uc?export=download&id=1-OnPX-RrzTE9FEF-PIWnAeAr-MvfKGVk') no-repeat center center fixed; background-size: cover; height: 100vh; display: flex; align-items: center; justify-content: center;}.logo-unan{width: 220px; margin: 5px 5px 5px 5px; border-radius: 10px 0px 0px 0px;}.titulo{color: rgb(255, 0, 0); letter-spacing: 2px; font-size: 20px; text-align: center; text-transform: uppercase; position: flex; margin-top: 1px; font-weight: bold;}.login-container{background-color: #222222; padding: 10px; border-radius: 20px; box-shadow: 5px 5px 10px 10px rgba(255, 1, 1, 0.285); width: 350px; height: 400px; text-align: center; border-width: 1px; border-color: rgba(255, 1, 1, 0.99); border-style: solid; animation: borderAnimation 0.1s infinite alternate-reverse;}@keyframes borderAnimation{0%{border-width: 1px;}100%{border-width: 6px;}}@keyframes bordertext{0%{text-shadow: 1px 1px 1px red;}100%{text-shadow: 3px 1px 3px red;}}.fa-house{font-size: 45px; color: #000000; margin-top: 20px; padding: 10px; text-shadow: 3px 1px 3px red; animation: bordertext 0.8s infinite alternate-reverse;}@media (max-width: 480px){.login-container{width: 80%;}}.error{color: rgb(255, 0, 0); font-size: 25px; margin: 0px 0px 15px 0px; letter-spacing: 2px; text-shadow: 1px 1px 1px rgb(72, 4, 4);}.fa-circle-exclamation{color: rgb(255, 0,0); background-color: transparent; font-size: 25px; text-shadow: 2px 2px 2px rgba(255, 255, 255, 0.081);}.mensaje{color: rgb(255, 251, 251); font-size: large; font-size: 18px; font-weight: bolder; text-shadow: 1px 1px 1px rgb(40, 7, 7);}.favor{color: rgb(246, 255, 0); font-size: small;}#segundos{color: rgb(255, 255, 0); font-size: 18px;}</style></head><body> <div class='login-container'> <div class='titulo-container'> <img class='logo-unan' src='https://drive.google.com/uc?export=download&id=1-o8Q9Ol7JsAEkVuM772RNePLjGVqEpxl' alt='UNAN MANAGUA'> <h1 class='titulo'>SNACK VENDING MACHINE</h1> <h5 class='error'> ERROR <i class='fa-solid fa-circle-exclamation'></i> </h5> <p class='mensaje'>ESTA CUENTA NO ESTA REGISTRADA</p><p class='favor'>SERA REDIRECCIONADO AL MENU PRINCIPAL EN <br></p><div id='contador'> <span id='segundos'>5</span> </div></div><a href='/' target='_self' rel='noopener noreferrer'><i class='fa-solid fa-house'></i></a> <br></div><script>function iniciarContador(){var segundos=5; var contador=document.getElementById('segundos'); var intervalo=setInterval(function(){segundos--; contador.textContent=segundos; if (segundos <=0){clearInterval(intervalo); window.location.href='/';}}, 1000);}window.addEventListener('load', iniciarContador); </script></body></html>" ;
    }
  }

  server.send(200, "text/html", response);
}*/


void setup()
{
  // INICIALIZA EL COMUNICADOR SERIAL A 115200 BAUDIOS
  Serial.begin(115200);


  // CARGA LOS DATOS ALMACENADOS DESDE NVS DEL ESP32
  preferences.begin("my-app", false);
  contador = preferences.getUInt("contador", 0);
  Producto1 = preferences.getUInt("Producto1", 2);
  Producto2 = preferences.getUInt("Producto2", 2);
  Producto3 = preferences.getUInt("Producto3", 2);
  Producto4 = preferences.getUInt("Producto4", 2);

  for (byte i = 0; i < contador; i++)
  {
    String key = "cliente" + String(i);
    preferences.getBytes(key.c_str(), &cliente[i], sizeof(cliente[i]));
  }
  preferences.end();

  // VOIDS
  RFID();
  lcdInit();
  wifiMulti.addAP("POCO-F3", "Jose Rizo");
  wifiMulti.addAP("Casa Dalmata", "Primos2021");
  wifiMulti.addAP("WiFi-Estudiante", "UNANManagua*");
  lcd.clear();
  cr();
  lcd.print("CONECTANDO WIFI");
  cr2();
  lcd.print("ESPERE");
  cargando();

  Serial.println("CONECTANDO A WIFI...");
  if(wifiMulti.run() == WL_CONNECTED) {
      lcd.print(".");
      delay(500);
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("Conectado a la red: ");
      Serial.println(WiFi.SSID());
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("Conexión WiFi exitosa");
      lcd.clear();
      cr();
      lcd.print("CONECTADO A WIFI");
      cr2();
      lcd.print(WiFi.SSID());
      delay(1000);
      lcd.clear();
  }
  else{
    lcd.clear();
    lcd.print("ERROR DE CONEXION");
    delay(500);
  }
  lcd.clear();

  // DECLARACION DE PINES DE SALIDA
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(P3, OUTPUT);
  pinMode(P4, OUTPUT);
  pinMode(P5, OUTPUT);

  

  // DECLARACION DE PINES DE ENTRADA

  // MENSAJE SI NO HAY NINGUNA CUENTA EN LA MEMORIA
  if (contador == 0)
  {
    cr();
    lcd.print("NESECITA CREAR");
    cr2();
    lcd.print("CUENTA");
    delay(1000);
  }

  //server.on("/", HTTP_GET, handleRoot);
  //server.on("/search", HTTP_GET, handleSearch);

  //server.begin();
  Serial.println("Servidor iniciado");
}

void loop()
{
  digitalWrite(P1, HIGH);
  digitalWrite(P2, HIGH);
  digitalWrite(P3, HIGH);
  digitalWrite(P4, HIGH);
  digitalWrite(P5, HIGH);
  //server.handleClient();

  // VARIABLES QUE ALMACENAN EL TIEMPO DESDE QUE SE ENCENDIO EL ESP32
  tiempo = millis();
  segundos = (tiempo / 1000);

  //VERIFICACION SI EL NFC ES COMPATIBLE CON EL PROGRAMA


  // SI DETECTA QUE NO HAY NINGUNA CUANTA EN LA ESTRUCUTA CLIENTE PEDIRA QUE SE INGRESE UNA CUENTA OBLIGATORIAMENTE
  if (contador == 0 && opc == 0 && band3 == 'f' || Cverificacion == false && opc == 2 && band3 == 'f')
  {
    mensaje_verificacion();
  }

  // VOID DE MENU DE OPCIONES QUE EL CLIENTE PUEDE REALIZAR
  opcion_menu_principal();

  // VERIFICA SI EL INGRESO ES DEL DUEÑO PARA HACER MODIFICACIONES EN ELLA
  verificacion_clave_maestra();

  // DESPUES DE VERIFICAR LA CONTRASEÑA PARA AGREGAR UNA CUENTA PASA A LA CREACION DE LA CUENTA
  if (opc == 2 && contador <= 9 && Cverificacion == true && subopc == 1 || contador == 0 && Cverificacion == true)
  {
    nuevaCuenta();
  }

  if (opc == 5)
  { // SI HAY MAS DE 10 USUARUIOS NO PERMITIRA EL INGRESO DE MAS USUARIOS
    lcd.clear();
    cr();
    Serial.print("USERS COMPLETED");
    lcd.print("USERS COMPLETED");
    estado = 1;
    delay(1000);
    lcd.clear();
    opc = 0;
    band3 ='v';
  }

  // SI LA CONTRASEÑA CORRECTA IMPRIME EN EL MONITOR SERIAL TODAS LAS CUENTAS ALMACENADAS
  if (CverificacionPrint == true){
    Serial.println("");
    Serial.println("<< CANTIDAD DE PRODUCTOS EN LA MAQUINA >>");
    Serial.print("PRODUCTO 1 -> ");
    Serial.println(Producto1);
    Serial.print("PRODUCTO 2 -> ");
    Serial.println(Producto2);
    Serial.print("PRODUCTO 3 -> ");
    Serial.println(Producto3);
    Serial.print("PRODUCTO 4 -> ");
    Serial.println(Producto4);
    Serial.println("");
    Serial.println("REGISTRO DE CUENTAS");
    Serial.println("");
    for (byte i = 0; i < contador; i++){
      Serial.print("Ciente ");
      Serial.println(i + 1);
      Serial.print("USUARIO : ");
      Serial.println(cliente[i].id);
      Serial.print("CONTRASEÑA : ");
      Serial.println(cliente[i].password);
      Serial.print("DINERO : ");
      Serial.println(cliente[i].dinero);
      Serial.print("TARJETA : ");
      Serial.println(cliente[i].Tarjeta);
      Serial.print("NFC : ");
      Serial.println(cliente[i].NFC);
      Serial.println("");
      Serial.println("");
    }
    CverificacionPrint = false;
  }

  // CUANDO SE HAYA MODIFICADO LA ESTRUCUTA SE ALMACENARA LOS CAMBIOS EN LA MEMORIA NVS DE LA ESP 32 REMPLZANDO ASI LOS CAMBIOS
  if (guardarEnPreferences){
    preferences.begin("my-app", false);
    preferences.putUInt("contador", contador);

    //GUARDA LA CANTIDAD DE PRODUCTO QUE HAY EN LA MAQUINA
    preferences.putUInt("Producto1", Producto1);
    preferences.putUInt("Producto2", Producto2);
    preferences.putUInt("Producto3", Producto3);
    preferences.putUInt("Producto4", Producto4);


    //BUCLE PARA ALMACENAR TODOS LOS DATOS DE LA ESTRUCTURA DENTRO DE LA NVS 
    for (byte i = 0; i < contador; i++){
      String key = "cliente" + String(i);
      preferences.putBytes(key.c_str(), &cliente[i], sizeof(cliente[i]));
    }
    preferences.end();
    guardarEnPreferences = false;
  }

 //MENU QUE PARA RECARGAR O AGREGAR CUENTA 
  if (opc == 2 && contador > 0 && Cverificacion == true && subopc == 0){
    char kye2 = keypad.getKey();
    //IMPRIME UNA SOLA  VEZ EN LA LCD 
    if (band7 == 'f'){
      lcd.clear();
      cr();
      lcd.print("RECARGAR -> 1");
      cr2(),
      lcd.print("NEW ACOUNT -> 2");
      cl(),
      lcd.print("IMPRIMIR CUENTAS-> 3");
      cl2();
      lcd.print("RELLENAR -> 4");
      band7 = 'v';
    }

    if (kye2){
      switch (kye2){
      //SI PRESIONA EL 1 ENTRARA AL MENU PARA RECARGAR UNA CUENTA
      case '1':
        subopc = 2; lcd.clear(); band7 = 'f'; break;
      //SI PRESIONA EL 2 ENTRAR AL MENU PARA CREAR NUEVA CUENTA
      case '2':
        if (contador<=9){subopc = 1; lcd.clear(); band7 = 'f';}
        else{opc = 5; Cverificacion = false; indice = 0; band2 = 'v'; band3 = 'f'; band6 = 'f';band7 ='f';} 
        break;
      //SI PRESIONA EL 3 BORRARA TODAS LAS CUENTAS QUE TENGA LA MAQUINA
      case '3': 
        lcd.clear(); CverificacionPrint = true; cr(); lcd.print("CUENTAS PRINT");    band_default(); lcd.clear(); ESP.restart(); ;break;
      //SI PRESIONA EL * VOLVERA AL MENU PRINCIPAL
      case '*': 
        band_default(); break;
      //SI PRESIONA EL 4 EL PROPIETARIO DE LA MAQUINA REABASTECERA LA CANTIDAD DE PRODUCTO QUE HAY EN LA MAQUINA
      case '4': 
        estadotiempo = false; Producto1 = 2; Producto2 = 2; Producto3 = 2; Producto4 = 2; lcd.clear(); cr(); lcd.print("PRODUCTOS LLENO"); delay(500); lcd.clear(); band_default(); break;
      //SI PRESIONA EL 0 BORRARA LAS CEUNTAS QUE TIENE LA MAQUINA EN SU BASE DE DATOS 
      case '0': 
        estadotiempo = false; lcd.clear(); cr(); lcd.print  ("CLEAR DATOS"); delay(500); lcd.clear(); preferences.begin("my-app", false); preferences.clear(); preferences.end(); ESP.restart(); break;
      //SI LA TECLA NO ES ENCONTRADA EN EL MENU DIRA QUE LA TECLA NO SE ENCONTRO
      default: 
        lcd.clear(); lcd.print("ERROR DE OPCION"); delay(500); lcd.clear(); band7 = 'f';break;
      }
    }
  }

  //PEDIRA EL ID DEL CLIENTE Y LO ALMACENARA DENTRO DE UNA VARIABLE PARA DESPUES COMPARARLA CON LOS DATOS DE LA ESTRUCTURA
  if (opc == 2 && subopc == 2){
    pedirID();
  }

  //SI SE INGRESA EL ID SE BUSCARA DENTRO DE TODA LA ESTRUCTURA
  if (user>=1000 && user < 9999){
    if (band7 == 'f'){
      lcd.clear();
      cr();
      lcd.print("ESPERE");
      delay(500);
      band7 = 'v';
    }
    //BUCLE PARA COMPARAR CON TODOS LOS ID DE LOS CLIENTES 
    for(int i=0; i<contador; i++){
      if (user == cliente[i].id){
        numCliente = i;
        bandVC = 'v';
        if (opc == 1){
          estadoVC = true;
        }
        else if (opc == 2 ){
          estado1 = true;
        }
        break;
      }
      else{
        bandVC = 'f';
        estado1 = false;
      }
    }

    //VERIFICA SI LA CUENTA SE ENCONTRO Y REGISTRA EN DONDE SE ENCONTRO
    if (bandVC == 'v' && estado1 == true ){
     // lcd.clear();
      user =0;
      cuentaExistente = true;
      lcd.clear();
      cr();
      lcd.print("MENU DE RECARGA");
      cr2();
      lcd.print("DINERO A GUARDAR");
      cl();
      lcd.print("$ ");
         
      estado1 = true;
    }

    //VERIFICA SI LA CUENTA NO SE ENCONTRO E IMPRIME QUE LA CUENTA NO SE ENCONTRO
    else if (bandVC == 'f' && estado1 == false){
      lcd.clear();
      cr();
      lcd.print("ERROR DE CUENTA");
      cr2();
      lcd.print("CUENTA NO REGISTRADA");
      band_default();
      cuentaExistente = false;
    }
  }

//SI LA CUENTA EXISTE ENTRA Y PREGUNTA CUANTO DINERO QUIERE DEPOSITAR
  if (cuentaExistente == true && estado1 == true ){
    //Serial.println(numCliente);
    char kye2 = keypad.getKey();
    if (kye2){
      dineroSave[indiceD] = kye2;
      lcd.print(kye2);
      indiceD ++;
      if (kye2 == '*'){
        band_default();
      }
      if (kye2 =='#'){
        lcd.clear();
        dineroint = atoi(dineroSave);
        cliente[numCliente].dinero = (cliente[numCliente].dinero + dineroint);
        cr();
        lcd.print("RECARGA REALIZADA");
        cr2();
        lcd.print("DEPOSITO EXITOSO");
        cl();
        lcd.print("DINERO: ");
        lcd.print(cliente[numCliente].dinero);
        delay(1500);
        Serial.print("NUEVO SALDO : ");
        Serial.println(cliente[numCliente].dinero);
        band_default();
      }
    }
  }


//RETIRO DE PRODUCTO DEPENDIENDO DE LA FORMA DE INGRESO DE ID Y CONTRASEÑA
  if (opc == 1 && estadoVC == false && retiroProducto == false){
    pedirID();
    band3 = 'f';
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      String idTarjeta = "";
      lcd.clear();
      cr();
      lcd.print("BUSCANDO ACOUNT.");
      for (byte i = 0; i < mfrc522.uid.size; i++){
        idTarjeta += String(mfrc522.uid.uidByte[i], HEX);
      }
      idTarjeta.toUpperCase();
      strcpy(VT, idTarjeta.c_str());
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      bandx = 'v';
      for(int i=0; i<contador; i++){
        if (!strcmp(VT, cliente[i].Tarjeta)){
          numCliente = i;
          band9 = 'v';
          bandx = 'f';
          break;
        }
        else{
          for(int i=0; i<contador; i++){
          if (!strcmp(VT, cliente[i].NFC)){
            numCliente = i;
            band9 = 'v';
            bandx = 'f';
            break;
          }
          else{
            band9 = 'f';
            bandx = 'g';
          }
          }
         }
        }
      }
      band5 = 'f';
    }

      
    if (band9 == 'f' && bandx == 'g' && opc == 1 && retiroProducto == false ){
      delay(500);
      band9 = 'g';
      band_default();
      lcd.clear();
      lcd.print("RFID NO EXISTE");
      lcd.clear();
    }
    else if (band9 == 'v' && bandx == 'f' && opc == 1 && retiroProducto == false){
         
      delay(500);
      retiroProducto = true;
    }

  if (opc == 1 && estadoVC == true && retiroProducto == false){
    
    if(band3 == 'f' ){
      lcd.clear();
      nombre();
      cr2();
      lcd.print("DIGITE PASSWORD");
      cl();
      lcd.print("-> ");
      band3 = 'v';
      indice = 0 ;
    }
    char key = keypad.getKey();
    if (key){
      passVF[indice] = key;
      lcd.print(key);
      indice++;
      if (key== '*'){
        band_default();
      }
    }
    if (indice == 4){
      if (!strcmp(passVF,cliente[numCliente].password)){
        lcd.clear();
        cr();
        lcd.print("EXITO");
        band3 = 'f';
        retiroProducto = true;
        indice = 0;
      }
      else{
        lcd.clear();
        cr();
        lcd.print("ERROR");
        band_default(); //EN CASO DE QUE LA CONTRASEÑA NO SEA CORRECTA VOLVERA AL MENU PRINCIPAL
      }
    }

  }

  if (opc == 1 && retiroProducto == true ){
    if (SelecProducto == 0){
      if (band3 == 'f'){
        lcd.clear();
        cr();
        lcd.print("AREA DE PRODUCTO");
        cr2();
        lcd.print("USUARIO -> ");
        lcd.print(numCliente + 1);
        band3 = 'v';
      }
      char key = keypad.getKey();
      cl();
      lcd.print("1.GALLETA 2.ZIBAS");
      cl2();
      lcd.print("3.JUGO    4.TAKIS");
      if (key){
        switch (key){
          case '1': SelecProducto = 1; lcd.clear(); cr(); lcd.print("MAXIMO-> "); lcd.print(Producto1); delay(500); lcd.clear(); cr(); lcd.print("SELECT PRODUCT"); cr2(); lcd.print("GALLETA $20");cl(); lcd.print("CANTIDAD -> "); precio = 20; break;
          case '2': SelecProducto = 2; lcd.clear(); cr(); lcd.print("MAXIMO-> "); lcd.print(Producto2); delay(500); lcd.clear(); cr(); lcd.print("SELECT PRODUCT"); cr2(); lcd.print("ZIBAS $50");cl(); lcd.print("CANTIDAD -> "); precio = 50; break;
          case '3': SelecProducto = 3; lcd.clear(); cr(); lcd.print("MAXIMO-> "); lcd.print(Producto3); delay(500); lcd.clear(); cr(); lcd.print("SELECT PRODUCT"); cr2(); lcd.print("JUGO $25");cl(); lcd.print("CANTIDAD -> ");  precio = 25; break;
          case '4': SelecProducto = 4; lcd.clear(); cr(); lcd.print("MAXIMO-> "); lcd.print(Producto4); delay(500); lcd.clear(); cr(); lcd.print("SELECT PRODUCT"); cr2(); lcd.print("TAKIS $30");cl(); lcd.print("CANTIDAD -> "); precio = 30; break;
          case '*': band_default(); break;
          default: lcd.clear(); cr(); lcd.print("ERROR DE OPCION"); delay(1000); lcd.clear(); cr(); lcd.print("CLIENTE -> "); lcd.print(numCliente + 1);  break;
        }
        
      }
    }
    if (SelecProducto != 0){
      if (band3 == 'v'){
      switch (SelecProducto){
          case 1: CTP = Producto1; break;
          case 2: CTP = Producto2; break;
          case 3: CTP = Producto3; break;
          case 4: CTP = Producto4; break;
        }
        band3= 'h';
      }
      
      if (cantidadProducto == 0 && CTP > 0 ){
        char key = keypad.getKey();
        if (key){
          cantchar[indiceD] = key;
          lcd.print(key);
          indiceD ++;
          if (key == '*'){
            band_default();
          }
          if (key =='#'){
            cantidadProducto = atoi(cantchar);
            if(cantidadProducto <= CTP){
              montoFinal = cantidadProducto * precio;
              cl();
              lcd.print("                ");
              cl(); 
              lcd.print("TOTAL -> ");
              lcd.print(montoFinal);
              delay(500);
              lcd.clear();
              if (cliente[numCliente].dinero >= montoFinal){
                cr();
                lcd.print("PAGO EXITOSO");
                cliente[numCliente].dinero -= montoFinal ;

                switch(SelecProducto){
                  case 1: Producto1 -= cantidadProducto  ;break;
                  case 2: Producto2 -= cantidadProducto  ;break;
                  case 3: Producto3 -= cantidadProducto  ;break;
                  case 4: Producto4 -= cantidadProducto  ;break;
                }
                delay(500);
                band_default();
              }
            else{
              cr();
              lcd.print("ESTIMADO CLIENTE SU");
              cr2();
              lcd.print("SALDO INSUFICIENTE");
              band_default();
            }
          }
          else if (cantidadProducto > CTP){
            lcd.clear();
            cr();
            lcd.print("OCURRIO UN PROBLEMA");
            cr2();
            lcd.print("NO HAY PRODUCTO");
            cl();
            lcd.print("SUFICIENTE");
            delay(500);
            band_default();
            lcd.clear();
            }
          }
        } 
      }
      else if (cantidadProducto == 0 && CTP <= 0 ){
        lcd.clear();
        cr();
        lcd.print("SIN PRODUCTOS");
        delay(500);
        band_default();

      }
    }
  }
}

////////////////////////////////////////////////////////////// <<<<< VOID >>>>> ////////////////////////////////////////////////////////////

///////// INICIALIZACION DE LA LCD /////////
void lcdInit(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
  cr();
  lcd.print("READING");
  cargando();
}

void cargando(){
  for(int i=0; i<4; i++){
    lcd.print(".");
    delay(200);
  }
}
void nombre() {
  cr();
  lcd.print("SNACK VENDIN MACHINE");
}

///////// VOID  INICIA LA LECTURA DEL RFID //////////
void RFID(){
  SPI.begin();
  mfrc522.PCD_Init();
}

// VOID PARA LA POSICION DE LA PANTALLA LCD
// VOID PARA LA PRIMERA LINEA DE LA LCD
void cr(){
  lcd.setCursor(0, 0);
}

void cr2(){
  lcd.setCursor(0, 1);
}

// VOID PARA SEGUNDA LINEA DE LA LCD
void cl(){
  lcd.setCursor(0, 2);
}

void cl2 (){
  lcd.setCursor(0, 3);
}



//// CREACION DE NUEVA CUENTA DE USUARIO DE MANERA SENCILLA SIN NFC O TARJETA////
void nuevaCuenta(){
  // PEDIRA EL ID DE LA CUENTA
  if (contador == 0 && band == 'f' || opc == 2 && band == 'f'){
    nombre();
    cr2();
    lcd.print("INGRESE ID 4DIG  ");
    cl();
    lcd.print("ID: ");
    band = 'v';
    Serial.print("Cliente ");
    Serial.println(contador + 1);
  }

  // GUARDA EN LA ESTRUCTURA EL ID DE LA CUENTA
  if (contador == 0 && band == 'v' || opc == 2 && band == 'v'){
    char key = keypad.getKey();
    // BUCLE HASTA QUE EL ID ESTE EN EL RANGO DE 1000 - 9999
    do{
      if (key){
        dato[indice] = key;
        lcd.print(key);
        indice++;
        cliente[contador].id = atoi(dato);
        dato[indice] = '\0';
        if (key== '*'){
          band_default();
        }
      }

      // CUANDO SE INGRESE EL 4 DIGITO SE GURADARA AUNTOMATICAMENTE EN EL ID
      if (indice == 4 && cliente[contador].id >= 1000){
        indice = 0;
        band = 'p';
        lcd.clear();
      }

      // EL ID DEBE DE ESTAR EN EL RANGO DE 1000 A 9999
      else if (indice == 4 && cliente[contador].id < 1000){
        indice = 0;
        cr();
        lcd.print("ERROR ID < 1000");
        band = 'f';
        lcd.clear();
      }
    } while (cliente[contador].id >= 1000 && band == 'f');
  }
  
  // PEDIRA LA CONTRASEÑA DEL CLIENTE CON UN TOTAL DE 4 CARACTERES
  if (contador == 0 && band == 'p' || opc == 2 && band == 'p'){
    cr();
    lcd.print("INGRESE PASSWORD ");
    delay(500);
    cr2();
    lcd.print("PASSWORD      ");
    cl();
    lcd.print("PASS: ");
    band = 'a';
    indice = 0;
  }

  // GUARDARA LA CONTRASEÑA DENTRO DE PASSWORD DEL CLIENTE
  if (contador == 0 && band == 'a' || opc == 2 && band == 'a'){
    char key = keypad.getKey();
    if (key){
      if (key== '*'){
        band_default();
      }
      dato[indice] = key;
      lcd.print(key);
      indice++;
    }

    // UNA VES INGRESADO LOS CUATRO DIGITOS IMPRIME EN EL MONITOR SERIAL LOS DATOS DEL CLIENTE
    if (indice == 4){
      strncpy(cliente[contador].password, dato, sizeof(cliente[contador].password) - 1);
      cliente[contador].password[sizeof(cliente[contador].password) - 1] = '\0';
      indice = 0;
      lcd.clear();
      band = 't';
    }
  }

  // VERIFICACION SI TIENE TARJETA
  if (contador == 0 && band == 't' || opc == 2 && band == 't'){
    char key = keypad.getKey();
    char banda = 'f';
    // char bandaa = 'f';
    if (banda == 'f'){
      cr();
      lcd.print("DESEA AGREGAR");
      cr2();
      lcd.print("TARJETA RFID?");
      cl();
      lcd.print("SI-> #  NO-> 0");
      band3 = 'v';
    }
  }

  // VERIFICA QUE SI PRESIONO * PARA LECTURA DEL LA RFID O # PARA CANCELAR LA LECTURA DEL RFID
  if (contador == 0 && band == 't' && band3 == 'v' || opc == 2 && band == 't' && band3 == 'v'){
    char key = keypad.getKey();
    if (key){
      switch (key){
      case '#': band = 'q'; lcd.clear(); break;
      case '0': band = 'n'; strcpy(cliente[contador].Tarjeta, "NO REGISTRADA");
        lcd.clear();break;
      //SI PRESIONA EL * VOLVERA AL MENU PRINCIPAL
      case '*': 
        band_default(); break;
      default: lcd.clear(); cr(); lcd.print("ERROR"); delay(500); lcd.clear(); break;
      }
    }
    // SI PRESIONO  * PEDIRA QUE ESCANEE LA TARJETA
    if (contador == 0 && band == 'q' && band3 == 'v' || opc == 2 && band == 'q' && band3 == 'v'){
      tarjeta();
    }
  }

  // VERIFICACION SI TIENE NFC
  if (contador == 0 && band == 'n' || opc == 2 && band == 'n'){
    char key = keypad.getKey();
    char banda = 'f';
    char bandaa = 'f';
    if (banda == 'f'){
      cr();
      lcd.print("DESEA AGREGAR NFC");
      cr2();
      lcd.print("A LA CUENTA?");
      cl();
      lcd.print("  SI-> #    NO-> 0");
      band3 = 'v';
    }
  }

  // SI PRESIONA * PEDIRA QUE ESCANEE EL NFC
  if (contador == 0 && band == 'n' && band3 == 'v' || opc == 2 && band == 'n' && band3 == 'v'){
    char key = keypad.getKey();
    if (key){
      switch (key){
      case '#': band = 'e'; break;
      //SI PRESIONA EL * VOLVERA AL MENU PRINCIPAL
      case '*': 
        band_default(); break;
      case '0': strcpy(cliente[contador].NFC, "NFC NO REGISTRADO"); band = 'z'; fin = true; break;
      default: lcd.clear(); cr(); lcd.print("ERROR"); delay(500); lcd.clear(); break;
      }
    }
  }

  if (contador == 0 && band == 'e' && band3 == 'v' || opc == 2 && band == 'e' && band3 == 'v'){
    nfcEscaner();
  }

  // UNA VEZ TERMINADO TODO EL PORCESO DE CREACION DE CUENTA AGREGA EL CLIENTE
  if (band == 'z' && fin == true){
    Serial.print("ID: ");
    Serial.println(cliente[contador].id);
    Serial.print("PASSWORD: ");
    Serial.println(cliente[contador].password);
    Serial.print("DINERO: ");
    Serial.println(cliente[contador].dinero);
    Serial.print("TARJETA: ");
    Serial.println(cliente[contador].Tarjeta);
    Serial.print("NFC: ");
    Serial.println(cliente[contador].NFC);
    Serial.println(" ");
    contador ++;
    band_default();
  }
} // FIN DEL INGRESO DE CUENTA



//////// VERIFICACION DE LA CLAVE PARA PODER RECARGAR O INGRESAR NUEVA CUENTA
void verificacion_clave_maestra(){
  //TARJETE MAESTRA PARA QUE SEA MENOS TIEMPO

  if (Cverificacion == false && opc == 2 && band3 == 'v'){
    char key = keypad.getKey();
    if (band2 == 'v'){
      cr();
      lcd.print("VERIFICACION");
      cr2();
      lcd.print("PASSWORD MASTER");
      cl();
      lcd.print("KEY: ");
      band2 = 'f';
      indice = 0;
    }
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      String idTarjeta = "";
      lcd.clear();
      nombre();
      cr2();
      lcd.print("VERIF MASTER TARGET");
      cl();
      lcd.print("WAITINNG"); 
      cargando();

      for (byte i = 0; i < mfrc522.uid.size; i++){
        idTarjeta += String(mfrc522.uid.uidByte[i], HEX);
      }
      idTarjeta.toUpperCase();
      strcpy(VT, idTarjeta.c_str());
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      //bandx = 'v';
        if (!strcmp(VT, MASTERCARD)){
          Cverificacion = true;
          indice = 0;
          lcd.clear();
        }
        else{
          lcd.clear();
          nombre();
          cr2();
          lcd.print("ERROR MS-CARD");
          opc = 0;
          lcd.clear();
          indice = 0;
          band2 = 'v';
          band3 = 'f';
         }
      }

    //  MUESTRA EN LA PANTALLA LA CONTRASEÑA PERO DE MANERA DE XXXXXXX
    if (key){
      if (key== '*'){
        band_default();
      }
      clavei[indice] = key;
      lcd.print("X");
      indice++;
    }

    // CUANDO LA CONTRASEÑA TENGA UN RANGO DE 6 DIGITOS LA COMPARA CON LA CONTRASEÑA MAESTRA
    if (indice == 6 && Cverificacion == false){
      lcd.clear();
      // COMPARA PARA LA GENERACION DE UNA NUEVA CUENTA
      if (!strcmp(clavei, clave)){
        Cverificacion = true;
        indice = 0;
      }

      // COMPARA PARA PODER IMPRIMIR LOS DATOS DE LAS CUENTAS DE LOS CLIENTES
      else if (!strcmp(clavei, clavePrint)){
        CverificacionPrint = true;
        cr();
        lcd.print("CUENTAS PRINT");
        indice = 0;
        opc = 0;
        band2 = 'v';
        band3 = 'f';
        lcd.clear();
      }

      // SI LA CONTRASEÑA NO COINCIDE ENTONCES DICE QUE LA CLAVE ES INCORRECTA Y VUELVE A SU MENU PRINCIPAL
      else{
        lcd.clear();
        cr();
        lcd.print("CLAVE INCORRECTA");
        opc = 0;
        lcd.clear();
        indice = 0;
        band2 = 'v';
        band3 = 'f';
      }
    }

    // En caso que en ese tiempo no se ingrese ningun caracter
    if (contador > 0){
      retardo();
      if (segundos == tiempo_anterior){
        lcd.clear();
        cr();
        lcd.print("TIEMPO AGOTADO");
        delay(500);
        opc = 0;
        lcd.clear();
        indice = 0;
        band2 = 'v';
        band3 = 'f';
        band6 = 'f';
      }
    }
  }
}

// MENU PRINCIPAL DE LAS ACCIONES QUE SE PUEDEN HACER
void opcion_menu_principal(){
  if (opc == 0 && contador > 0){

    if (estadotiempo == true){
      timea = segundos + 15;
      estadotiempo = false;
      estadowifi = false;
    }

    if (segundos >= timea) {
    if (wifiMulti.run() == WL_CONNECTED) {
        estadowifi = true;
        timea = segundos + 15;
    } else {
        estadowifi = false;
        lcd.clear();
        cr();
        lcd.print("RECONECTANDO");
        cargando();
        if (wifiMulti.run() != WL_CONNECTED) {
          Serial.println("No conectado a ninguna red WiFi. Intentando reconexión...");
          wifiMulti.addAP("POCO-F3", "Jose Rizo");
          wifiMulti.addAP("Casa Dalmata", "Primos2021");
          wifiMulti.addAP("WiFi-Estudiante", "UNANManagua*");
          timea = segundos + 15;
        }

        if (wifiMulti.run() == WL_CONNECTED) {
            Serial.print("Conectado a la red: ");
            Serial.println(WiFi.SSID());
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            lcd.clear();
            cr();
            lcd.print("CONECTADO");
            delay(500);
            lcd.clear();
        } else {
            cr();
            lcd.print("ERROR DE CONEXION");
            delay(500);
            lcd.clear();
        }
    }
}

//estadotiempo = false;


    char key = keypad.getKey();
    nombre();
    cr2();
    lcd.print("      PRESIONE  ");
    cl();
    lcd.print("AVANZADO -> *");
    cl2();
    lcd.print("RETIRO PRODUCTO -> #");
    estado = 0;
    // VERIFICACION DE LA ACCION QUE QUIERE HACER EL CLIENTE
    if (key){
      switch (key){
      // OPCIONES QUE REQUIEREN QUE INGRESE CONTRASEÑA PARA INGRESAR UNA CUENTA
      case '*': opc = 2; Cverificacion = false; band3 = 'f'; band6 = 'f'; lcd.clear(); estadotiempo = false; break;
      // EN CASO DE QUE QUIERA PROCEDER A RETIRAR UN  PRODUCTO
      case '#': opc = 1; subopc = 1; lcd.clear(); band6 = 'f'; estadotiempo = false; break;
      // EN CASO DE QUE PRESIONE UN TECLA QUE NO ESTE EN EL MENU
      default: lcd.clear(); cr(); lcd.print("ERROR DE OPCION"); delay(500); lcd.clear(); estadotiempo = false; break;
      }
    }
  }
}
// MENSAJE DE VERIFICACION
void mensaje_verificacion(){
  opc = 2;
  Cverificacion = false;
  indice = 0;
  lcd.clear();
  cr();
  lcd.print("VERIFICACION");
  delay(300);
  band3 = 'v';
}

// LECTURA DE TARJETA VOID
void tarjeta(){
  while (band == 'q'){
    if (band5 == 'f'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ESCANEE LA TARJETA");
      lcd.setCursor(0, 1);
      lcd.print("WAINTING");
      band5 = 'g';
    }

    // SI SE PRESENTA UNA TARJETA EN EL LECTOR LA LEE Y ALMACENA LA ID DENTRO DE TARJEA DE CLIENTE
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
      String idTarjeta = "";
      lcd.clear();
      for (byte i = 0; i < mfrc522.uid.size; i++){
        idTarjeta += String(mfrc522.uid.uidByte[i], HEX);
      }
      idTarjeta.toUpperCase();
      strcpy(cliente[contador].Tarjeta, idTarjeta.c_str()); // COPIA A LA ESTREUCTURA DEL CLIENTE
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      // TERMINA LA LECTURA DEL RFID
      band = 'n'; // PASA ALA VERFICACION DEL NFC
      lcd.clear();
      cr();
      lcd.print("TARJETA SAVE");
         
      lcd.clear();
      band5 = 'f';
    }
  }
}

// VOID DEL ESCANEO NFC ///
void nfcEscaner(){
  while (band == 'e'){
    if (band5 == 'f'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ESCANEE NFC");
      lcd.setCursor(0, 1);
      lcd.print("WAINTING...");
      band5 = 'g';
    }
    if (band5 == 'g'){
      // SI SE PRESENTA UN NFC EN EL LECTOR LO LEE Y LO ALMACENA DENTRO DE NFC DEL CLIENTE
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
      {
        lcd.clear();
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          idTarjeta1 += String(mfrc522.uid.uidByte[i], HEX);
        }
        idTarjeta1.toUpperCase();
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        if (band5 == 'g'){
          lcd.clear();
          cr();
          lcd.print("EXITO"); 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("RESCANEE NFC");
          lcd.setCursor(0, 1);
          lcd.print("WAINTING...");
          band5 = 'h';
        }
      }
    }
    if (band5 =='h'){
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
      {
        lcd.clear();
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          idTarjeta2 += String(mfrc522.uid.uidByte[i], HEX);
        }
        idTarjeta2.toUpperCase();
        Serial.println(idTarjeta1);
        Serial.println(idTarjeta2);
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        band5 ='v';
      }
    }
    if (band5 == 'v'){
      if (!strcmp(idTarjeta1.c_str(), idTarjeta2.c_str())){
          strcpy(cliente[contador].NFC, idTarjeta1.c_str()); // COPIA A LA ESTREUCTURA DEL CLIENTE
          lcd.clear();
          
          cr();
          lcd.print("NFC COMPATIBLE");
          cl();
          lcd.print("NFC SAVE");
          lcd.clear();
          band5 = 'f';
          band = 'z';
          fin = true;
        }
        else{
          strcpy(cliente[contador].NFC, "NO COMPATIBLE"); // COPIA A LA ESTREUCTURA DEL CLIENTE
          lcd.clear();
          cr();
          lcd.print("NO COMPATIBLE");
          cl();
          lcd.print("NFC ERROR");
          lcd.clear();
          band5 = 'f';
          band = 'z';
          fin = true;
         }
    }
  }
}

//VOID PARA RETARDO
void retardo(){
  if (band6 == 'f'){
    tiempo_anterior = segundos + 10;
    band6 = 'v';
  }
}


void pedirID (){
  char key = keypad.getKey();
  if (band7 == 'f'){
      cr();
      lcd.print("INGRESE SU USERS");
      cr2();
      lcd.print("->  ");
      band7 = 'v';
      indice = 0;
    }
    
  if (key){
    if (key== '*'){
      band_default();
    }
    idVF[indice] = key;
    lcd.print(key);
    indice++;
    if (indice == 4){
    user = atoi(idVF);
    }
  }
    if (indice == 4 && user >= 1000){
      indice = 0;
      lcd.clear();
      band7 = 'f';
      subopc = 5;
  }
    // EL ID DEBE DE ESTAR EN EL RaANGO DE 1000 A 9999
    else if (indice == 4 && user < 1000){
      indice = 0;
      cr();
      lcd.print("ERROR ID < 1000");
      band7 = 'f';
      lcd.clear();
      subopc = 2; 
    }
}

//MANTIENE POR DEFECTO TODAS LAS VARIABLES DE TIPO BAND Y OTRAS PARA EL INICIO AL MENU PRINCIPAL
void band_default(){
  band9 = 'g'; bandx = 'g'; Cverificacion = false;
  opc = 0; user =0; bandVC = 'g'; band2 = 'v'; band3 = 'f'; band4 = 'f'; band5 = 'f'; band6 = 'f'; band7 = 'f'; band8 = 'f'; bandVC = 'g';
  indice = 0; band = 'f'; lcd.clear(); band2 = 'v'; fin = false; band3 = 'f'; opc = 0; guardarEnPreferences = true; subopc = 0; band7 = 'f';
  dineroint = 0; estado1 = false; indiceD = 0; estado1 = false;  estadoVC = false; cuentaExistente = false; retiroProducto = false;
  SelecProducto = 0; montoFinal = 0; precio = 0; cantidadProducto = 0; estadotiempo = true ;
}