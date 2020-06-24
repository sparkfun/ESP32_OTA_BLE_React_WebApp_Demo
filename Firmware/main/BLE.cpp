/*
  Copyright (c) 2019 Andy England
  Header: BLE.h
*/

#include "BLE.h"

#define SERVICE_UUID_INFINITEPYRAMID        "e804b643-6ce7-4e81-9f8a-ce0f699085eb"
#define CHARACTERISTIC_UUID_IP_ID           "e804b644-6ce7-4e81-9f8a-ce0f699085eb"

#define SERVICE_UUID_OTA                    "b8659210-af91-4ad3-a995-a58d6fd26145" // UART service UUID
#define CHARACTERISTIC_UUID_FW              "b8659211-af91-4ad3-a995-a58d6fd26145"
#define CHARACTERISTIC_UUID_HW_VERSION      "b8659212-af91-4ad3-a995-a58d6fd26145"
#define CHARACTERISTIC_UUID_READY           "b8659213-af91-4ad3-a995-a58d6fd26145"

#define SOFTWARE_VERSION_MAJOR 0
#define SOFTWARE_VERSION_MINOR 1
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 3

#define FULL_PACKET 512
#define CHARPOS_UPDATE_FLAG 5

esp_ota_handle_t otaHandler = 0;

bool updateFlag = false;
bool readyFlag = false;
int bytesReceived = 0;
int timesWritten = 0;

void otaCallback::onWrite(BLECharacteristic *pCharacteristic)
{
  std::string rxData = pCharacteristic->getValue();
  if (!updateFlag) { //If it's the first packet of OTA since bootup, begin OTA
    Serial.println("BeginOTA");
    esp_ota_begin(esp_ota_get_next_update_partition(NULL), OTA_SIZE_UNKNOWN, &otaHandler);
    updateFlag = true;
  }
  if (_p_ble != NULL)
  {
    if (rxData.length() > 0)
    {
      esp_ota_write(otaHandler, rxData.c_str(), rxData.length());
      if (rxData.length() != FULL_PACKET)
      {
        esp_ota_end(otaHandler);
        Serial.println("EndOTA");
        if (ESP_OK == esp_ota_set_boot_partition(esp_ota_get_next_update_partition(NULL))) {
          esp_restart();
        }
        else {
          Serial.println("Upload Error");
        }
      }
    }
  }

  uint8_t txData[5] = {1, 2, 3, 4, 5};
  //delay(1000);
  pCharacteristic->setValue((uint8_t*)txData, 5);
  pCharacteristic->notify();
}

//
// Constructor
BLE::BLE(void) {

}

//
// Destructor
BLE::~BLE(void)
{

}

//
// begin
bool BLE::begin(const char* localName = "UART Service") {
  // Create the BLE Device
  BLEDevice::init(localName);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLECustomServerCallbacks());

  // Create the BLE Service
  pInfinitePyramidService = pServer->createService(SERVICE_UUID_INFINITEPYRAMID);
  pService = pServer->createService(SERVICE_UUID_OTA);

  // Create a BLE Characteristic
  pInfinitePyramidIdCharacteristic = pInfinitePyramidService->createCharacteristic(
                                       CHARACTERISTIC_UUID_IP_ID,
                                       BLECharacteristic::PROPERTY_READ
                                     );

  pVersionCharacteristic = pService->createCharacteristic(
                             CHARACTERISTIC_UUID_HW_VERSION,
                             BLECharacteristic::PROPERTY_READ
                           );

  pOtaCharacteristic = pService->createCharacteristic(
                         CHARACTERISTIC_UUID_FW,
                         BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE
                       );

  pOtaCharacteristic->addDescriptor(new BLE2902());
  pOtaCharacteristic->setCallbacks(new otaCallback(this));

  // Start the service(s)
  pInfinitePyramidService->start();
  pService->start();

  // Start advertising
  pServer->getAdvertising()->addServiceUUID(SERVICE_UUID_INFINITEPYRAMID);
  pServer->getAdvertising()->start();

  uint8_t hardwareVersion[5] = {HARDWARE_VERSION_MAJOR, HARDWARE_VERSION_MINOR, SOFTWARE_VERSION_MAJOR, SOFTWARE_VERSION_MINOR, SOFTWARE_VERSION_PATCH};
  pVersionCharacteristic->setValue((uint8_t*)hardwareVersion, 5);
  return true;
}
