// LoRaWAN Configuration

/* OTAA para*/
uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x36, 0x98, 0x0B, 0xB6, 0x21, 0x9B, 0xDC, 0xA7, 0xA3, 0x03, 0x52, 0x2D, 0x7F, 0x93, 0x6E, 0x4C };
uint8_t appSKey[] = { 0x83, 0x24, 0x16, 0xCE, 0x42, 0x4B, 0x1D, 0xFA, 0xEC, 0x0C, 0x1B, 0x52, 0x04, 0x03, 0xA9, 0x71 };
uint32_t devAddr =  ( uint32_t )0x260BAD01;

// Devise location
const float latitude = 39.570569;
const float longitude = 2.653495;
const int alt = 5;

// Other params
float SensorId= 10.03;// Sensor  identifcator number 
const int ReadDutyCycle = 600000; // Transmision and reading period ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++