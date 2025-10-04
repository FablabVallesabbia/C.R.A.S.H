# C.R.A.S.H.

Firmware open source per il controllo di ogni tipo di macchinina RC.

## Descrizione

Questo è un progetto comunitario del FabLab Vallesabbia, consiste in un firmware customizzabile per auto RC, pienamente configurabile attraverso un file senza bisogno di scrivere codice. Utilizza un microcontrollore ESP32 per gestire la comunicazione wireless oltre al controllo i motori e servi e il monitoraggio della batteria. Supporta molteplici protocolli di comunicazione: ESP-NOW, Bluetooth Classic e Bluetooth Low Energy (BLE).
Il core del progetto consiste nella semplicità di programamzione: date un occhiata al codice e vi renderete conto che è estremamente semplice, in modo che chiunque possa creare telecomandi o device per il controllo.
Per maggiori informazioni: **LINK**

## Caratteristiche Principali
  Esempi di codice per comandare le macchinine:
  - Codice telecomando: **LINK**
  - Codice python per pc che utilizza gamepad o volanti logitech per il controllo: **LINK**

### Caratteristiche Principali

- **Comunicazione Wireless**:
  - ESP-NOW per comunicazione con telecomandi autocostruiti o modificati.
  - Bluetooth Classic per controllo tramite smartphone.
  - Bluetooth Low Energy (BLE) per controllo tramite smartphone.
  Il bluetooth classic/BLE sono utilizzabili a seconda della serie del microcontrollore ESP32 che sceglierete (ES: esp wroom ha solo il bluetooth classic, mentre esp32-C3 ha solo il BLE)

- **Controllo Motori**:
Il controllo dei motori è customizzabile a seconda della configurazione della vostra macchinina, ed è molto semplice aggiungere altri driver, vedere il file gestionemotori.hpp e la wiki per ulteriori dettagli.
I motori/driver attualmente supportarti sono:
  - L293D
  - Amazon Fake L293 (breakout board rossa con chip smd)
  - ESC per motori brushless
  - Servo per sterzo anteriore

- **Monitoraggio Batteria**:
  - Lettura tensione batteria customizzabile sia in precisione che in tensione.

- **Sicurezza e Feedback**:
  - Timeout per perdita connessione.
  - Buzzer per feedback sonoro.
  - LED RGB per indicazione stati (rosso, verde, blu).

- **Configurabilità**:
Abilitazione/disabilitazione moduli tramite il metodo define. questo permette di adattare il firmware alla propria macchinina senza dover modificare nessuna aprte del programma.

## Requisiti Hardware

- **Microcontrollore**: ESP32.
- **Motori e Servi**: Servo o motore DC per sterzo, motore brushless o DC per motore posteriore.
- **Alimentazione**: qualunque batteria va bene, la sezione di lettura batteria è configurabile inserendo i parametri della propria batteria.

## Installazione

1. **Installa PlatformIO**:
   - Scarica e installa Visual Studio Code.
   - Installa l'estensione PlatformIO.

2. **Clona il Repository**:
   ```bash
   git clone https://github.com/MiticoDan/MK_X-codice-test-1.git
   cd MK_X-codice-test-1
   ```

3. **Configura la macchinina**:
   - Apri il progetto in VS Code con PlatformIO.
   - Modifica il file config.hpp con le impostazioni desiderate

4. **Carica il Firmware**:
   - Collega l'ESP32 al computer.


## Wiki
  Si prega di leggere la wiki per guide e informazioni più dettagliate.

## TODO
- [ ] Aggiungere supporto per più tipi di driver motori (es. TB6612FNG).
- [ ] Migliorare l'interfaccia BLE.
- [ ] Creare WebServer per configurare i parametri senza dover ricompilare 
- [ ] Creare Configuratore e caricatore grafico 



## Contributi

Contributi sono benvenuti! Apri un issue o una pull request su GitHub.

## Licenza

Questo progetto è distribuito sotto licenza MIT. Vedi il file `LICENSE` per dettagli.

## Contatti
- **Autore**: FabLabValleSabbia
- **INFO**: (https://www.fablabvallesabbia.it/)
- **Repository**: [GitHub](https://github.com/MiticoDan/MK_X-codice-test-1)</content>
<parameter name="filePath">c:\Users\Daniel\Desktop\Codice MK X\MK_X codice test 1\README.md