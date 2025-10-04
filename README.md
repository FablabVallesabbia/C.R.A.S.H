# C.R.A.S.H.

Open-source firmware for controlling any type of RC car.

## Description

This is a community project by FabLab Vallesabbia. It consists of customizable firmware for RC cars, fully configurable through a file without the need to write code. It uses an ESP32 microcontroller to manage wireless communication as well as motor and servo control and battery monitoring. It supports multiple communication protocols: ESP-NOW, Bluetooth Classic, and Bluetooth Low Energy (BLE).  
The core of the project is its programming simplicity: take a look at the code and you will realize it is extremely simple, so that anyone can create remotes or control devices.  
For more information: **LINK**

## Main Features

Examples of code to control RC cars:
- Remote control code: **LINK**
- Python code for PC using gamepads or Logitech wheels for control: **LINK**

### Main Features

- **Wireless Communication**:
  - ESP-NOW for communication with custom-built or modified remotes.
  - Bluetooth Classic for control via smartphone.
  - Bluetooth Low Energy (BLE) for control via smartphone.  
  Bluetooth Classic/BLE can be used depending on the ESP32 microcontroller series you choose (e.g.: ESP-WROOM only has Bluetooth Classic, while ESP32-C3 only has BLE).

- **Motor Control**:  
  Motor control is customizable depending on your RC car's configuration, and it is very easy to add other drivers. See the file gestionemotori.hpp and the wiki for further details.  
  The currently supported motors/drivers are:
  - L293D
  - Amazon Fake L293 (red breakout board with SMD chip)
  - ESC for brushless motors
  - Servo for front steering

- **Battery Monitoring**:
  - Battery voltage reading customizable in both precision and voltage.

- **Safety and Feedback**:
  - Timeout for connection loss.
  - Buzzer for sound feedback.
  - RGB LED to indicate states (red, green, blue).

- **Configurability**:  
  Enabling/disabling modules via define method. This allows adapting the firmware to your RC car without needing to modify any part of the program.

## Hardware Requirements

- **Microcontroller**: ESP32.
- **Motors and Servos**: Servo or DC motor for steering, brushless or DC motor for rear drive.
- **Power Supply**: any battery is fine, the battery reading section is configurable by inserting your battery parameters.

## Installation

1. **Install PlatformIO**:
   - Download and install Visual Studio Code.
   - Install the PlatformIO extension.

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/MiticoDan/MK_X-codice-test-1.git
   cd MK_X-codice-test-1
