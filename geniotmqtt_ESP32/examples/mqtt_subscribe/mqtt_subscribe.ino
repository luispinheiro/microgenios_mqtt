
#include <WiFi.h>
#include <geniotmqtt_ESP32.h>

/*
  SSID e Password da rede WiFi
*/
const char* ssid      = " ";
const char* password  = " ";

const char * Temperatura  = "temperatura";  
const char * ClientID     = "ClientID_GUID"; //defina um nome único para cada conexão/device!!

WiFiClient espClient;
Mqttlib client(espClient);

unsigned long int lastReconnectAttempt = 0;
 
/*
  A função de callback é chamada todas as vezes que alguma mensagem do tópico 
  que foi assinado for recebida pela Api MQTT.
  topic   => informa o nome/string do tópico recebido.
  payload => Pacote de dados armazenado em um vetor de bytes.
  length  => Quantidade de bytes da mensagem recebida.

  A função de callback é defina por client.setCallback(Nome_do_callback);
*/
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
 
}
 
void setup() {
 
  Serial.begin(9600);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    /*
       No Mqtt a conexão cada dispositivo conectado ao broker precisa ter um nome único.
       Por esse motivo, cada device precisa ter um nome diferente defino em "ClientID_GUID" 
    */
    if (client.connect(ClientID, "login", "senha")) { //Exemplo: "user1" , "321"
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  /*
    A função client.subscribe(Temperatura) assina a variável "Temperatura". 
    Assim, este device receberá todas as mensagens envidas para "Temperatura" por meio
    da função de callback.  
  */
  client.subscribe(Temperatura);

  lastReconnectAttempt = 0;
 
}

boolean reconnect() {
  return client.connected();
}


void loop() {

/*
  No Mqtt, os devices normalmente são mantidos conectados ao broker. Por esse motivo, 
  faz necessário a chamada continua em um loop da função mqtt.
*/

 if(!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }

}