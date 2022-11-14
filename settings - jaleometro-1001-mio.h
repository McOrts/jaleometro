// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x77, 0x79 };
static uint8_t appEui[] = { 0x76, 0x15, 0xF3, 0x09, 0x37, 0x72, 0xA6, 0x55 };
static uint8_t appKey[] = { 0x42, 0xDD, 0xB2, 0x28, 0x09, 0xC0, 0x9E, 0xF3, 0x29, 0x10, 0xD9, 0xCB, 0xE7, 0xC7, 0xDA, 0x27 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.53573;
const float longitude = 2.71832;
const int alt = 20;

// Other params
float SensorId= 10.01;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
