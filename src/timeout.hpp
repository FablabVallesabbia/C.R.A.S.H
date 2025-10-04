
// Dichiarazioni esterne per timeout.h
extern unsigned long ultimaRicezione;
extern const unsigned long TIMEOUT_ESPNOW;
extern unsigned long TempoBuzzer;
extern unsigned long TimeOutBuzzer;
extern bool MemBuzzer;
extern bool connessioneAttiva;
extern int DatoRicevuto[5];
// ROSSO, VERDE, BLU, BUZZER, AttivaBuzzer sono #define - non servono extern

void timeout() {
    unsigned long tempoAttuale = millis();
    if (tempoAttuale - ultimaRicezione > TIMEOUT_ESPNOW) {
        digitalWrite(ROSSO, (tempoAttuale / 500) % 2); // Lampeggia ogni 500ms
        digitalWrite(BUZZER, ((tempoAttuale / 500) % 2) && (MemBuzzer) && AttivaBuzzer);
        if (tempoAttuale - TempoBuzzer > TimeOutBuzzer) {
        MemBuzzer = false;
        }
        
        if (connessioneAttiva) {//Timeout
        Serial.print("TIMEOUT ESP-NOW - Arresto");
        Serial.println(NomeMacchinina);
        DatoRicevuto[1]=9;
        DatoRicevuto[2]=512;
        DatoRicevuto[3]=512;
        
        digitalWrite(VERDE, LOW);
        digitalWrite(BLU, LOW);
        
        TempoBuzzer = tempoAttuale; // Inizia il timer del buzzer
        MemBuzzer = true; // Riattiva il buzzer quando inizia il timeout
        connessioneAttiva = false;
        }
    } else {
        if (!connessioneAttiva) {
        Serial.println("ESP-NOW riconnesso!");
        connessioneAttiva = true;
        digitalWrite(ROSSO, LOW); 
        digitalWrite(BUZZER, LOW); 
        MemBuzzer=true;
        }
    }
}