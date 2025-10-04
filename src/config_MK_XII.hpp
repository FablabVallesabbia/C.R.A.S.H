// questo file contiene al configurazione della macchinina
// ad esempio, il tipo di driver per il motore posteriore, il tipo di controllo per lo sterzo
// i vari pin etc.

//GIAN

#define AttivaBuzzer true //Attiva il buzzer
#define AbilitaTimeout true //Attiva il timeout per la perdita di connessione
//#define AbilitaBluetooth //Attiva la connessione bluetooth
#define AbilitaBLE //Attiva la connessione bluetooth BLE
//#define EscPosteriore // Usa un esc per il motore posteriore
#define ServoAnteriore // Usa un servo per lo sterzo anteriore
#define FakeL298Posteriore // Usa un driver L298 fake per il motore posteriore
//#define FakeL298Anteriore // Usa un driver L298 fake per il motore anteriore
//#define L293dPosteriore // Usa un driver L293D per il motore posteriore
#define LetturaBatteria //abilita la lettura della batteria

////////////-PINOUT-///////////
#define ROSSO 8
#define VERDE 5
#define BLU 11
#define BUZZER 10
#ifdef EscPosteriore
    #define ESC_PIN 23
    uint8_t ValMinMotore=0;
#endif
#ifdef ServoAnteriore
    #define STERZO_PIN 21
#endif
#ifdef FakeL298Posteriore
    #define In1 4
    #define In2 3
#endif
#ifdef L293dPosteriore
    #define In1 2
    #define In2 3
    #define Ena 4
#endif
#ifdef FakeL298Anteriore
    #define In3 7
    #define In4 6
#endif
//////////////////////////////


/////////-PARAMETRI-////////////
#define NomeMacchinina "MK_XII"

#ifdef ServoAnteriore
    uint8_t ValSx=170; //valore servo per sterzo a sinistra
    uint8_t ValDx=5;  //valore servo per sterzo a destra
    uint8_t Val0=90;   //valore servo per sterzo al centro
    uint8_t ValServo=Val0; //valore sterzata servo
#endif

#ifdef FakeL298Anteriore
    uint8_t ValPwm=180; //valore del pwm per il motore anteriore
    int TempoBoostSterzo=400; //tempo di boost iniziale sterzo in millisecondi
#endif
uint8_t ValCentroMotore=90; //motore fermo

#ifdef LetturaBatteria
    #define BatteriaPin 0 //pin di lettura della batteria
    #define TensioneBatteriaScarica 3.7
    #define TensioneBatteriaCarica 4.2
    #define ValoriMedia 10
    uint8_t ValoriRaccolti = 0; //contatore dei valori raccolti
    float SommaTensioni = 0.0;
    #define FattoreConversioneTensione 1.84 //Fattore di conversione per la tensione della batteria
    // Variabili per l'invio della percentuale batteria
    int PercentualeBatteria = 0;
#endif




uint8_t ValMaxMotoreVEL[] = {0, 240,255}; // Indice 0 non usato
uint8_t ConfigurazioniLED[][3] = {
  {0, 0, 0},     // Marcia 0 (non usata)
  {0, 0, 1},     // Marcia 1: BLU
  {1, 0, 0}      // Marcia 5: ROSSO
};
uint8_t ConfigurazioneBuzzer[] = {0, 1, 2};

const int NUM_MARCE = sizeof(ValMaxMotoreVEL) / sizeof(ValMaxMotoreVEL[0]) - 1; // -1 perché indice 0 non usato

// Controllo che i 3 array abbiano la stessa lunghezza
static_assert(sizeof(ValMaxMotoreVEL) / sizeof(ValMaxMotoreVEL[0]) == 
              sizeof(ConfigurazioneBuzzer) / sizeof(ConfigurazioneBuzzer[0]), 
              "ERRORE: ValMaxMotoreVEL e ConfigurazioneBuzzer devono avere la stessa lunghezza!");

static_assert(sizeof(ValMaxMotoreVEL) / sizeof(ValMaxMotoreVEL[0]) == 
              sizeof(ConfigurazioniLED) / sizeof(ConfigurazioniLED[0]), 
              "ERRORE: ValMaxMotoreVEL e ConfigurazioniLED devono avere la stessa lunghezza!");

const unsigned long TIMEOUT_ESPNOW = 200; // 200ms
uint8_t ValMaxMotore=ValMaxMotoreVEL[1]; // Inizializza con velocità 1
///////////////////////////////