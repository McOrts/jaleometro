/**
 *
 * Carlos Orts
 */
#include "LoRaWanMinimal_APP.h"
#include <CayenneLPP.h>//the library needed is https://github.com/ElectronicCats/CayenneLPP with version 1.0.1
#include "Arduino.h"
#include "settings.h"
#include "softSerial.h"

#define Pin_LED_Red GPIO0
#define Pin_LED_Green GPIO4

int loops; // number of readings
unsigned int cycles; // number of read and transmisions cycles.
float icycles; // delta for cycles counter.
float noise_avg; // noise level average
float noise_avg_pre; // noise level average previous
unsigned int noise_peak; // noise peak value
unsigned int noise_min; // noise minimun value
unsigned int noise; // current noise value
unsigned long noise_sum; // noise level addition

///////////////////////////////////////////////////
String outgoingString; // readings in JSON string sent to WiFi module
String incomingString; // readings received from WiFi module
bool wifiConnected; // WiFi module status
int motionVibrations; // Wifi vibration sensor output
softSerial softwareSerial(GPIO1 /*TX pin*/, GPIO2 /*RX pin*/);

///////////////////////////////////////////////////
unsigned long tmp_ini; // Time control reading period
int loops_legal; // number of readings
float noise_avg_legal; // noise level average PMI
float noise_avg_legal_max; // noise level average PMI previous
unsigned int noise_avg_legal_period = 5000; // period of time in miliseconds for legal measurement 
unsigned long noise_sum_legal; // noise level addition
long LegalStart = 0;

float battery;

long CountStart = 0;

CayenneLPP lpp(LORAWAN_APP_DATA_MAX_SIZE);//if use AT mode, don't modify this value or may run dead https://github.com/HelTecAutomation/CubeCell-Arduino/search?q=commissioning.h

bool lorawanConnected; // WiFi module status

///////////////////////////////////////////////////
//Some utilities for going into low power mode
TimerEvent_t sleepTimer;
//Records whether our sleep/low power timer expired
bool sleepTimerExpired;
static void wakeUp()
{
  sleepTimerExpired=true;
}

static void lowPowerSleep(uint32_t sleeptime)
{
  sleepTimerExpired=false;
  TimerInit( &sleepTimer, &wakeUp );
  TimerSetValue( &sleepTimer, sleeptime );
  TimerStart( &sleepTimer );
  //Low power handler also gets interrupted by other timers
  //So wait until our timer had expired
  while (!sleepTimerExpired) lowPowerHandler();
  TimerStop( &sleepTimer );
}

///////////////////////////////////////////////////
void setup() {
	Serial.begin(38400);

  // This disabled (with HIGH) the power of the microphone
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH );
  Serial.printf(" | Vext: ");
  Serial.println(digitalRead(Vext));
  delay(500);

  if (moixMode) {
    // initialize digital pin for LED as an output.
    pinMode(Pin_LED_Red, OUTPUT);
    digitalWrite(Pin_LED_Red, LOW);
    pinMode(Pin_LED_Green, OUTPUT);
    digitalWrite(Pin_LED_Green, HIGH);
    Serial.println("MOIX mode activated");
  }

  if (wifiModule){
    softwareSerial.begin(38400);
    wifiConnected = wifiModule;
    motionVibrations = LOW;
  }

  // Sensor identification
  Serial.printf(" | SensorId: ");
  Serial.println(SensorId);

  if (ACTIVE_REGION==LORAMAC_REGION_AU915) {
    //TTN uses sub-band 2 in AU915
    LoRaWAN.setSubBand2();
  }
 
  LoRaWAN.begin(LORAWAN_CLASS, ACTIVE_REGION);
    
  //Enable ADR
  LoRaWAN.setAdaptiveDR(true);

  while (1) {
    Serial.print("Joining... ");
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    if (!LoRaWAN.isJoined()) {
      //In this example we just loop until we're joined, but you could
      //also go and start doing other things and try again later
      if (wifiModule) {
        Serial.println("JOIN FAILED! proceeding with WiFi conectivity");
        lorawanConnected = false;
        break;
      } else {
        Serial.println("JOIN FAILED! Sleeping for 60 seconds");
        lowPowerSleep(60000);
      }
    } else {
      Serial.println("JOINED");
      lorawanConnected = true;      
      // This enable (with LOW) the power of the microphone
      digitalWrite(Vext, LOW );
      if (LORAWAN_CLASS == CLASS_A) {
        Serial.println("CLASS A Confirmed");
      } else {
        Serial.println("ERROR MOT CLASS A");
      }
      break;
    }
  }

  tmp_ini = millis(); 
  CountStart = millis();
  noise_avg = LowNoiseLevel;
  noise_avg_legal = LowNoiseLevel;
  noise_avg_pre = LowNoiseLevel;
  noise_avg_legal_max = LowNoiseLevel;
  noise_peak = 0;
  noise_min = 1000; 
  noise_sum = 0;
  noise_sum_legal = 0;
  loops = 0;
  loops_legal = 0;
  cycles = 50;
  icycles = 1;

}

void transmitRecord()
{
  /*
  * set LoraWan_RGB to Active,the RGB active in loraWan
  * RGB red means sending;
  * RGB purple means joined done;
  * RGB blue means RxWindow1;
  * RGB yellow means RxWindow2;
  * RGB green means received done;
  */
   
  // Cayenne
  lpp.reset();
  lpp.addAnalogInput(1,SensorId);
  lpp.addAnalogInput(2,cycles);
  lpp.addAnalogInput(3,battery);
  lpp.addAnalogInput(4,LowNoiseLevel);
  lpp.addLuminosity(1, noise_avg);
  lpp.addLuminosity(2, noise_peak);
  lpp.addLuminosity(3, noise_min);    
  lpp.addLuminosity(4, noise_avg_legal_max); 
  lpp.addLuminosity(5, motionVibrations); 

  //lpp.addGPS(2, latitude, longitude, alt);

  Serial.println("Transmiting...");
  Serial.print("lpp data size: ");
  Serial.print(lpp.getSize());
  Serial.println();
  
  // Request ACK only for the 3 firts transmision of a cycle
  bool requestack=cycles<3?true:false;
  
  // if (LoRaWAN.send(1, lpp.getBuffer(), lpp.getSize(), requestack)) {
  if (LoRaWAN.send(lpp.getSize(), lpp.getBuffer(), 2, requestack)) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send FAILED");
  }
}

///////////////////////////////////////////////////
void loop() {
  // Noise reading each second
  if (digitalRead(Vext) == 0) {
    noise = analogReadmV(ADC);
  } else {
      digitalWrite(Vext, LOW);
  }

  // ++++++++++++++  Remove outlier ++++++++++++++++
  if (noise > 4500) {
    Serial.print("outlier removed: ");
    Serial.println(noise);
  } else { 
    // Recalculate the LowNoiseLevel
    if (noise < LowNoiseLevel) {
      LowNoiseLevel = noise;
    }
    
    // ++++++++++++++  LAeq based on samples taken every second ++++++++++++++++
    if (millis() - tmp_ini > 1000) {
      noise_sum += noise;
      loops ++;

      Serial.print("Noise: ");
      Serial.print(noise);
      Serial.print(" loop: ");
      Serial.print(loops);
      Serial.print(" cycles: ");
      Serial.println(cycles);
      tmp_ini = millis(); 
    }

    // ++++++++++++++  Noise legal calculations ++++++++++++++++
    loops_legal ++;
    noise_sum_legal += noise;
    if (millis() - LegalStart > noise_avg_legal_period) {

      noise_avg_legal = int(noise_sum_legal / loops_legal);
      Serial.print("   (Legal) noise_avg_legal: ");
      Serial.print(noise_avg_legal);
      Serial.print(" noise_avg_legal_max: ");
      Serial.print(noise_avg_legal_max);
      Serial.print(" loops: ");
      Serial.println(loops_legal);

      if (noise_avg_legal > noise_avg_legal_max) {
        noise_avg_legal_max = noise_avg_legal;
        Serial.print("Noise legal current average: ");
        Serial.println(noise_avg_legal_max);
      }

      // LED control of MOIX
      if (moixMode) {
        if (noise_avg_legal > moixRedLevel) {
          digitalWrite(Pin_LED_Green, LOW);
          digitalWrite(Pin_LED_Red, HIGH);
          Serial.println("Red");
        } else {
          digitalWrite(Pin_LED_Red, LOW);
          digitalWrite(Pin_LED_Green, HIGH);
          Serial.println("Green");
        }
      }

      loops_legal = 0;
      noise_sum_legal = 0;
      LegalStart = millis();
    }

    // ++++++++++++++  Maximun and Minimun detection ++++++++++++++++ 
    if (noise > noise_peak) {
      noise_peak = noise;
      Serial.print("Noise peak: ");
      Serial.println(noise_peak);
    }
    if (noise < noise_min) {
      noise_min = noise;
      Serial.print("Noise min: ");
      Serial.println(noise_min);
    }
  }

  if (millis() - CountStart > DutyCycle) {
    CountStart = millis();

    // Noise calculations
    noise_avg = int(noise_sum / loops);
    Serial.print("Noise average: ");
    Serial.println(noise_avg);
    Serial.print("Noise sum: ");
    Serial.println(noise_sum);
    Serial.print("Loops: ");
    Serial.println(loops);

    // Swith off-on the power of the microphone in order to read the battery
    digitalWrite(Vext, HIGH);
    delay(500);
    battery = getBatteryVoltage();
    Serial.print("Battery: ");
    Serial.println(battery);
    battery = battery / 100;

    // Cycles control
    if (cycles>99) {
      icycles = -1 ;
    } else if (cycles<1){
      // re-iniciate the LowNoiseLevel to avoid low levels like 0
      LowNoiseLevel = noise_min;
      icycles = +1 ;
    }
    cycles += icycles;

    // WiFi module data sharing
    if (wifiModule) {
      outgoingString = "{";
      outgoingString += "\"analog_in_1\":" + String(SensorId, 2) + ",";
      outgoingString += "\"analog_in_2\":" + String(cycles) + ",";
      outgoingString += "\"analog_in_3\":" + String(battery,2) + ",";
      outgoingString += "\"analog_in_4\":" + String(LowNoiseLevel) + ",";
      outgoingString += "\"luminosity_1\":" + String(noise_avg) + ",";
      outgoingString += "\"luminosity_2\":" + String(noise_peak) + ",";
      outgoingString += "\"luminosity_3\":" + String(noise_min) + ",";
      outgoingString += "\"luminosity_4\":" + String(noise_avg_legal_max);
      outgoingString += "}";
      softwareSerial.println(outgoingString);
      Serial.print("Sent Serial: ");
      Serial.println(outgoingString);
      delay (500);
      while(softwareSerial.available()) {
        char serialbuffer[64] = {0};
        int i = 0;
        while (softwareSerial.available() && i<63) {
          serialbuffer[i] = (char)softwareSerial.read();
          i++;
        }
        serialbuffer[i] = '\0';
        if(serialbuffer[0]) {
          Serial.print("Received Serial: ");
          Serial.println(serialbuffer);
          char *commaPos = strchr(serialbuffer, ','); // Find the comma position
          wifiConnected = (strncmp(serialbuffer, "true", 4) == 0); // Check if the start is "true"
          motionVibrations = atoi(commaPos + 1); // Convert the number part to an integer
          Serial.print("wifiConnected: ");
          Serial.println(wifiConnected);
          Serial.print("motionVibrations: ");
          Serial.println(motionVibrations);
        }
      }
    }

    if (lorawanConnected) {
      transmitRecord();
    }  
    
    digitalWrite(Vext, LOW);
    delay(100);
  
    // Low Noise Mode if two Noise Overage are under LowNoiseLevel 
    if (noise_avg < LowNoiseLevel + noiseDiffSleep && noise_avg_pre < LowNoiseLevel + noiseDiffSleep && indoor == false) {
      if (LORAWAN_CLASS == CLASS_A) {
        cycles -= icycles;
          Serial.print("noise_avg: ");
          Serial.println(noise_avg);
          Serial.print("LowNoiseLevel: ");
          Serial.println(LowNoiseLevel);
          Serial.print("noiseDiffSleep: ");
          Serial.println(noiseDiffSleep);
          Serial.print("noise_avg_pre: ");
          Serial.println(noise_avg_pre);
        digitalWrite(Vext, HIGH);
        Serial.println("Switch OFF Microphone");
        Serial.println("Sleep");
        digitalWrite(Pin_LED_Red, LOW);
        digitalWrite(Pin_LED_Green, LOW);
        delay(400);
        lowPowerSleep(Sleep4NoNoise); 
        digitalWrite(Vext, LOW);
        Serial.println("Switch ON Microphone");
      }
      CountStart = millis();
    } 
    
    noise_peak = 0;
    noise_min = 1000;
    noise_sum = 0;
    loops = 0;
    noise_avg_legal_max = 0;
    noise_avg_pre = noise_avg;
  }
}

// Handling downlink data
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("Received downlink: %s, RXSIZE %d, PORT %d, DATA: ",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
    if (mcpsIndication->Port == 1) {
      noiseDiffSleep = mcpsIndication->Buffer[i];
      Serial.print(" | noiseDiffSleep: ");
      Serial.println(noiseDiffSleep);
    }
  }
  Serial.println();
}
