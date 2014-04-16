#include <nRF24L01_PhysicalLayer.h>
#include <nRFTransportProtocol.h>
#include "Util/Util.h"
#include "SPI.h"
#include "Message/Message.h"
#include "Message/PingMessage.h"
#include "Message/SensorData.h"
#include <IMessageHandler.h>

#define SELF_ADDRESS 11111
#define LIGHT_PIN A0

using namespace nRFTP;

class nRFTPExample : public IMessageHandler {
    void handleMessage(nRFTP::ByteBuffer& bb, uint8_t type){
      switch (type){
          case nRFTP::Message::TYPE_PING:
            break;

          case Message::TYPE_ROUTE:
            break;

          case Message::TYPE_SENSORDATA:
          {
				SensorData SensorData(bb);
				uint16_t tmp = SensorData.header.srcAddress;
				SensorData.header.srcAddress = SensorData.header.destAddress;
				SensorData.header.destAddress = tmp;

				switch(SensorData.sensorType){
					case SensorData::TYPE_BATTERY:
					{
						break;
					}

					case SensorData::TYPE_LIGHT:
					{
						SensorData.sensorData = analogRead(LIGHT_PIN);

						break;
					}

					case SensorData::TYPE_CURRENT:
					{
						break;
					}

					case SensorData::TYPE_TEMPERATURE:
					{
						break;
					}

					default:
						break;

				}
				SensorData.copyToByteBuffer(bb);
				delay(20);
				//sendMessage(bb, SensorData.header.destAddress);

            break;
            }


          default:
            break;
      }
    }

    void pingResponseArrived(uint16_t milis, uint16_t destAddress){
        Serial.print("Pinged "); Serial.print(destAddress); Serial.print(": "); Serial.print(milis); Serial.println(" ms");
    }
};

nRFTPExample example;

nRF24L01_PhysicalLayer pLayer(Util::TPAddress_to_nRF24L01Address(SELF_ADDRESS),9,10);
nRFTransportProtocol test_nRFTP(&pLayer, SELF_ADDRESS, &example);

void setup() {
  Serial.begin(57600);
  test_nRFTP.begin();

  Serial.println(sizeof(Header));
  Serial.println("csumpa");

  pinMode(LIGHT_PIN, INPUT);
}

void loop() {
  test_nRFTP.run();

  if ( Serial.available() )
  {
	delay(10);
	char addr[5];
    Serial.readBytes(addr, 5);
    test_nRFTP.ping((uint16_t)atoi(addr));
  }
}
