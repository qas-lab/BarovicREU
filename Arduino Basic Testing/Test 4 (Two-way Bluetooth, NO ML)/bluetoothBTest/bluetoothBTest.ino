/*
  BLE_Peripheral.ino
*/
#include <ArduinoBLE.h> // Include the ArduinoBLE library

// UUIDs for the service and characteristic
const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

// Variable to store the LED value
int ledValue = 0;

// Create a BLE service and characteristic
BLEService ledService(deviceServiceUuid); 
BLEByteCharacteristic ledCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);

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
  BLE.setAdvertisedService(ledService); // Advertise the service
  ledService.addCharacteristic(ledCharacteristic); // Add the characteristic to the service
  BLE.addService(ledService); // Add the service
  ledCharacteristic.writeValue(-1); // Initialize the characteristic value
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
      if (ledCharacteristic.written()) { // If the characteristic value is written
        ledValue = ledCharacteristic.value(); // Get the new LED value
        //writeGesture(gesture); // Perform the action based on the gesture
      }
    }
    
    Serial.println("* Disconnected from central device!");
  }
}
