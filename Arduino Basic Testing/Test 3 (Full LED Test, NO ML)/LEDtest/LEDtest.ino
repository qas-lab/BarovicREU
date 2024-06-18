// Define the pin numbers for the RGB LED
const int ledRedPin = 22;
const int ledGreenPin = 23;
const int ledBluePin = 24;

void setup() {
  // Initialize the serial communication:
  Serial.begin(9600);
  
  // Set the RGB LED pins as outputs:
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  // Turn off all colors to start with:
  analogWrite(ledRedPin, 0);
  analogWrite(ledGreenPin, 0);
  analogWrite(ledBluePin, 0);

  // Print instructions to the serial monitor
  Serial.println("Enter a number (1-7) to change the LED color:");
  Serial.println("1 - Red");
  Serial.println("2 - Green");
  Serial.println("3 - Blue");
  Serial.println("4 - Yellow");
  Serial.println("5 - Cyan");
  Serial.println("6 - Magenta");
  Serial.println("7 - White");
}

void loop() {
  // Check if data is available to read from the serial port
  if (Serial.available() > 0) {
    // Read the incoming byte:
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any leading or trailing whitespace
    int incomingNumber = input.toInt();
    
    // Turn off all colors first
    analogWrite(ledRedPin, 0);
    analogWrite(ledGreenPin, 0);
    analogWrite(ledBluePin, 0);
    
    // Set the color based on the incoming number
    switch (incomingNumber) {
      case 1: // Red
        analogWrite(ledRedPin, 255);
        break;
      case 2: // Green
        analogWrite(ledGreenPin, 255);
        break;
      case 3: // Blue
        analogWrite(ledBluePin, 255);
        break;
      case 4: // Yellow (Red + Green)
        analogWrite(ledRedPin, 255);
        analogWrite(ledGreenPin, 255);
        break;
      case 5: // Cyan (Green + Blue)
        analogWrite(ledGreenPin, 255);
        analogWrite(ledBluePin, 255);
        break;
      case 6: // Magenta (Red + Blue)
        analogWrite(ledRedPin, 255);
        analogWrite(ledBluePin, 255);
        break;
      case 7: // White (Red + Green + Blue)
        analogWrite(ledRedPin, 255);
        analogWrite(ledGreenPin, 255);
        analogWrite(ledBluePin, 255);
        break;
      default:
        // Invalid number, turn off the LED
        Serial.println("Invalid number! Enter a number between 1 and 7.");
        break;
    }
  }
}
