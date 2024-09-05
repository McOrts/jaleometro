// LoRaWAN Configuration

/* OTAA para*/
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t devEui[] = { 0x6A, 0x01, 0x1E, 0xFE, 0x1D, 0x85, 0xF2, 0xDF };
static uint8_t appKey[] = { 0xD6, 0x57, 0x6C, 0x9B, 0xE3, 0xD6, 0xDA, 0x05, 0xDD, 0x7A, 0x4C, 0xA3, 0x60, 0xA2, 0x70, 0xB5 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.634708582824864;
const float longitude = 2.6343323359171507;
const int alt = -5;

// Other params
float SensorId= 12.05;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 39; // Limit for low noise mode
const bool moix = true;
const int MoixRedLevel = 41; // Limit for low noise mode
