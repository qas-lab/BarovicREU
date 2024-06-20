/*
  BLE_Central_Device.ino
*/
#include <ArduinoBLE.h>       // Include the ArduinoBLE library

// Define the pin numbers for the RGB LED
const int ledRedPin = 22;
const int ledGreenPin = 24;
const int ledBluePin = 23;

// UUIDs for the service and characteristic we want to interact with
const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

// Variables to store the current and previous gesture values
int ledValue = 8;
int oldLedValue = 8;   

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
  
  BLE.setLocalName("Nano 33 BLE (Central)"); // Set the local name for the BLE device
  BLE.advertise();          // Start advertising the BLE device

  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
}

void loop() {
  connectToPeripheral();    // Continuously try to connect to the peripheral device
}

void connectToPeripheral() {
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  // Scan for peripherals with the specified service UUID
  do {
    BLE.scanForUuid(deviceServiceUuid);
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

  // Get the characteristic we want to write to
  BLECharacteristic ledCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
  
  // Check if the characteristic is found and writable
  if (!ledCharacteristic) {
    Serial.println("* Peripheral device does not have led_type characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("* Peripheral does not have a writable led_type characteristic!");
    peripheral.disconnect();
    return;
  }
  
  // Continuously check for gestures while connected to the peripheral
  while (peripheral.connected()) {
    //gesture = gestureDetectection(); //Read the value for the variable here 

    // Write the new gesture value if it has changed
    if (oldLedValue != ledValue) {  
      oldLedValue = ledValue;
      Serial.print("* Writing value to led_type characteristic: ");
      Serial.println(ledValue);
      ledCharacteristic.writeValue((byte)ledValue);
      Serial.println("* Writing value to led_type characteristic done!");
      Serial.println(" ");
    }
  }
  Serial.println("- Peripheral device disconnected!");
}

void switchLED(int incomingNumber)
{
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
