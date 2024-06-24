# Natural Language Processing with TinyML
#### Contributors: Andrew Barovic & Armin Moin
------------
## Getting Started
```bash
git clone https://github.com/qas-lab/BarovicREU.git
```
## Arduino
- Download [Arduino IDE](https://www.arduino.cc/en/software)
  - Version 2.3.2
- Download [Arduino CLI](https://arduino.github.io/arduino-cli/1.0/installation/)
  - Version 1.0
## Edge Impulse
- Sign Into [Edge Impulse](https://edgeimpulse.com/)
- Download [Edge Impulse CLI](https://docs.edgeimpulse.com/docs/tools/edge-impulse-cli/cli-installation)
## Node.js
- Download [Node.js](https://nodejs.org/en/)
## Arduino Nano 33 BLE Sense
- Purchase two [Arduino Nano 33 BLE Sense Rev 2](https://store-usa.arduino.cc/products/nano-33-ble-sense-rev2) Chips
  - [Arduino Nano 33 BLE Sense](https://store-usa.arduino.cc/products/arduino-nano-33-ble-sense) will also work
- [Hardware Schematic](https://docs.arduino.cc/resources/schematics/ABX00069-schematics.pdf)
- [Datasheet](https://docs.arduino.cc/resources/datasheets/ABX00069-datasheet.pdf)
- [Pinout](https://docs.arduino.cc/resources/pinouts/ABX00069-full-pinout.pdf)

----------
## Project Structure
```.
Arduino Basic Testing
    ├───Edge Impulse Modified Files (ignore version, COM 5)
    │       flash_windows.bat
    │
    ├───Test 1 (Bad Microphone Data, 2 colors)
    │   │   ei-colorkeyword-arduino-1.0.9.zip
    │   │   READ-FILE_USE_EXPLAINED
    │   │
    │   └───modifed-colorkeyword-microphone
    │           modifed-colorkeyword-microphone.ino
    │
    ├───Test 2 (Chip Microphone Data, 1 color)
    │   │   ei-colorkeyword-arduino-1.0.11.zip
    │   │   READ-FILE_USE_EXPLAINED
    │   │
    │   └───modified_color_keyword_2
    │           modified_color_keyword_2.ino
    │
    ├───Test 3 (Full LED Test, NO ML)
    │   └───LEDtest
    │           LEDtest.ino
    │
    ├───Test 4 (Two-way Bluetooth, NO ML)
    │   ├───bluetoothATest
    │   │       bluetoothATest.ino
    │   │
    │   └───bluetoothBTest
    │           bluetoothBTest.ino
    │
    └───Test 5 (Initial Prototype, 3 colors)
        │   ei-colorkeyword-arduino-1.0.12.zip
        │   READ-FILE_USE_EXPLAINED
        │
        └───modified_color_keyword_3
                modified_color_keyword_3.ino
```
