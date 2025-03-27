// Identification 
const char* SensorId= "";// Sensor identifcator number 
const char* SensorName= ""; 

// MQTT Configuration
const char* mqtt_server = "";
const int   mqtt_port = ;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqtt_id = "";
const char* mqtt_sub_topic_payload = "devices/jaleometros_wifi/data/";
const char* mqtt_sub_topic_ip = "devices/jaleometros_wifi/ip/";
const char* mqtt_sub_topic_healthcheck = "devices/jaleometros_wifi/healthcheck/";
const char* mqtt_sub_topic_operation = "devices/jaleometros_wifi/operation/";
const char* mqtt_sub_topic_motion = "/sensors_raw/motion/";
// Motion sensor params SW18010P
#define MOTION_ANA_IN_PIN 3
#define MOTION_DIG_IN_PIN 9
