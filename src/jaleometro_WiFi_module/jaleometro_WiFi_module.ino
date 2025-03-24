/*****************************************************
 * Date: Febrary 2025 * Written by: McOrts
 * 
 * ***************************************************/

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include "settings.h"
#include <Arduino.h>

// Serial communications ports
#define RXD1 1
#define TXD1 0

String incomingString;
String outcomingString;

// WiFI variables
WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )
boolean WifiConnected;

// MQTT variables
String mqttcommand = String(14);
char msg[50];
WiFiClient espClient;
PubSubClient clientMqtt(espClient);
String IP = String(15);
int MQTTConnectionAttempts = 0;

// MOtion variables
int motionDetected = LOW;
int motionDetectedPast = LOW;
int motionValue = LOW ;
int motionVibrations = LOW;
int motionMax = LOW;

void setup() {

  Serial.begin(38400); 
  Serial1.begin(38400, SERIAL_8N1, RXD1, TXD1);

  String custom_radio_str = "<br/><label for='customfieldid'>Sensor Id: </label><input type='radio' name='customfieldid' value='1' checked> " + String(SensorId);
  new (&custom_field) WiFiManagerParameter(custom_radio_str.c_str()); // custom html input
  wm.addParameter(&custom_field);
  wm.setConfigPortalTimeout(90);
  wm.setCustomHeadElement("<h2>IoT-Foundations.org</h2>");
  //wm.setCustomHeadElement("<h3>NoiseQX</h3>");

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("Jaleometro"); // anonymous ap
  // res = wm.autoConnect("Jaleometro","password"); // password protected ap

  if(!res) {
    Serial.println("Failed to connect Wifi");
    WifiConnected = false;
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("Wifi connected :)");
    WifiConnected = true;

    IP=IpAddress2String(WiFi.localIP());
  
    // Connect to MQTT broker
    clientMqtt.setServer(mqtt_server, mqtt_port);
    clientMqtt.setCallback(callback);
  }

  // Port motion configuration SW18010P
  pinMode(MOTION_ANA_IN_PIN, INPUT);
  pinMode(MOTION_DIG_IN_PIN, INPUT_PULLUP);
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]); 
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT]Message received (");
  Serial.print(topic);
  Serial.print(") ");
  mqttcommand = "";
  for (int i = 0; i < length; i++) {
    mqttcommand += (char)payload[i];
  }
  Serial.print(mqttcommand);
  Serial.println();
  if (mqttcommand == "comando") {
    Serial.println("comando");
  }  
}

void reconnect() {
  Serial.print("[MQTT]Trying to connect to MQTT server... ");
  // Bucle hasta conseguir conexión
  while (!clientMqtt.connected()) {
    Serial.print(".");
    // Intento de conexión
    if (clientMqtt.connect(mqtt_id, mqttUser, mqttPassword )) {
      Serial.println("");
      Serial.println("[MQTT]Connecting to MQTT server");
      // Once connected, publish an announcement...
      clientMqtt.publish((mqtt_sub_topic_healthcheck + String(mqtt_id)).c_str(), "starting");
      clientMqtt.publish((mqtt_sub_topic_ip + String(mqtt_id)).c_str(), IP.c_str());

      // ... and subscribe
      clientMqtt.subscribe((mqtt_sub_topic_operation + String(mqtt_id)).c_str());
      Serial.println("[MQTT]Connected to MQTT server");
      MQTTConnectionAttempts = 0;
      WifiConnected = true;
    } else {
      Serial.print("[MQTT]Error, rc=");
      Serial.println(clientMqtt.state());
      if (MQTTConnectionAttempts < 4) {
        Serial.println("[MQTT]Trying to connect to MQTT server in 5 seconds");
        MQTTConnectionAttempts ++;
        delay(5000);
      } else {
        // RESTART if lost the WiFi communication 
        ESP.restart();
      }
    }
  }
}

void loop() {
  if (WifiConnected) {
    if (!clientMqtt.connected()) {
      reconnect();
      clientMqtt.publish((mqtt_sub_topic_healthcheck + String(mqtt_id)).c_str(), "ok");
    }
    clientMqtt.loop();
  }

  while(Serial1.available()) {
    incomingString = Serial1.readStringUntil('\n');// read the incoming data as string
    Serial.print("Received Serial: ");
    Serial.println(incomingString);
    outcomingString = String(WifiConnected ? "true" : "false") +"," + String(motionVibrations);
    Serial1.println(outcomingString);
    Serial.print("Sent Serial: ");
    Serial.println(outcomingString);

    if (WifiConnected) {
      // Append motionVibrations to the JSON string
      incomingString.remove(incomingString.length() - 2);  // Remove the last closing brace
      incomingString += ",\"motion\":" + String(motionVibrations) + "}";
      // Send noise MQTT message
      clientMqtt.publish ((mqtt_sub_topic_payload + String(SensorName)).c_str(), (char*)incomingString.c_str());
      Serial.print ("Sent MQTT: ");
      Serial.println (incomingString);
      // Send motion MQTT message
      delay (500);
      snprintf (msg, 10, "%6i", motionVibrations);
      clientMqtt.publish ((mqtt_sub_topic_motion + String(SensorName)).c_str(), msg);   
      Serial.print ("Sent MQTT: ");
      Serial.println (msg);
    }
    motionVibrations = LOW;
  }

  // Motion sensor
  motionDetected = digitalRead(MOTION_DIG_IN_PIN)*1000;
  motionValue = analogRead(MOTION_ANA_IN_PIN);
  if (motionValue < 4095) {
    if (motionDetectedPast != motionDetected) {
      motionVibrations += 1;
      motionDetectedPast = motionDetected;
    }
  }
}
