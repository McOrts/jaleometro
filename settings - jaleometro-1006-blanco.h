// LoRaWAN Configuration

/* OTAA para*/
uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x8B, 0x87, 0x1A, 0x94, 0xB6, 0x07, 0x75, 0x69, 0x91, 0x32, 0x48, 0x25, 0x98, 0xDE, 0xC0, 0x7B };
uint8_t appSKey[] = { 0xE5, 0x48, 0x9C, 0x63, 0x70, 0x57, 0xB8, 0x7E, 0xAC, 0xDD, 0xE4, 0x57, 0x33, 0x49, 0x8F, 0xA3 };
uint32_t devAddr =  ( uint32_t )0x260B9304;

// Devise location
const float latitude = 39.57046579260317;
const float longitude = 2.652842424622102;
const int alt = 5;

// Other params
float SensorId= 10.06;// Sensor  identifcator number 
const int ReadDutyCycle = 600000; // Transmision and reading period ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++