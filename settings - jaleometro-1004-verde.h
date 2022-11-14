// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x77, 0xCB };
static uint8_t appEui[] = { 0x86, 0x19, 0x88, 0x14, 0xBC, 0x08, 0x64, 0x2B };
static uint8_t appKey[] = { 0x7C, 0x20, 0xAB, 0x8F, 0xD5, 0xE4, 0xAF, 0xF3, 0xFF, 0x8E, 0x2E, 0xE9, 0x5B, 0xF1, 0x76, 0xE7 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.57052181046839;
const float longitude = 2.652869339830219;
const int alt = 15;

// Other params
float SensorId= 10.04;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
