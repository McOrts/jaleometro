// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x77, 0xCD };
static uint8_t appEui[] = { 0x3A, 0x2E, 0xEB, 0x52, 0x39, 0x09, 0xE4, 0xBC };
static uint8_t appKey[] = { 0x8C, 0x76, 0x0E, 0x14, 0x00, 0x10, 0x37, 0xCF, 0x50, 0xF2, 0x11, 0x74, 0xFD, 0x9E, 0x85, 0xFC };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.57093438859319;
const float longitude = 2.653670848075559;
const int alt = 12;

// Other params
float SensorId= 10.02;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
