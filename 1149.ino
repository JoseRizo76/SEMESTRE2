#include <WiFi.h>
#include <WiFiMulti.h>
#include <CTBot.h>
 CTBot miBot;
 
WiFiMulti wifiMulti;

const char *token = "6690611788:AAGq0ELx30JZK1xgUdX03nptYGhirfkbB3M";

const char *ssid1 = "POCO-F3";
const char *password1 = "Jose Rizo";
const char *ssid2 = "Casa Dalmata";
const char *password2 = "Primos2021";
const char *ssid3 = "WiFi-Estudiante";
const char *password3 = "UNANManagua*";


int dinero = 0;
boolean estado = false;

void setup() {
  Serial.begin(115200);
  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3);

  Serial.print("CONECTANDO A WIFI...");
  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("Conectado a la red: ");
      Serial.println(WiFi.SSID());
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("Conexión WiFi exitosa");
  }
  else{
    Serial.print("ERROR DE CONEXION WIFI VERIFIQUE LA CONEXION ");
    delay(500);
  }
  miBot.wifiConnect(WiFi.SSID().c_str(), WiFi.psk().c_str());
  miBot.setTelegramToken(token);

  if (miBot.testConnection()) {
    Serial.println("CONECTADO A TELEGRAM");
  } else {
   Serial.println("PROBLEMAS DE CONEXION CON TELEGRAM");
  }
}


void loop() {

  TBMessage msg;
  if (CTBotMessageText == miBot.getNewMessage(msg)) {
    String text = msg.text;
    text.toLowerCase();

    if (msg.text.equalsIgnoreCase("url")) {
      String direccion = "Bienvenido "+msg.sender.firstName + "\n\nDirección IP Local: http://";
      direccion += WiFi.localIP().toString() + "\n\nRecuerde!\nPara poder acceder a la página web, tiene que estar conectado a la red WiFi → " + WiFi.SSID();
      direccion += "\n\nGracias por su tiempo ♡";
      miBot.sendMessage(msg.sender.id, direccion);
    } else if (text.startsWith("cuenta ")) {
      String accountIDStr = text.substring(7);
      int accountID = accountIDStr.toInt();

      if (accountID >= 1000 && accountID <= 9999) {
        bool found = false;
        String response;
        do {
          Serial.println(accountID);
          if (Serial.available() > 0) {
            String mensaje = Serial.readString(); // Lee hasta encontrar un salto de línea
            dinero = mensaje.toInt();
            if (dinero > 0 && dinero != 10000001) {
              response = "Bienvenido Cliente → 2 \n\n";
              response += "Cuenta → " + String(accountID) + "\n";
              response += "El saldo de tu cuenta es: C$ " + String(dinero) + "\n\n";
              response += "Gracias por su tiempo ♡";
              found = true;
              estado = false;
            }
            else{
              estado = true;
              found = true;
            }
          }
        } while (!found );

        if (found && estado == false) {
          miBot.sendMessage(msg.sender.id, response);
        } else if(estado == true) {
          miBot.sendMessage(msg.sender.id, "LA CUENTA NO SE ENCONTRO");
          estado = false;
        }
      } else {
        miBot.sendMessage(msg.sender.id, "EL ID DE LA CUENTA TIENE UN RANGO ENTRE 1000 Y 9999 \nEJEMPLO → cuenta 1000 o cuenta 9999 ");
      }
    } else {
      miBot.sendMessage(msg.sender.id, "Bienvenido → " + msg.sender.firstName + "\n\nINTENTA USAR\n\nURL → Para conocer la Direccion de la pagina\n\nCuenta XXXX → Para conocer el saldo de la su Cuenta\nReemplaze XXXX Por el ID de su cuenta\n ");
    }
  }
}
