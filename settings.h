// LoRaWAN Configuration

/* ABP para*/
uint8_t nwkSKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };
uint8_t appSKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };
uint32_t devAddr =  ( uint32_t )0x260B954D;

// Devise location
const float latitude = 39.536;
const float longitude = 2.718;
const int alt = 30;

// Other params
float SensorId= 10.02;// Sensor  identifcator number 
const int ReadDutyCycle = 600000; // Transmision and reading period
