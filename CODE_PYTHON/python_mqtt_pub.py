# -*- coding: cp1252 -*-
import paho.mqtt.client as mqtt
import time
import random

# Mosquitto MQTT
username 	= "user1"
password 	= "123"
broker_address 	= "pc-fernando"
port 		= 1883

# Funções de Callback dos eventos
# on_connect = função chamada quando ocorre a conexão entre o cliente e o broker MQTT.
# on_message = função chamada quando uma mensagem de um tópico assinado for recebido.
# on_publish = função chamada quando uma mensagem for publicada. 
# on_subscribe = função chamada quando um tópico for assinado pelo cliente MQTT.
# on_log = função para debug do Paho.

Connected = False   #global variable 

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker")
        global Connected                #Use global variable
        Connected = True                #Signal connection 
    else:
        print("Connection failed")
 
def on_message(client, obj, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload) )
	
def on_publish(client, obj, mid):
    print("mid: " + str(mid))

def on_subscribe(client, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(client, obj, level, string):
    print(string)

#------------------------------------------------
mqttc = mqtt.Client("clientidUnico")

# Assina as funções de callback
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe

# Uncomment to enable debug messages
#mqttc.on_log = on_log

# Connecta ao Mosquitto MQTT. 
# Informe o nome do usuário e senha. 
mqttc.username_pw_set(username=username, password=password)

# Carrega o certificado TLS.
#mqttc.tls_set("C:/Program Files (x86)/mosquitto/certs/ca.crt")

# Informe o endereço IP ou DNS do servidor Mosquitto. 
# Use 1883 para conexão SEM TLS, e 8883 para conexão segura TLS.
mqttc.connect(broker_address, port=port)

# Assina o tópico, com QoS level 0 (pode ser 0, 1 ou 2)
# mqttc.subscribe("temperatura", 0)

mqttc.loop_start()        #start the loop

while Connected != True:    #Wait for connection
    time.sleep(0.1)
	
# Permanece em loop até a geração de uma excessão. 
try:
    while True:  
        value = random.randint(0,100)
        mqttc.publish("temperatura",value)
        time.sleep(5)
 
except:
    print "Falha de Leitura"
    mqttc.disconnect()
    mqttc.loop_stop()
