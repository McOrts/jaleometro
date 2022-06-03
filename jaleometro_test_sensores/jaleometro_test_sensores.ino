#include "Arduino.h"
float voltage=0;
uint16_t ruido=0;

void setup() {
  Serial.begin(115200);
  uint64_t chipID=getID();
  Serial.printf("ChipID:%04X%08X\r\n",(uint32_t)(chipID>>32),(uint32_t)chipID);

}


void loop() {

  voltage=getBatteryVoltage()/1000;
  ruido=analogReadmV(ADC);

  Serial.print("Voltaje: ");
  Serial.print(voltage);
  Serial.printf(",  Ruido: ");
  Serial.println(ruido);
  delay(5000);
}
