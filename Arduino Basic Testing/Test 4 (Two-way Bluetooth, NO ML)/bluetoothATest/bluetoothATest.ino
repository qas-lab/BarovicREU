/*
  BLE_Central_Device.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE Sense 
  as a central device and looks for a specified service and characteristic in a 
  peripheral device. If the specified service and characteristic are found in a 
  peripheral device, the last detected value of the on-board gesture sensor of 
  the Nano 33 BLE Sense, the APDS9960, is written in the specified characteristic. 

  The circuit:
  - Arduino Nano 33 BLE Sense. 

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>       // Include the ArduinoBLE library
#include <Arduino_APDS9960.h> // Include the Arduino library for the APDS9960 sensor

// UUIDs for the service and characteristic we want to interact with
const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

// Variables to store the current and previous gesture values
int gesture = -1;
int oldGestureValue = -1;   

void setup() {
  Serial.begin(9600);       // Start the serial communication
  while (!Serial);          // Wait for the serial port to connect

  // Initialize the APDS9960 sensor
  if (!APDS.begin()) {
    Serial.println("* Error initializing APDS9960 sensor!");
  }

  APDS.setGestureSensitivity(80); // Set the sensitivity of the gesture sensor

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
  BLECharacteristic gestureCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
  
  // Check if the characteristic is found and writable
  if (!gestureCharacteristic) {
    Serial.println("* Peripheral device does not have gesture_type characteristic!");
    peripheral.disconnect();
    return;
  } else if (!gestureCharacteristic.canWrite()) {
    Serial.println("* Peripheral does not have a writable gesture_type characteristic!");
    peripheral.disconnect();
    return;
  }
  
  // Continuously check for gestures while connected to the peripheral
  while (peripheral.connected()) {
    gesture = gestureDetectection();

    // Write the new gesture value if it has changed
    if (oldGestureValue != gesture) {  
      oldGestureValue = gesture;
      Serial.print("* Writing value to gesture_type characteristic: ");
      Serial.println(gesture);
      gestureCharacteristic.writeValue((byte)gesture);
      Serial.println("* Writing value to gesture_type characteristic done!");
      Serial.println(" ");
    }
  }
  Serial.println("- Peripheral device disconnected!");
}

int gestureDetectection() {
  // Check if a gesture is available from the sensor
  if (APDS.gestureAvailable()) {
    gesture = APDS.readGesture();

    // Print the detected gesture to the serial monitor
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("- UP gesture detected");
        break;
      case GESTURE_DOWN:
        Serial.println("- DOWN gesture detected");
        break;
      case GESTURE_LEFT:
        Serial.println("- LEFT gesture detected");
        break;
      case GESTURE_RIGHT:
        Serial.println("- RIGHT gesture detected");
        break;
      default:
        Serial.println("- No gesture detected");
        break;
    }
  }
  return gesture; // Return the detected gesture
}
