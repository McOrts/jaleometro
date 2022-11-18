// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x77, 0xCA };
static uint8_t appEui[] = { 0xAA, 0xF0, 0xAC, 0x38, 0xBF, 0xF4, 0x74, 0xF5 };
static uint8_t appKey[] = { 0x8A, 0xC5, 0x4C, 0x39, 0xBF, 0xAA, 0xCF, 0xDD, 0x68, 0xD3, 0x80, 0x0C, 0xE5, 0x36, 0xA9, 0x08 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.570569;
const float longitude = 2.653495;
const int alt = 5;

// Other params
float SensorId= 10.03;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
