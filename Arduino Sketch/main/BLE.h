#ifndef _BLE_H_
#define _BLE_H_

#include "Arduino.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "esp_ota_ops.h"

#define SOFTWARE_VERSION_MAJOR 0
#define SOFTWARE_VERSION_MINOR 1
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 2

class BLE; // forward declaration

class BLECustomServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      // deviceConnected = true;
      // // code here for when device connects
    };

    void onDisconnect(BLEServer* pServer) {
      // deviceConnected = false;
      // // code here for when device disconnects
    }
};

class otaCallback: public BLECharacteristicCallbacks {
  public:
    otaCallback(BLE* ble) {
      _p_ble = ble;
    }
    BLE* _p_ble;

    void onWrite(BLECharacteristic *pCharacteristic);
};

class BLE
{
  public:

    BLE(void);
    ~BLE(void);

    bool begin(const char* localName);
  
  private:
    String local_name;

    BLEServer *pServer = NULL;

    BLEService *pESPOTAService = NULL;
    BLECharacteristic * pESPOTAIdCharacteristic = NULL;

    BLEService *pService = NULL;
    BLECharacteristic * pVersionCharacteristic = NULL;
    BLECharacteristic * pOtaCharacteristic = NULL;
};

#endif
