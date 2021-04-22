#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <PubSubClient.h>

#define OUTPUT              (0<<4)

void setMqttClient();
void connectMqtt();
String levelOutputFromReadTemperatue();
void publishVentilateurMoteur(int value);
void subscribeActionTopic();
String payloadToString();
void actionCallback(char * topicChar, byte* payloadByte, unsigned int length);
 
const char * moteurTopic = "VentilateurConnect";
//const char * capteurTempTopic = "CapteurTempGet";

Ethernet wsa;
EthernetClient ethClient;
PubSubClient mqttClient;
String none ="none";
String low = "low";
String normal = "normal";
String high = "high";


void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  setMqttClient();
}

void loop() {
  connectMqtt();
  mqttClient.loop();
}

// connect to mqtt
void setMqttClient(){
  mqttClient.setClient(ethClient);
  mqttClient.setServer("mqtt.flespi.io", 1883);
  mqttClient.setCallback(actionCallback);
}

//connection a une session (= capteur par exemple)
void connectMqtt() {
  if(mqttClient.connected()){
    return;
  }
  const char * PASSWORD = "6CxabSSDNaJLgC8xdjdrCR2PagdQ42W0shYuZHXCHubMW89YfafGR7jQbmw6eiUw";
  Serial.println("connecting to Mqtt ventilateur ...");
  if(!mqttClient.connect(moteurTopic, PASSWORD, "")){
    Serial.println("connection failed !!");
  }else{
    Serial.println("connection succes !!");
  }
  subscribeActionTopic();
}

//ex suscribe ventilateur connect level
void subscribeActionTopic()
{
  String finalTopic = "VentilateurConnect/level";
  if(!mqttClient.subscribe(finalTopic.c_str()))
  {
    Serial.println("Topic ventilateur level subscribe error");
  }else{
    Serial.println("Topic ventilateur level subscribe success");
  }
}


String payloadToString(byte *payload, unsigned int length)
{
  char buffer[length];
  sprintf(buffer, "%.*s", length, payload);
  return String(buffer);
}

// suscribe callback
void actionCallback(char * topicChar, byte* payloadByte, unsigned int length)
{
  Serial.println("New message received");
  String topic = String(topicChar);
  String payload = payloadToString(payloadByte, length);
  int payloadInt = payload.toInt();

  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  Serial.println(payload);

  if(!topic.equals("VentilateurConnect/level"))
  {
    return;
  }else{
    if(payloadInt == 1) {
      digitalWrite(9, 1);
    }else if(payloadInt == 2){
      digitalWrite(9, 2);
    }else if(payloadInt == 3){
      digitalWrite(9, 3);
    }else if(payloadInt == 4){
      digitalWrite(9, 4);
    }else{
      digitalWrite(9, 0);
    }
  }
}