// LoRaWAN Configuration

/* OTAA para*/
uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* ABP para*/
uint8_t nwkSKey[] = { 0xFE, 0x1F, 0xAB, 0x5B, 0x19, 0x90, 0x17, 0x40, 0x10, 0x5A, 0xA1, 0xCF, 0x03, 0x67, 0x78, 0xE9 };
uint8_t appSKey[] = { 0xFA, 0xE2, 0x10, 0xEC, 0xCA, 0x03, 0xD5, 0xBC, 0xFA, 0xE5, 0xE8, 0x44, 0x05, 0x33, 0x04, 0x7E };
uint32_t devAddr =  ( uint32_t )0x260B239D;

// Devise location
const float latitude = 39.57093438859319;
const float longitude = 2.653670848075559;
const int alt = 30;

// Other params
float SensorId= 10.02;// Sensor  identifcator number 
const int ReadDutyCycle = 600000; // Transmision and reading period 