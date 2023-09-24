#include "Arduino.h"

#define SWITCH_ADC_PIN GPIO1 // signal to switch

unsigned long int c;
uint16_t battery;
float voltage;
uint16_t level;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  int c = 1;
  pinMode(SWITCH_ADC_PIN, OUTPUT);
  digitalWrite(SWITCH_ADC_PIN, LOW);

  // This enables the output to power the sensor
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

}


void loop() {
  // put your main code here, to run repeatedly:
  c ++;
  battery = getBatteryVoltage();

  //return adc level 0-4095;
  level = analogRead(ADC); 

  /*return the voltage in mV
  * for asr650x, max value can be read is 2400mV
  * for asr6601,value can be read is 100mV-1100mV
  */
  voltage = analogReadmV(ADC);

  Serial.print("ADC level: ");
  Serial.print(level);
  Serial.print(", voltage: ");
  Serial.print(voltage);
  Serial.print(", battery: ");
  Serial.print(battery);
  Serial.printf(" | GPIO1: ");
  Serial.print(digitalRead(SWITCH_ADC_PIN));
  Serial.printf(" | Vext: ");
  Serial.println(digitalRead(Vext));

  delay(1000);
  if (c == 20) {
    digitalWrite(SWITCH_ADC_PIN, HIGH);
    // Turn the power to the sensor off again
    digitalWrite(Vext, HIGH);
  }
  if (c == 40) {
    digitalWrite(SWITCH_ADC_PIN, LOW);
    digitalWrite(Vext, LOW);
  }
}