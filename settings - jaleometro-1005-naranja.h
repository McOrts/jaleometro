// LoRaWAN Configuration

/* OTAA para*/
uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* ABP para*/
uint8_t nwkSKey[] = { 0xD2, 0xF8, 0x63, 0xB5, 0x3F, 0x90, 0x78, 0x1B, 0x6A, 0x0F, 0x6A, 0xDE, 0xD3, 0xB7, 0xA4, 0xA9 };
uint8_t appSKey[] = { 0x9E, 0x8C, 0x5A, 0x0C, 0xBC, 0x38, 0x00, 0x89, 0xAC, 0x1C, 0xED, 0x96, 0xD9, 0x7F, 0x5D, 0x0F };
uint32_t devAddr =  ( uint32_t )0x260B608D;

// Devise location
const float latitude = 39.5726447770259;
const float longitude = 2.652504173970349;
const int alt = 30;

// Other params
float SensorId= 10.05;// Sensor  identifcator number 
const int ReadDutyCycle = 600000; // Transmision and reading period ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++