#include <Arduino.h>

#include <ESP32Servo.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

int DatoRicevuto[5]={0,9,512,512,90}; //inizializzo i dati ricevuti da ESP-NOW
int DatoRicevutoBLE[2]={50,50}; //inizializzo i dati ricevuti da BLE
uint8_t ValVelocita;

uint8_t OldMarcia=1;
bool MemBuzzer=true;

unsigned long TimeOutBuzzer = 2000; // Tempo di suono del buzzero quando perdo la connessione

// Variabili per timeout ESP-NOW
unsigned long ultimaRicezione = 0;
unsigned long TempoBuzzer=0;
bool connessioneAttiva = false;
bool espNowAttivo = true;

// Variabili per il tempo di ciclo
unsigned long startTime = 0;
unsigned long TempoCiclo = 0;

bool DatiRicevuti = false;

uint8_t macTelecomando[6];
bool telecomandoRegistrato = false;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void taskFunction(void * parameter);

bool abilitazione=true;

#include "config.hpp"
#include "timeout.hpp"
#ifdef AbilitaBLE
  #include <BLE.hpp>
#endif

#ifdef EscPosteriore
  Servo ESC;
#endif
#ifdef ServoAnteriore
  Servo Sterzo;
#endif

#ifdef AbilitaBluetooth
  #include "BluetoothSerial.h"
  BluetoothSerial SerialBT;
  #include "bluetooth.hpp"
#endif

#include "gestionemotori.hpp"

typedef struct {
  int percentualeBatteria;
  uint8_t marciaAttuale;
  uint8_t marciaMassima;
  char nomeMacchinina[16];
} DatiRisposta_t;
DatiRisposta_t datiRisposta = {0, 1, NUM_MARCE, NomeMacchinina}; // String letterale

void setup() {
  pinMode(ROSSO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  #ifdef ServoAnteriore
    Serial.println("Inizializzazione servo anteriore...");
    Sterzo.attach(STERZO_PIN);
    Sterzo.write(90);
  #endif

  #ifdef LetturaBatteria
    pinMode(BatteriaPin, INPUT);
  #endif

  #ifdef FakeL298Anteriore
    pinMode(In3, OUTPUT);
    pinMode(In4, OUTPUT);
  #endif
  #ifdef FakeL298Posteriore
    pinMode(In1, OUTPUT);
    pinMode(In2, OUTPUT);
  #endif
    #ifdef L293dPosteriore
    pinMode(In1, OUTPUT);
    pinMode(In2, OUTPUT);
    pinMode(Ena, OUTPUT);
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);
    digitalWrite(Ena, LOW);
  #endif

  digitalWrite(ROSSO,HIGH);
  delay(2000);
  Serial.begin(115200);
  Serial.println("starting...");
  digitalWrite(ROSSO,LOW);
  
  // Inizializza Bluetooth
  #ifdef AbilitaBluetooth
    SerialBT.begin(NomeMacchinina);
    Serial.print("Bluetooth inizializzato - Dispositivo: ");
    Serial.println(NomeMacchinina);
  #endif
  #ifdef AbilitaBLE
    setupBLE();
    Serial.println("Bluetooth LE inizializzato");
  #endif
  
  // Inizializza WiFi in modalità Station
  WiFi.mode(WIFI_STA);

  uint8_t newMACAddress[] = {0x6c, 0xc8, 0x40, 0x34, 0xca, 0x30};
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  // Stampa il MAC address
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  WiFi.setTxPower(WIFI_POWER_11dBm);
  
  // Inizializza ESP-NOW
  digitalWrite(VERDE,HIGH);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Errore nell'inizializzazione di ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  delay(100);
  Serial.println("ESP-NOW inizializzato correttamente");
  digitalWrite(VERDE,LOW);
  
  digitalWrite(BLU,HIGH);
  #ifdef EscPosteriore
    Serial.println("press brake to arm the ESC!");
    delay(2000);
    ESC.attach(ESC_PIN,  1000, 2000);
    ESC.write(90);
  #endif

  digitalWrite(BLU,LOW);

  for (int i = 0; i < 5; i++) {
    digitalWrite(BUZZER, AttivaBuzzer);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
  // Serial.println("MK_X operativa");
  
  // Inizializza il timestamp per il timeout ESP-NOW
  ultimaRicezione = millis();
  
  // Crea il task che esegue ogni 50ms
  xTaskCreate(taskFunction, "TaskRisposta", 2048, NULL, 1, NULL);
}

void loop() {
  startTime = millis();
  
  // Gestione comandi Bluetooth
  #ifdef AbilitaBluetooth
    GestioneBT();
  #endif

  #ifdef LetturaBatteria
    // Leggi la tensione della batteria solo se non si sta muovendo
    if(DatoRicevuto[2] == 512 && DatoRicevuto[3] == 512) {
      int letturaBatteria = analogRead(BatteriaPin);

      float tensioneBatteria = (letturaBatteria / 4095.0) * 3.3 * FattoreConversioneTensione;
      SommaTensioni+= tensioneBatteria;
      
      ValoriRaccolti++;
      if (ValoriRaccolti >= ValoriMedia) {
        // Calcola la media
        float TensioneMediaBatteria = SommaTensioni / ValoriRaccolti;
        //Serial.println(TensioneMediaBatteria);
        ValoriRaccolti = 0;
        SommaTensioni=0;

        // Calcolo percentuale
        PercentualeBatteria = (int)((TensioneMediaBatteria - TensioneBatteriaScarica) / 
                                        (TensioneBatteriaCarica - TensioneBatteriaScarica) * 100.0);
        
        // Limita la percentuale tra 0 e 100
        PercentualeBatteria = constrain(PercentualeBatteria, 0, 100);

        datiRisposta.percentualeBatteria = PercentualeBatteria;

        // Serial.print("Batteria: ");
        // Serial.print(PercentualeBatteria);
        // Serial.println("%");
      }
    }
  #endif

  ////-AGGIORNAMENTO BLE-/////
  #ifdef AbilitaBLE
  static unsigned long lastBLEUpdate = 0;

  if (millis() - lastBLEUpdate >= UpdateBLETime) {
    lastBLEUpdate = millis();
    //Serial.println("Aggiornamento BLE: " + String(PercentualeBatteria) + "%");
    uint8_t batteryLevel = (uint8_t)PercentualeBatteria;
    pBatteria->setValue(&batteryLevel, 1);
    pBatteria->notify();
    pDatoRicevuto->setValue((uint8_t*)DatoRicevuto, sizeof(int) * 5);
    pDatoRicevuto->notify();
    pNomeTelecomando->setValue("MK_BOX");
    pNomeTelecomando->notify();

    //Serial.println(abilitazione);
    
  }
  if (abilitazione == false) {
      DatoRicevuto[3] = map(DatoRicevutoBLE[0], 0, 100, 0, 1023);//motore
      DatoRicevuto[2] = map(DatoRicevutoBLE[1], 0, 100, 0, 1023);//sterzo
      DatoRicevuto[0] = (sizeof(ValMaxMotoreVEL)/sizeof(ValMaxMotoreVEL[0])) - 1;


      ///zone morte BLE///
      if (DatoRicevuto[3] > 490 && DatoRicevuto[3] < 534) { // Evita che il motore vada in avanti o indietro se il valore è vicino a 512
        DatoRicevuto[3] = 512;
      }
      if (DatoRicevuto[2] > 490 && DatoRicevuto[2] < 534) { // Evita che il sterzo vada in avanti o indietro se il valore è vicino a 512
        DatoRicevuto[2] = 512;
      }
      /////////////////////

      if (DatoRicevuto[3] > 512) DatoRicevuto[1] = 1;
      else if (DatoRicevuto[3] < 512) DatoRicevuto[1] = 2;
      else DatoRicevuto[1] = 9;
      Serial.println(DatoRicevuto[3]);

    }
    #endif
  ////////////////////////////

  ComandoMotorePosteriore(DatoRicevuto[1], DatoRicevuto[3]);
  SteeringCommand(DatoRicevuto[2]);


  if (DatoRicevuto[0] != OldMarcia) {
    OldMarcia = DatoRicevuto[0];
    CambioMarcia(DatoRicevuto[0]); 
  }

  

  ////////////////////////////-TIMEOUT-/////////////////////////////
  if (AbilitaTimeout == true and abilitazione) timeout();
  //////////////////////////////////////////////////////////////////

  TempoCiclo = millis() - startTime;
  if (TempoCiclo < 10) { //delay di minimo 10ms
    delay(10 - TempoCiclo);
  }
  TempoCiclo = millis() - startTime;
}




void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len == 5 * sizeof(int)) {
    memcpy(DatoRicevuto, incomingData, 5 * sizeof(int));
    ultimaRicezione = millis();
    // Serial.print("Ricevuti: ");
    // for (int i = 0; i < 4; i++) {
    //   Serial.print(DatoRicevuto[i]);
    //   Serial.print(" ");
    // }
    // Serial.println();

    // Registra il telecomando se non è già fatto
    if (!telecomandoRegistrato) {
      memcpy(macTelecomando, mac, 6);
      esp_now_peer_info_t peerInfo = {};
      memcpy(peerInfo.peer_addr, mac, 6);
      peerInfo.channel = 1;
      peerInfo.encrypt = false;
      //esp_now_add_peer(&peerInfo);
      //telecomandoRegistrato = true;


    for (int i = 0; i < 6; i++) {
      if (mac[i] < 16) Serial.print("0");
      Serial.print(mac[i], HEX);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
    

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Errore aggiunta peer");
    }
    else {
      Serial.println("Telecomando registrato con successo");
      telecomandoRegistrato = true;
    }
  }

    DatiRicevuti = true;
  } else {
    Serial.println("Lunghezza dati non valida!");
  }
}

///////////-TASK PER INVIARE DATI AL TELECOMANDO-//////////////////
void taskFunction(void * parameter) {
  for(;;) {
    // Debug: sempre attivo per vedere se il task gira
    // static unsigned long debugCounter = 0;
    // debugCounter++;
    // if (debugCounter % 100 == 0) {  // Ogni 10 secondi (100 cicli * 100ms)
    //   Serial.print("Task attivo - Counter: ");
    //   Serial.print(debugCounter);
    //   Serial.print(" | telecomandoRegistrato: ");
    //   Serial.print(telecomandoRegistrato ? "SI" : "NO");
    //   Serial.print(" | DatiRicevuti: ");
    //   Serial.println(DatiRicevuti ? "SI" : "NO");
    // }
    
    if (telecomandoRegistrato && DatiRicevuti) {
      datiRisposta.marciaAttuale = OldMarcia;
      
      // Debug prima dell'invio
      Serial.println("→ Tentativo invio dati...");
      
      esp_err_t result = esp_now_send(macTelecomando, (uint8_t *) &datiRisposta, sizeof(datiRisposta));
      
      if (result == ESP_OK) {
        Serial.println("Dati inviati al telecomando");
      } else {
        Serial.print("Errore invio: ");
        Serial.println(result);
      }
      
      DatiRicevuti = false;
      
      vTaskDelay(90 / portTICK_PERIOD_MS);
    } else {
      // Delay più corto quando non invia
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}
//////////////////////////////////////////////////////////////////