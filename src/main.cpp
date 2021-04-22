#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <PubSubClient.h>

#define OUTPUT              (1<<3)

void setMqttClient();
void connectMqtt();
String levelOutputFromReadTemperatue();
void publishVentilateurMoteur();
void subscribeActionTopic();
 
const char * moteurTopic = "VentilateurMoteur";
const char * capteurTempTopic = "CapteurTemperatureV";

Ethernet wsa;
EthernetClient ethClient;
PubSubClient mqttClient;

void setup() {
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  setMqttClient();
}

void loop() {
  connectMqtt();
  publishVentilateurMoteur();
  delay(6000);
  mqttClient.loop();
}

// connect to mqtt
void setMqttClient(){
  mqttClient.setClient(ethClient);
  mqttClient.setServer("mqtt.flespi.io", 1883);
  //mqttClient.setCallback(actionCallback);
}

//connection a une session (= capteur par exemple)
void connectMqtt() {
  const char * PASSWORD = "6CxabSSDNaJLgC8xdjdrCR2PagdQ42W0shYuZHXCHubMW89YfafGR7jQbmw6eiUw";
  Serial.println("connecting to Mqtt moteur ...");
  if(!mqttClient.connect(moteurTopic, PASSWORD, "")){
    Serial.println("connection failed !!");
  }else{
    Serial.println("connection succes !!");
  }
  Serial.println("connecting to Mqtt capteur temperature ...");
  if(!mqttClient.connect(capteurTempTopic, PASSWORD, "")){
    Serial.println("connection failed !!");
  }else{
    Serial.println("connection succes !!");
  }
}

//ex suscribe capteur
void subscribeActionTopic()
{
  String finalTopic = "CapteurTemperatureV";
  if(!mqttClient.subscribe(finalTopic.c_str()))
  {
    Serial.println("Topic led action subscribe error");
  }
  Serial.println("Topic led action subscribe success");
}

//Set Level From read TemperatureValue
String levelOutputFromReadTemperatue() {
  String level = "1";
  return level;
}

//ex publish Ventilateur moteur
void publishVentilateurMoteur()
{
  if(!mqttClient.connected())
  {
    Serial.println("Unable to publish ventlateurMoteur value since mqtt broker insn't connect");
    return;
  }
  String finalTopic = "VentilateurMoteur/level";
  if(!mqttClient.publish(finalTopic.c_str(), levelOutputFromReadTemperatue().c_str() ))
  {
    Serial.println("Unable to publish ventilateur moteur value..");
  }else{
    Serial.println("Ventilateur moteur value published ");
  }
}

// void switchLed(bool on)
// {
//   if(on)
//   {
//     digitalWrite(LED_PIN, 1);
//   }
//   else
//   {
//     digitalWrite(LED_PIN, 0);
//   }
//   if(!mqttClient.publish(ledStateTopic.c_str(), String(on).c_str()))
//   {
//     Serial.println("Unable to publish led state value..");
//   }
// }

String payloadToString(byte *payload, unsigned int length)
{
  char buffer[length];
  sprintf(buffer, "%.*s", length, payload);
  return String(buffer);
}

void actionCallback(char * topicChar, byte* payloadByte, unsigned int length)
{
  Serial.println("New message received");
  String topic = String(topicChar);
  String payload = payloadToString(payloadByte, length);

  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  Serial.println(payload);

  // if(!topic.equals("ee"))
  // {
  //   return;
  // }
  // bool a;
  // if(payload.equalsIgnoreCase("on"))
  // {
  //   //switchLed(true);
  // }
  // else if(payload.equalsIgnoreCase("off"))
  // {
  //   //switchLed(false);
  // }
}