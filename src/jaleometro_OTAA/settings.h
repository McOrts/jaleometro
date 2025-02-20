// LoRaWAN Configuration

/* OTAA para*/
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t devEui[] = {  };
static uint8_t appKey[] = {  };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 0.0;
const float longitude = 0.0;
const int alt = 5;

// Other params
float SensorId= 00.00;// Sensor  identifcator number 
const int DutyCycle = 120000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
int LowNoiseLevel = 34; // Limit for low noise mode
int noiseDiffSleep = 4; // UPDATED by downlink Port 1. Is the difference in sound level between the 
const bool moix = false;
const int MoixRedLevel = 41; // Limit for low noise mode
const bool indoor = false;