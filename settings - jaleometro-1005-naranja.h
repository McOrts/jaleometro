// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x4F, 0x6C };
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t appKey[] = { 0x23, 0x8C, 0xB0, 0x9C, 0x35, 0xEE, 0x36, 0x69, 0x81, 0xA8, 0xAE, 0x3E, 0xBD, 0x30, 0xA0, 0x48 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.5726447770259;
const float longitude = 2.652504173970349;
const int alt = 12;

// Other params
float SensorId= 10.05;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
