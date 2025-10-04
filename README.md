# C.R.A.S.H.

Open source firmware for controlling any type of RC car.

## Description

This is a community project of FabLab Vallesabbia, it consists of a customizable firmware for RC cars, fully configurable through a file without needing to write code. It uses an ESP32 microcontroller to manage wireless communication in addition to controlling motors and servos and battery monitoring. It supports multiple communication protocols: ESP-NOW, Bluetooth Classic and Bluetooth Low Energy (BLE).
The core of the project consists in the simplicity of programming: take a look at the code and you will realize that it is extremely simple, so that anyone can create remotes or devices for control.
For more information: **LINK**

## Main Features
  Examples of code to control the cars:
  - Remote code: **LINK**
  - Python code for pc that uses gamepad or logitech wheels for control: **LINK**

### Main Features

- **Wireless Communication**:
  - ESP-NOW for communication with self-built or modified remotes.
  - Bluetooth Classic for control via smartphone.
  - Bluetooth Low Energy (BLE) for control via smartphone.
  Bluetooth classic/BLE are usable depending on the series of the ESP32 microcontroller you choose (EG: esp wroom has only bluetooth classic, while esp32-C3 has only BLE)

- **Motor Control**:
Motor control is customizable depending on your car's configuration, and it is very simple to add other drivers, see the gestionemotori.hpp file and the wiki for more details.
The currently supported motors/drivers are:
  - L293D
  - Amazon Fake L293 (red breakout board with smd chip)
  - ESC for brushless motors
  - Servo for front steering

- **Battery Monitoring**:
  - Customizable battery voltage reading both in precision and voltage.

- **Safety and Feedback**:
  - Timeout for connection loss.
  - Buzzer for sound feedback.
  - RGB LED for status indication (red, green, blue).

- **Configurability**:
Enable/disable modules via the define method. this allows adapting the firmware to your own car without having to modify any part of the program.

## Hardware Requirements

- **Microcontroller**: ESP32.
- **Motors and Servos**: Servo or DC motor for steering, brushless or DC motor for rear motor.
- **Power Supply**: any battery is fine, the battery reading section is configurable by entering the parameters of your own battery.

## Installation

1. **Install PlatformIO**:
   - Download and install Visual Studio Code.
   - Install the PlatformIO extension.

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/MiticoDan/MK_X-codice-test-1.git
   cd MK_X-codice-test-1
   ```

3. **Configure the car**:
   - Open the project in VS Code with PlatformIO.
   - Modify the config.hpp file with the desired settings

4. **Upload the Firmware**:
   - Connect the ESP32 to the computer.


## Wiki
  Please read the wiki for more detailed guides and information.

## TODO
- [ ] Add support for more types of motor drivers (e.g. TB6612FNG).
- [ ] Improve the BLE interface.
- [ ] Create WebServer to configure parameters without having to recompile
- [ ] Create Graphical Configurator and uploader



## Contributions

Contributions are welcome! Open an issue or a pull request on GitHub.

## License

This project is distributed under the MIT license. See the `LICENSE` file for details.

## Contacts
- **Author**: FabLabValleSabbia
- **INFO**: (https://www.fablabvallesabbia.it/)
- **Repository**: [GitHub](https://github.com/MiticoDan/MK_X-codice-test-1)</content>
<parameter name="filePath">c:\Users\Daniel\Desktop\Codice MK X\MK_X codice test 1\README.md
