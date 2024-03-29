// LoRaWAN Configuration

/* OTAA para*/
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t devEui[] = { 0x16, 0x7E, 0x5E, 0x17, 0x82, 0xA1, 0x52, 0x45 };
static uint8_t appKey[] = { 0x0C, 0xF8, 0x58, 0x60, 0x84, 0xAB, 0xD9, 0x22, 0x91, 0xA5, 0xB1, 0xF3, 0xDA, 0xD7, 0x7F, 0xDF };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.572813;
const float longitude = 2.637521;
const int alt = 9;

// Other params
float SensorId= 11.04;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 39; // Limit for low noise mode
