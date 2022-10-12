#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <CayenneLPP.h>//the library is needed ��https://github.com/ElectronicCats/CayenneLPP��
#include "settings.h"

int loops; // number of readings
float cycles; // number of read and transmisions cycles.
float icycles; // delta for cycles counter.
float noise_avg; // noise level average
unsigned int noise_peak; // noise peak value
unsigned int noise_min; // noise minimun value
unsigned int noise; // current noise value
unsigned long noise_sum; // noise level addition
unsigned long tmp_ini; 
unsigned long tmp_pause; 
boolean run_init = true; // aboid to send in the first connection

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 60000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
	/*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
	*appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
	*if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
	*if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
	*for example, if use REGION_CN470, 
	*the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
	*/
  if (!run_init) {
    // Cycles control
    if (cycles>99) {
      icycles = -1 ;
    } else if (cycles<1){
      icycles = +1 ;
    }
    cycles += icycles;

    // Noise calculations
    noise_avg = int(noise_sum / loops);
    Serial.print("Noise average: ");
    Serial.println(noise_avg);
    Serial.println(noise_sum);
    Serial.println(loops);
    
    CayenneLPP lpp(LORAWAN_APP_DATA_MAX_SIZE);
    lpp.addAnalogInput(1,SensorId);
    lpp.addAnalogInput(2,cycles);
    lpp.addLuminosity(1, noise_avg);
    lpp.addLuminosity(2, noise_peak);
    lpp.addLuminosity(3, noise_min);    
    lpp.addGPS(2, latitude, longitude, alt);
    lpp.getBuffer(), 
    appDataSize = lpp.getSize();
    memcpy(appData,lpp.getBuffer(),appDataSize);
    Serial.print(appDataSize);
    Serial.println(F(" bytes long LPP packet queued."));    

    noise_peak = 0;
    noise_min = 1000;
    noise_sum = 0;
    loops = 0;
  } else {
    run_init = false;
  } 
}

void setup() {
	Serial.begin(115200);

  tmp_ini = millis(); 
  noise_avg = 0;
  noise_peak = 0;
  noise_min = 1000;  
  noise_sum = 0;
  loops = 0;
  cycles = 1;
  icycles = +1;
    
#if(AT_SUPPORT)
	enableAt();
#endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();
  Serial.print("deviceState: "); 
  Serial.println(deviceState); 
}

void loop()
{
	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
#if(LORAWAN_DEVEUI_AUTO)
			LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
			getDevParam();
#endif
			printDevParam();
			LoRaWAN.init(loraWanClass,loraWanRegion);
			deviceState = DEVICE_STATE_JOIN;
      Serial.println ("Init----------------------"); 
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();
      Serial.println ("Join----------------------");
			break;
		}
		case DEVICE_STATE_SEND:
		{
      voltage=getBatteryVoltage()/1000;
      Serial.print("Voltaje: ");
      Serial.print(voltage);
			Serial.print(" cycles: ");
      Serial.println(cycles);
			prepareTxFrame( appPort );
			LoRaWAN.send();
			deviceState = DEVICE_STATE_CYCLE;
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule next packet transmission
			txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
			break;
		}
		case DEVICE_STATE_SLEEP:
		{
			LoRaWAN.sleep();
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;
			break;
		}
	}
}
