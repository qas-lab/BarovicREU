/*
  BLE_Peripheral.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE 
  as a peripheral device and specifies a service and a characteristic. Depending 
  on the value of the specified characteristic, an on-board LED turns on.

  The circuit:
  - Arduino Nano 33 BLE. 

  This example code is in the public domain.
*/

#include <ArduinoBLE.h> // Include the ArduinoBLE library

// Define gesture values using an enumeration for better readability
enum {
  GESTURE_NONE  = -1,
  GESTURE_UP    = 0,
  GESTURE_DOWN  = 1,
  GESTURE_LEFT  = 2,
  GESTURE_RIGHT = 3
};

// UUIDs for the service and characteristic
const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

// Variable to store the gesture value
int gesture = -1;

// Create a BLE service and characteristic
BLEService gestureService(deviceServiceUuid); 
BLEByteCharacteristic gestureCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);

void setup() {
  Serial.begin(9600); // Start serial communication
  while (!Serial);    // Wait for the serial port to connect
  
  // Set the pin modes for the on-board LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize the LEDs to off state
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize the BLE module
  if (!BLE.begin()) {
    Serial.println("- Starting Bluetooth® Low Energy module failed!");
    while (1); // Halt the program if BLE initialization fails
  }

  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)"); // Set the local name for the BLE device
  BLE.setAdvertisedService(gestureService); // Advertise the service
  gestureService.addCharacteristic(gestureCharacteristic); // Add the characteristic to the service
  BLE.addService(gestureService); // Add the service
  gestureCharacteristic.writeValue(-1); // Initialize the characteristic value
  BLE.advertise(); // Start advertising the BLE device

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  BLEDevice central = BLE.central(); // Wait for a central device to connect
  Serial.println("- Discovering central device...");
  delay(500);

  if (central) { // If a central device is found
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    // While the central device is connected
    while (central.connected()) {
      if (gestureCharacteristic.written()) { // If the characteristic value is written
        gesture = gestureCharacteristic.value(); // Get the new gesture value
        writeGesture(gesture); // Perform the action based on the gesture
      }
    }
    
    Serial.println("* Disconnected from central device!");
  }
}

// Function to perform actions based on the gesture value
void writeGesture(int gesture) {
  Serial.println("- Characteristic <gesture_type> has changed!");
  
  switch (gesture) {
    case GESTURE_UP:
      Serial.println("* Actual value: UP (red LED on)");
      Serial.println(" ");
      digitalWrite(LEDR, LOW); // Turn on the red LED
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case GESTURE_DOWN:
      Serial.println("* Actual value: DOWN (green LED on)");
      Serial.println(" ");
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, LOW); // Turn on the green LED
      digitalWrite(LEDB, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case GESTURE_LEFT:
      Serial.println("* Actual value: LEFT (blue LED on)");
      Serial.println(" ");
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, LOW); // Turn on the blue LED
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case GESTURE_RIGHT:
      Serial.println("* Actual value: RIGHT (built-in LED on)");
      Serial.println(" ");
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);
      digitalWrite(LED_BUILTIN, HIGH); // Turn on the built-in LED
      break;
    default:
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);
      digitalWrite(LED_BUILTIN, LOW); // Turn off all LEDs for undefined gestures
      break;
  }      
}
