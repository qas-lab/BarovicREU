# Natural Language Processing with TinyML
#### Contributors: Andrew Barovic
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
- Purchase [Arduino Nano 33 BLE Sense Rev 2](https://store-usa.arduino.cc/products/nano-33-ble-sense-rev2) Chip
  - [Arduino Nano 33 BLE Sense](https://store-usa.arduino.cc/products/arduino-nano-33-ble-sense) will also work
- [Hardware Schematic](https://docs.arduino.cc/resources/schematics/ABX00069-schematics.pdf)
- [Datasheet](https://docs.arduino.cc/resources/datasheets/ABX00069-datasheet.pdf)
- [Pinout](https://docs.arduino.cc/resources/pinouts/ABX00069-full-pinout.pdf)
------------
## Using the Model
- Final prototype located in Arduino Final Model (1-Chip)
- Download the .zip file
  - This .zip file is a library folder for the Arduino compiler (do not unzip)
  - Add this library through the Arduino IDE via:
    - Sketch > Include Library > [Add .ZIP File...]
- Download the 'modified_color_command_keyword_microphone_continuous' file
  - Open the .ino file with the Arduino IDE (Compiler)
    - Attach Arduino Nano 33 BLE sense chip to the computer (make sure Arduino recognizes the chip & port)
    - Compile, Verify, and Flash the code to the chip
      - compiling the code will take a few minutes the first time it is done
    - Open the serial monitor
      - Predictions should now be displayed
  - 'Wake up' turns on the listening mode of the device, 'LED' executes the currently heard commands
    - Try: 'Wake up', 'On', 'Red', 'LED'
      - Wake up should turn on the yellow LED on the chip (this is listening mode)
      - 'On', 'Red', and 'LED' should make the RGB LED red (may require a few tries in order to function properly. Watch the serial monitor to see what is being heard by the microchip)
  - If any errors occur check the library files imported from Arduino.
## Using the Dataset

----------
## Project Structure
```.
C:.
├───Arduino Basic Models
├───Arduino Basic Testing
│   ├───Command Keyword List
│   ├───Edge Impulse Modified Files
│   │   ├───(ignore version, COM5)
│   │   └───(ignore version, COM6)
│   ├───Test 1 (Bad Microphone Data, 2 colors)
│   │   └───modifed-colorkeyword-microphone
│   ├───Test 2 (Chip Microphone Data, 1 color)
│   │   └───modified_color_keyword_2
│   ├───Test 3 (Full LED Test, NO ML)
│   │   └───LEDtest
│   ├───Test 4 (Two-way Bluetooth, NO ML)
│   │   ├───bluetoothATest
│   │   └───bluetoothBTest
│   ├───Test 5 (Initial Prototype, 3 colors)
│   │   └───modified_color_keyword_3
│   ├───Test 6 (Color Prototype, 8 colors)
│   │   └───modified_color_keyword_4
│   ├───Test 7 (Command Prototype, 12 commands)
│   ├───Test 7 (Sleep Mode Testing, NO ML)
│   │   └───sleep_test
│   └───Test 8 (Color Prototype 2, 8 Colors)
│       └───modified_color_keyword_4
├───Arduino Final Model (1-Chip)
│   └───modified_color_command_keyword_microphone_continuous
├───Arduino Final Model (2-Chip)
├───Datasets
│   ├───Colors
│   │   ├───Json (Edge Impulse Format)
│   │   └───Wav (Edge Impulse Formatting)
│   └───Commands
│       ├───Json (Edge Impulse Format)
│       └───Wav (Edge Impulse Formatting)
├───Edge Impulse Pipeline (Concept)
├───External Pipeline Example
└───Keywords List
```
