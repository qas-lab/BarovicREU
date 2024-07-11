/*
  BLE_Central_Device.ino
*/
#include <ArduinoBLE.h>

// Define the pin numbers for the RGB LED
const int ledRedPin = 22;
const int ledGreenPin = 24;
const int ledBluePin = 23;

// UUIDs for the service and characteristic we want to interact with
const char* searchDeviceServiceUuid = "12345678-1234-1234-1234-1234567890AB";//"19b10000-e8f2-537e-4f6c-d104768a1214";
const char* searchDeviceServiceCharacteristicUuid = "12345678-1234-1234-1234-1234567890AC";//"19b10001-e8f2-537e-4f6c-d104768a1214";

//UUIDs
const char* localDeviceServiceUuid = "12345678-1234-1234-1234-1234567890AD";
const char* localDeviceServiceCharacteristicUuid = "12345678-1234-1234-1234-1234567890AE";

// Variables to store the current and previous LED values
int ledRead = 4;
int ledWrite = 4;
int oldLedValue = 4;

BLEService ledService(localDeviceServiceUuid);
BLEByteCharacteristic ledReadingCharactaristic(localDeviceServiceCharacteristicUuid, BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);       // Start the serial communication
  while (!Serial);          // Wait for the serial port to connect
  
  // Set the RGB LED pins as outputs:
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  // Turn off all colors to start with:
  analogWrite(ledRedPin, 255);
  analogWrite(ledGreenPin, 255);
  analogWrite(ledBluePin, 255);
  
  // Initialize the BLE module
  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);              // Halt the program if BLE initialization fails
  }
  
  BLE.setLocalName("Arduino Nano 33 BLE B"); // Set the local name for the BLE device
  BLE.setAdvertisedService(ledService); // Advertise the service
  ledService.addCharacteristic(ledReadingCharactaristic); // Add the characteristic to the service
  BLE.addService(ledService); // Add the service
  ledReadingCharactaristic.writeValue(ledRead); // Initialize the characteristic value
  BLE.advertise(); // Start advertising the BLE device

  Serial.println("Nano 33 BLE B");
  Serial.println(" ");
}

void loop() {
  connectToCentral();

  connectToPeripheral();
}

void connectToCentral()
{
  Serial.println("*******Critical Error Drop-out************");
  BLEDevice central = BLE.central(); // Wait for a central device to connect
  
  Serial.println("- Discovering central device...");
  
  if (central) { // If a central device is found
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    // Get the characteristic we want to read from
    BLECharacteristic ledWritingCharactaristic = central.characteristic(searchDeviceServiceCharacteristicUuid);
    // While the central device is connected
    while (central.connected()) {
      // Check if the characteristic value has been written by the central device
      if (ledReadingCharactaristic.written()) {
        ledRead = ledReadingCharactaristic.value(); // Get the new LED value
        Serial.print("* Received value from central: ");
        Serial.println(ledRead);
        switchLED(ledRead); // Update LED based on received value
        return;
      }

      // Read user input from Serial Monitor to send to central
      if (Serial.available() > 0) {
        int locCount = 0;
        ledRead = Serial.parseInt();
        if (ledRead >= 1 && ledRead <= 8) {
          while(locCount < 10)
          {
            ledReadingCharactaristic.writeValue((byte)ledRead); // Send value to central
            Serial.print("* Sent value to central: ");
            Serial.println(ledRead);
            locCount++;
          }
          return;
        } else {
          Serial.println("Invalid numbersssss! Enter a number between 1 and 8.");
        }
      }
    }
    central.disconnect();
    Serial.println("* Disconnected from central device!");
  }
}

void connectToPeripheral() {
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  // Scan for peripherals with the specified service UUID
  do {
    BLE.scanForUuid(searchDeviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);    // Keep scanning until a peripheral is found
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();         // Stop scanning once the peripheral is found
    controlPeripheral(peripheral); // Control the found peripheral
    peripheral.disconnect();
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  // Attempt to connect to the peripheral
  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return; // Exit the function if connection fails
  }

  Serial.println("- Discovering peripheral device attributes...");
  // Attempt to discover the peripheral's attributes
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect(); // Disconnect if attribute discovery fails
    return;
  }

  // Get the characteristic we want to read from
  BLECharacteristic ledWritingCharactaristic = peripheral.characteristic(searchDeviceServiceCharacteristicUuid);
  
  // Check if the characteristic is found and writable
  if (!ledWritingCharactaristic) {
    Serial.println("* Peripheral device does not have led_type characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledWritingCharactaristic.canWrite()) {
    Serial.println("* Peripheral does not have a writable led_type characteristic!");
    peripheral.disconnect();
    return;
  }
  ledWritingCharactaristic.writeValue((byte)ledWrite);
  // Continuously check for gestures while connected to the peripheral
  while (peripheral.connected()) {
    // Check if the characteristic value has been written by the central device
      if (ledReadingCharactaristic.written()) {
        ledRead = ledReadingCharactaristic.value(); // Get the new LED value
        Serial.print("* Received value from central: ");
        Serial.println(ledRead);
        switchLED(ledRead); // Update LED based on received value
        return;
      }
    // Read user input from Serial Monitor to send to peripheral
    if (Serial.available() > 0) {
      int locCount = 0;
      ledWrite = Serial.parseInt();
      if (ledWrite >= 1 && ledWrite <= 8) {
        // Write the new gesture value if it has changed
        if (oldLedValue != ledWrite) {  
          oldLedValue = ledWrite;
          while(locCount < 10)
          {
            Serial.print("* Writing value to led_type characteristic: ");
            Serial.println(ledWrite);
            ledWritingCharactaristic.writeValue((byte)ledWrite);
            Serial.println("* Writing value to led_type characteristic done!");
            Serial.println(" ");
            locCount++;
          }
          return;
        }
      } else {
        Serial.println("Invaliddddd number! Enter a number between 1 and 8.");
      }
    }
  }
  Serial.println("- Peripheral device disconnected!");
}

void switchLED(int incomingNumber)
{
    analogWrite(ledRedPin, 255);
    analogWrite(ledGreenPin, 255);
    analogWrite(ledBluePin, 255);
   switch (incomingNumber) {
      case 1: // Red
        analogWrite(ledRedPin, 0);
        Serial.println("Setting LED color to Red");
        break;
      case 2: // Green
        analogWrite(ledGreenPin, 0);
        Serial.println("Setting LED color to Green");
        break;
      case 3: // Blue
        analogWrite(ledBluePin, 0);
        Serial.println("Setting LED color to Blue");
        break;
      case 4: // Yellow (Red + Green)
        analogWrite(ledRedPin, 0);
        analogWrite(ledGreenPin, 0);
        Serial.println("Setting LED color to Yellow (Red + Green)");
        break;
      case 5: // Cyan (Green + Blue)
        analogWrite(ledGreenPin, 0);
        analogWrite(ledBluePin, 0);
        Serial.println("Setting LED color to Cyan (Green + Blue)");
        break;
      case 6: // Magenta (Red + Blue)
        analogWrite(ledRedPin, 0);
        analogWrite(ledBluePin, 0);
        Serial.println("Setting LED color to Magenta (Red + Blue)");
        break;
      case 7: // White (Red + Green + Blue)
        analogWrite(ledRedPin, 0);
        analogWrite(ledGreenPin, 0);
        analogWrite(ledBluePin, 0);
        Serial.println("Setting LED color to White (Red + Green + Blue)");
        break;
      case 8:
        analogWrite(ledRedPin, 255);
        analogWrite(ledGreenPin, 255);
        analogWrite(ledBluePin, 255);
        Serial.println("Turning off the LED");
        break;
      default:
        // Invalid number, turn off the LED
        Serial.println("Invalid number! Enter a number between 1 and 8.");
        break;
    }
}
