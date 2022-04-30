#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <CayenneLPP.h>//the library is needed ��https://github.com/ElectronicCats/CayenneLPP��
#include "settings.h"

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
uint32_t appTxDutyCycle = TransmitPeriod;

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
    CayenneLPP lpp(LORAWAN_APP_DATA_MAX_SIZE);
    lpp.addDigitalInput(1,FlowMeterPulses); 
    lpp.getBuffer(), 
    appDataSize = lpp.getSize();
    memcpy(appData,lpp.getBuffer(),appDataSize);
}

/* Read the sensor */
void flow () // Interrupt function
{
  FlowMeterPulses++;
  Serial.println (FlowMeterPulses);
}

void setup() {
	boardInitMcu();
	Serial.begin(115200);
  #if(AT_SUPPORT)
  	enableAt();
  #endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();

  /* Initialize the sensor port */
  pinMode(flowsensor, INPUT);
  //digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  attachInterrupt(flowsensor, flow, RISING); // CubeCell boar version
  //attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Arduino board version
}

void loop()
{

  /* Application port */
	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
      #if(AT_SUPPORT)
      			getDevParam();
      #endif
			printDevParam();
			LoRaWAN.init(loraWanClass,loraWanRegion);
			deviceState = DEVICE_STATE_JOIN;
      Serial.println ('Init'); 
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();
      Serial.println ('Join');
			break;
		}
		case DEVICE_STATE_SEND:
		{
			prepareTxFrame( appPort );
			LoRaWAN.send();
			deviceState = DEVICE_STATE_CYCLE;
      FlowMeterPulses = 0; // Reset Counter
      Serial.println ('Send');
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule next packet transmission
			txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
      Serial.println ('Cycle');
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
