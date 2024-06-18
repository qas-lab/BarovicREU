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
}

void loop() {
  // Example: Cycle through colors

  // Red
  analogWrite(ledRedPin, 255);
  analogWrite(ledGreenPin, 0);
  analogWrite(ledBluePin, 0);
  delay(1000); // Wait for a second

  // Green
  analogWrite(ledRedPin, 0);
  analogWrite(ledGreenPin, 255);
  analogWrite(ledBluePin, 0);
  delay(1000); // Wait for a second

  // Blue
  analogWrite(ledRedPin, 0);
  analogWrite(ledGreenPin, 0);
  analogWrite(ledBluePin, 255);
  delay(1000); // Wait for a second

  // Yellow (Red + Green)
  analogWrite(ledRedPin, 255);
  analogWrite(ledGreenPin, 255);
  analogWrite(ledBluePin, 0);
  delay(1000); // Wait for a second

  // Cyan (Green + Blue)
  analogWrite(ledRedPin, 0);
  analogWrite(ledGreenPin, 255);
  analogWrite(ledBluePin, 255);
  delay(1000); // Wait for a second

  // Magenta (Red + Blue)
  analogWrite(ledRedPin, 255);
  analogWrite(ledGreenPin, 0);
  analogWrite(ledBluePin, 255);
  delay(1000); // Wait for a second

  // White (Red + Green + Blue)
  analogWrite(ledRedPin, 255);
  analogWrite(ledGreenPin, 255);
  analogWrite(ledBluePin, 255);
  delay(1000); // Wait for a second

  // Turn off the LED
  analogWrite(ledRedPin, 0);
  analogWrite(ledGreenPin, 0);
  analogWrite(ledBluePin, 0);
  delay(1000); // Wait for a second
}
