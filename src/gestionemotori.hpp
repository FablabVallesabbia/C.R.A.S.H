



void CommandRearMotor(int MotorState, int MotorSpeed) {
  int ValVelocita;
  #ifdef EscPosteriore
    if (MotorState == 1) { // avanti
      ValVelocita = map(MotorSpeed, 512, 1023, ValCentroMotore, ValMaxMotore);
      ESC.write(ValVelocita);
    } else if (MotorState == 2) { // indietro
      ValVelocita = map(MotorSpeed, 0, 512, ValMinMotore, ValCentroMotore);
      ESC.write(ValVelocita);
    } else if (MotorState == 9) { // STOP
      ESC.write(ValCentroMotore);
    }
  #endif

  #ifdef FakeL298Posteriore
    if (MotorState == 1) { // avanti
      ValVelocita = map(MotorSpeed, 512, 1023, ValCentroMotore, ValMaxMotore);
      analogWrite(In1, ValVelocita);
      analogWrite(In2, 0);
    } else if (MotorState == 2) { // indietro
      Serial.println("MotorSpeed: " + String(MotorSpeed));
      ValVelocita = map(MotorSpeed, 0, 512, ValMaxMotore, ValCentroMotore);
      Serial.println("ValVelocita: " + String(ValVelocita));
      analogWrite(In1, 0);
      analogWrite(In2, ValVelocita);
    } else if (MotorState == 9) { // STOP
      analogWrite(In1, 0);
      analogWrite(In2, 0);
    }
  #endif

  
  #ifdef L293dPosteriore
    if (MotorState == 1) { // avanti
      ValVelocita = map(MotorSpeed, 512, 1023, ValCentroMotore, ValMaxMotore);
      digitalWrite(In1, HIGH);
      digitalWrite(In2, LOW);
      analogWrite(Ena, ValVelocita);
    } else if (MotorState == 2) { // indietro
      ValVelocita = map(MotorSpeed, 0, 512, ValMaxMotore, ValCentroMotore);
      //Serial.println("ValVelocita: " + String(ValVelocita));
      digitalWrite(In1, LOW);
      digitalWrite(In2, HIGH);
      analogWrite(Ena, ValVelocita);
      //Serial.println("MotorSpeed: " + String(ValVelocita));
    } else if (MotorState == 9) { // STOP
      digitalWrite(In1, LOW);
      digitalWrite(In2, LOW);
      analogWrite(Ena, 255);
    }
  #endif
}


void SteeringCommand(int ValSteering) {
  #ifdef ServoAnteriore
    Val0=DatoRicevuto[4];
    if (ValSteering < 512){
      ValServo = map(ValSteering, 512, 0, Val0, ValSx);
    }
    else if (ValSteering > 512){
      ValServo = map(ValSteering, 512, 1023, Val0, ValDx);
    }
    else {
      ValServo = Val0;
    }
    Sterzo.write(ValServo);
    Serial.println("ValServo: " + String(ValServo));
  #endif

  #ifdef FakeL298Anteriore
    static unsigned long ultimoSteeringCommand = 0;
    static int ultimaDirezioneSterzo = 512; // Centro
    static bool boostAttivo = false;
    
    unsigned long tempoCorrente = millis();
    
    if (ValSteering < 512) { // sinistra
      // Se cambia direzione o è la prima attivazione, attiva il boost
      if (ultimaDirezioneSterzo >= 512) {
        boostAttivo = true;
        ultimoSteeringCommand = tempoCorrente;
        analogWrite(In3, 0);
        analogWrite(In4, 255); // Boost iniziale a 255 per TempoBoostSterzo ms
      } else {
        // Controlla se sono passati i millisecondi di boost configurati
        if (boostAttivo && (tempoCorrente - ultimoSteeringCommand >= TempoBoostSterzo)) {
          boostAttivo = false;
          analogWrite(In3, 0);
          analogWrite(In4, ValPwm); // Valore di mantenimento
        } else if (!boostAttivo) {
          // Mantieni il valore normale se il boost è già finito
          analogWrite(In3, 0);
          analogWrite(In4, ValPwm);
        }
      }
      ultimaDirezioneSterzo = ValSteering;
      
    } else if (ValSteering > 512) { // destra
      // Se cambia direzione o è la prima attivazione, attiva il boost
      if (ultimaDirezioneSterzo <= 512) {
        boostAttivo = true;
        ultimoSteeringCommand = tempoCorrente;
        analogWrite(In3, 255); // Boost iniziale a 255 per TempoBoostSterzo ms
        analogWrite(In4, 0);
      } else {
        // Controlla se sono passati i millisecondi di boost configurati
        if (boostAttivo && (tempoCorrente - ultimoSteeringCommand >= TempoBoostSterzo)) {
          boostAttivo = false;
          analogWrite(In3, ValPwm); // Valore di mantenimento
          analogWrite(In4, 0);
        } else if (!boostAttivo) {
          // Mantieni il valore normale se il boost è già finito
          analogWrite(In3, ValPwm);
          analogWrite(In4, 0);
        }
      }
      ultimaDirezioneSterzo = ValSteering;
      
    } else { // centro (512)
      analogWrite(In3, 0);
      analogWrite(In4, 0);
      ultimaDirezioneSterzo = 512;
      boostAttivo = false;
    }
  #endif
}






// Funzione parametrica per gestione cambio marcia
void CambioMarcia(uint8_t nuovaMarcia) {
  extern uint8_t ValMaxMotoreVEL[];
  extern uint8_t ConfigurazioniLED[][3];
  extern uint8_t ConfigurazioneBuzzer[];
  extern const int NUM_MARCE;
  extern uint8_t ValMaxMotore;
  
  // Verifica che la marcia sia valida
  if (nuovaMarcia < 1 || nuovaMarcia > NUM_MARCE) {
    return; // Marcia non valida
  }
  
  // Imposta la velocità massima per la marcia selezionata
  ValMaxMotore = ValMaxMotoreVEL[nuovaMarcia];
  
  // Imposta i LED secondo la configurazione
  digitalWrite(ROSSO, ConfigurazioniLED[nuovaMarcia][0] ? HIGH : LOW);
  digitalWrite(VERDE, ConfigurazioniLED[nuovaMarcia][1] ? HIGH : LOW);
  digitalWrite(BLU, ConfigurazioniLED[nuovaMarcia][2] ? HIGH : LOW);
  
  // Gestisce il buzzer secondo la configurazione
  for (int i = 0; i < ConfigurazioneBuzzer[nuovaMarcia]; i++) {
    digitalWrite(BUZZER, AttivaBuzzer);
    delay(50);
    digitalWrite(BUZZER, LOW);
    if (i < ConfigurazioneBuzzer[nuovaMarcia] - 1) {
      delay(50); // Pausa tra i beep (tranne dopo l'ultimo)
    }
  }
}