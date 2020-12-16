
#include <WiFi.h>
#include <geniotmqtt_ESP32.h>

/*
  SSID e Password da rede WiFi
*/
const char* ssid      = "NetVirtua_5G_203";
const char* password  = "10881570";

const char * Temperatura  = "temperatura";  
const char * ClientID     = "ClientID2"; //defina um nome único para cada conexão/device!!

 
WiFiClient espClient;
Mqttlib client(espClient);

void setup() {
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    /*
       No Mqtt a conexão cada dispositivo conectado ao broker precisa ter um nome único.
       Por esse motivo, cada device precisa ter um nome diferente defino em "ClientID_GUID" 
    */
    if (client.connect(ClientID, "login", "senha")) {  //exemplo: "user1"  ,  "321"
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
}

boolean reconnect() {
  return client.connected();
}

void loop() {

  static const long interval = 5000;  
  static unsigned long previousMillis = 0, currentMillis;
  currentMillis = millis();

  if(currentMillis - previousMillis >= interval) 
   {
         previousMillis = currentMillis;
         client.publish(Temperatura, random(300));
   }

  if(!client.connected()) {
      delay(2000);
      if (reconnect()) {     
      }
    }  

    client.loop();
}