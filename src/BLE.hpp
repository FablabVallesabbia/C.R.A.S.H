#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUID personalizzati
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcdefab-1234-5678-1234-abcdefabcdef"

#define SERVICE_UUID_COMMAND "feedbeef-1234-5678-1234-abcdefabcdef"
#define CHARACTERISTIC_UUID_ARRAY "beadfeed-1234-5678-1234-abcdefabcdef"
#define CHARACTERISTIC_UUID_ABILITAZIONE "deadbeef-1234-5678-1234-abcdefabcdef"


BLEServer* pServer = NULL;
BLECharacteristic* pBatteria = NULL;
BLECharacteristic *pDatoRicevuto = nullptr;
BLECharacteristic *pNomeTelecomando = nullptr;
BLECharacteristic *pAbilitazione = nullptr;
BLECharacteristic *pArrayInt = nullptr;

#define UpdateBLETime 100 // tempo di aggiornamento BLE in ms


extern int DatoRicevuto[5];
extern bool abilitazione;

extern int DatoRicevutoBLE[2];


class AbilitazioneCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() == 1) {
      if (value[0] == 0) {
        abilitazione = true;
        Serial.println("Abilitazione da BLE");
      } else if (value[0] == 1) {
        abilitazione = false;
        Serial.println("Disabilitazione da BLE");
      }
    }
  }
};

class ArrayIntCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    int numValues = value.size() / sizeof(int);
    static unsigned long lastReceiveTime = 0;
    unsigned long currentTime = millis();
    
    if (numValues == 2) {
      memcpy(DatoRicevutoBLE, value.data(), value.size());
      
      unsigned long intervallo = currentTime - lastReceiveTime;
      // Serial.print("Intervallo ricezione dati BLE: ");
      // Serial.print(intervallo);
      // Serial.println(" ms");
      
      lastReceiveTime = currentTime;
    } else {
      Serial.print("Lunghezza dati BLE non valida: ");
      Serial.println(value.size());
    }
  }
};

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    abilitazione = true;
    Serial.println("Connesso a BLE!");
  }
  
  void onDisconnect(BLEServer* pServer) {
    BLEDevice::startAdvertising();
    abilitazione = true;
    
    // Reset valori joystick al centro quando si disconnette
    DatoRicevutoBLE[0] = 50; // motore
    DatoRicevutoBLE[1] = 50; // sterzo
    Serial.println("Disconnesso");
  }
};

void setupAbilitazioneService() {
  BLEService *pServiceAbilitazione = pServer->createService(SERVICE_UUID_COMMAND);
  

  pAbilitazione = pServiceAbilitazione->createCharacteristic(
    CHARACTERISTIC_UUID_ABILITAZIONE,
    BLECharacteristic::PROPERTY_WRITE 
  );
  pAbilitazione->setCallbacks(new AbilitazioneCallback());
  pAbilitazione->setValue((uint8_t*)&abilitazione, 1);

  pArrayInt = pServiceAbilitazione->createCharacteristic(
    CHARACTERISTIC_UUID_ARRAY,
    BLECharacteristic::PROPERTY_WRITE
  );
  pArrayInt->setCallbacks(new ArrayIntCallback());

  pServiceAbilitazione->start();
  BLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID_COMMAND);
}

void setupBLE() {
  BLEDevice::init(NomeMacchinina);
  
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  
  // Servizio principale per dati di stato
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Caratteristica batteria (standard)
  pBatteria = pService->createCharacteristic(
    "2A19",
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pBatteria->addDescriptor(new BLE2902());
  uint8_t batteryLevel = 100;
  pBatteria->setValue(&batteryLevel, 1);

  // Caratteristica custom DatoRicevuto
  pDatoRicevuto = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pDatoRicevuto->addDescriptor(new BLE2902());
  pDatoRicevuto->setValue((uint8_t*)DatoRicevuto, sizeof(int) * 5);

  // Caratteristica nome dispositivo
  pNomeTelecomando = pService->createCharacteristic(
    "c0debeef-1234-5678-1234-abcdefabcdef",
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pNomeTelecomando->addDescriptor(new BLE2902());
  pNomeTelecomando->setValue("BOX_X");

  pService->start();
  
  // Configurazione advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  pAdvertising->start();

  // Avvia il servizio per comandi (joystick + abilitazione)
  setupAbilitazioneService();

}