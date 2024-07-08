/**
 *
 * Carlos Orts
 */
#include "LoRaWanMinimal_APP.h"
#include <CayenneLPP.h>//the library is needed https://github.com/ElectronicCats/CayenneLPP
#include "Arduino.h"
#include "settings.h"

#define Pin_LED_Red GPIO0
#define Pin_LED_Green GPIO4

int loops; // number of readings
float cycles; // number of read and transmisions cycles.
float icycles; // delta for cycles counter.
float noise_avg; // noise level average
float noise_avg_pre; // noise level average previous
unsigned int noise_peak; // noise peak value
unsigned int noise_min; // noise minimun value
unsigned int noise; // current noise value
unsigned long noise_sum; // noise level addition

uint16_t battery;

unsigned long tmp_ini; 
long CountStart = 0;
long now_DutyCycle = 0;

CayenneLPP lpp(LORAWAN_APP_DATA_MAX_SIZE);//if use AT mode, don't modify this value or may run dead https://github.com/HelTecAutomation/CubeCell-Arduino/search?q=commissioning.h

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
	Serial.begin(115200);

if (moix == true) {
  // initialize digital pin for LED as an output.
  pinMode(Pin_LED_Red, OUTPUT);
  digitalWrite(Pin_LED_Red, LOW);
  pinMode(Pin_LED_Green, OUTPUT);
  digitalWrite(Pin_LED_Green, HIGH);
}
  // This enables (with LOW) the output to power the sensor
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  Serial.printf(" | Vext: ");
  Serial.println(digitalRead(Vext));
  delay(500);

  // Sensor identification
  Serial.printf(" | SensorId: ");
  Serial.println(SensorId);

  tmp_ini = millis(); 
  noise_avg = 0;
  noise_avg_pre = LowNoiseLevel;
  noise_peak = 0;
  noise_min = 1000; 
  noise_sum = 0;
  loops = 0;
  cycles = 50;
  icycles = 1;
  noise_avg = LowNoiseLevel;

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
      Serial.println("JOIN FAILED! Sleeping for 60 seconds");
      lowPowerSleep(60000);
    } else {
        Serial.println("JOINED");
        if (LORAWAN_CLASS == CLASS_A) {
          Serial.println("CLASS A Confirmed");
        } else {
           Serial.println("ERROR MOT CLASS A");
        }
      break;
    }
  }

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
 
  // Cycles control
  if (cycles>99) {
    icycles = -1 ;
  } else if (cycles<1){
    icycles = +1 ;
  }
  cycles += icycles;
   
  // Cayenne
  lpp.reset();
  lpp.addAnalogInput(1,SensorId);
  lpp.addAnalogInput(2,cycles);
  lpp.addAnalogInput(3,battery);
  lpp.addLuminosity(1, noise_avg);
  lpp.addLuminosity(2, noise_peak);
  lpp.addLuminosity(3, noise_min);    
  lpp.addGPS(2, latitude, longitude, alt);

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

  if (noise > 4500) {
    Serial.print("outlier removed: ");
    Serial.println(noise);
 } else {
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
      // LED control of MOIX
      if (moix == true && noise > MoixRedLevel) {
        digitalWrite(Pin_LED_Green, LOW);
        digitalWrite(Pin_LED_Red, HIGH);
        Serial.println("Red");
      } else {
        digitalWrite(Pin_LED_Red, LOW);
        digitalWrite(Pin_LED_Green, HIGH);
        Serial.println("Green");
      }

    }
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

  long now_DutyCycle = millis();
  if (now_DutyCycle - CountStart > DutyCycle) {
    CountStart = now_DutyCycle;

    // Noise calculations
    noise_avg = int(noise_sum / loops);
    Serial.print("Noise average: ");
    Serial.println(noise_avg);
    Serial.println(noise_sum);
    Serial.println(loops);

    // Swith off-on the power of the sensor in order to read the battery
    digitalWrite(Vext, HIGH);
    delay(1000);
    battery = getBatteryVoltage();
    digitalWrite(Vext, LOW);
    Serial.print("Battery: ");
    Serial.println(battery);
    delay(100);

    transmitRecord();

    noise_peak = 0;
    noise_min = 1000;
    noise_sum = 0;
    loops = 0;
  
    // Low Noise Mode if two Noise Overage are under LowNoiseLevel
    if (noise_avg < LowNoiseLevel && noise_avg_pre < LowNoiseLevel) {
      if (LORAWAN_CLASS == CLASS_A) {
        cycles -= icycles;
        digitalWrite(Vext, HIGH);
        Serial.println("Switch OFF Micro");
        Serial.println("Sleep");
        digitalWrite(Pin_LED_Red, LOW);
        digitalWrite(Pin_LED_Green, LOW);
        delay(400);
        lowPowerSleep(Sleep4NoNoise); 
        digitalWrite(Vext, LOW);
        Serial.println("Switch ON Micro");
      }
    } 
    noise_avg_pre = noise_avg;
  }
}

///////////////////////////////////////////////////
//Example of handling downlink data
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("Received downlink: %s, RXSIZE %d, PORT %d, DATA: ",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
}
