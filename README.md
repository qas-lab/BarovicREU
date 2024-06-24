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
    ├───Test 1 (Bad Microphone Data, 2 colors)
    │   └───modifed-colorkeyword-microphone
    ├───Test 2 (Chip Microphone Data, 1 color)
    │   └───modified_color_keyword_2
    ├───Test 3 (Full LED Test, NO ML)
    │   └───LEDtest
    ├───Test 4 (Two-way Bluetooth, NO ML)
    │   ├───bluetoothATest
    │   └───bluetoothBTest
    └───Test 5 (Initial Prototype, 3 colors)
        └───modified_color_keyword_3
```
