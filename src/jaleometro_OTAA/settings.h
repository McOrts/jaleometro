// LoRaWAN Configuration

/* OTAA para*/
static uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t devEui[] = { };
static uint8_t appKey[] = { };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = ;
const float longitude = ;
const int alt = ;

// Other params
float SensorId= 00.00;// Sensor  identifcator number 
const int DutyCycle = 120000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
int LowNoiseLevel = 36; // Limit for low noise mode
int noiseDiffSleep = 0; // UPDATED by downlink Port 1. Is the difference in sound level between the minimum and average in a period of sample. 
const bool moixMode = false; // Activate the red & green LED interface and logic
const int moixRedLevel = 41; // Limit for low noise mode
const bool indoor = false; // sleep mode is never activated
const bool wifiModule = false;// WiFi and motion module present
