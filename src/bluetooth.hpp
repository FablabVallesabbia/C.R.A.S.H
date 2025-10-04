///////// - GESTIONE COMUNICAZIONE BLUETOOTH - ///////////

// Dichiarazioni esterne delle variabili dal main.cpp
extern BluetoothSerial SerialBT;
extern bool espNowAttivo;
extern int DatoRicevuto[4];
extern unsigned long ultimaRicezione;
// VERDE, BUZZER, AttivaBuzzer sono #define - non servono extern

// Dichiarazioni esterne delle funzioni dal main.cpp
extern void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

void GestioneBT() {
  if (SerialBT.hasClient() && espNowAttivo) {
    esp_now_deinit();
    espNowAttivo = false;
    for (int i = 0; i < 2; i++) {
      digitalWrite(VERDE, HIGH);
      digitalWrite(BUZZER, AttivaBuzzer);
      delay(200);
      digitalWrite(VERDE, LOW);
      digitalWrite(BUZZER, LOW);
      delay(200);
    }
  }
  
  // Se Bluetooth disconnesso, riaccendi ESP-NOW  
  if (!SerialBT.hasClient() && !espNowAttivo) {
    esp_now_init();
    esp_now_register_recv_cb(OnDataRecv);
    espNowAttivo = true;
  }

  if (SerialBT.available()) {
    char comando = SerialBT.read();

    ultimaRicezione = millis();

    // Serial.println("Comando Bluetooth ricevuto: " + String(comando));

    switch (comando) {
      case 'S': //stop
        DatoRicevuto[1] = 9; // STOP
        DatoRicevuto[2] = 512; //sterzo
        DatoRicevuto[3] = 512; // velocità motore
        break;
      
      case 'F': //avanti
        DatoRicevuto[1] = 1; // STOP
        DatoRicevuto[2] = 512; //sterzo
        DatoRicevuto[3] = 1024; // velocità motore
        break;

      case 'B': //indietro
        DatoRicevuto[1] = 2; // STOP
        DatoRicevuto[2] = 512; //sterzo
        DatoRicevuto[3] = 0; // velocità motore
        break;
      
      case 'R': //destra
        DatoRicevuto[1] = 9; // STOP
        DatoRicevuto[2] = 1024; //sterzo
        DatoRicevuto[3] = 512; // velocità motore
        break;

      case 'L': //sinistra
        DatoRicevuto[1] = 9; // STOP
        DatoRicevuto[2] = 0; //sterzo
        DatoRicevuto[3] = 512; // velocità motore
        break;

      case 'G': //avanti sinistra
        DatoRicevuto[1] = 1; // Avanti
        DatoRicevuto[2] = 0; //sterzo
        DatoRicevuto[3] = 1024; // velocità motore
        break;

      case 'I': //avanti destra
        DatoRicevuto[1] = 1; // Avanti
        DatoRicevuto[2] = 1024; //sterzo
        DatoRicevuto[3] = 1024; // velocità motore
        break;

      case 'H': //indietro sinistra
        DatoRicevuto[1] = 2; // Indietro
        DatoRicevuto[2] = 0; //sterzo
        DatoRicevuto[3] = 0; // velocità motore
        break;

      case 'J': //indietro destra
        DatoRicevuto[1] = 2; // Indietro
        DatoRicevuto[2] = 1024; //sterzo
        DatoRicevuto[3] = 0; // velocità motore
        break;

      case '0':
      case '1':
        DatoRicevuto[0]=1;
        break;
      case '2':
      case '3':
        DatoRicevuto[0]=2;
        break;
      case '4':
      case '5':
        DatoRicevuto[0]=3;
        break;
      case '6':
      case '7':
        DatoRicevuto[0]=4;
        break;
      case '8':
      case '9':
      case 'q':
        DatoRicevuto[0]=5;
        break;
      
      case 'V': //clacson on
        digitalWrite(BUZZER, HIGH);
        break;
      case 'v': //clacson off
        digitalWrite(BUZZER, LOW);
        break;
    }

  }
}