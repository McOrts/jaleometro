// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x77, 0x9E };
static uint8_t appEui[] = { 0x12, 0x38, 0x49, 0x60, 0x95, 0xC0, 0xA2, 0x09 };
static uint8_t appKey[] = { 0xFB, 0x14, 0x97, 0x6C, 0xBE, 0xFB, 0xD3, 0xE8, 0x92, 0x8B, 0xED, 0x12, 0x2D, 0x57, 0xE9, 0x19 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.57046579260317;
const float longitude = 2.652842424622102;
const int alt = 5;

// Other params
float SensorId= 10.06;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
