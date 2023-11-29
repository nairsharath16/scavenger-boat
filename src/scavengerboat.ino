//initiaisation
// Include necessary libraries
#include <Wire.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

// Define a SoftwareSerial object for Bluetooth communication on pins 8 and 9
SoftwareSerial Bluetooth(8, 9);

// Define constants for ultrasonic sensor pins and maximum distance
#define trig_pin A4 // analog input 1
#define echo_pin A5 // analog input 2
#define maximum_distance 200

// Create a NewPing object for the ultrasonic sensor
NewPing sonar(trig_pin, echo_pin, maximum_distance);

// Define variables and pins for motor control
boolean goesForward = false;
int distance = 100;
int motor1 = 6;
int motor11 = 5;
int motor2 = 4;
int motor22 = 3;
int mopping = 2; // Pin for mopping control (assuming it's connected to digital pin 2)
int data1 = 0;
String dataIn = "0";

// Setup function runs once when the Arduino is powered on or reset
void setup()
{
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();

  // Set motor control pins as OUTPUT
  pinMode(motor1, OUTPUT);
  pinMode(motor11, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor22, OUTPUT);
  pinMode(mopping, OUTPUT);

  // Ensure all motors and mopping mechanism are initially turned off
  digitalWrite(motor22, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor1, LOW);
  digitalWrite(motor11, LOW);
  digitalWrite(mopping, LOW);

  // Bluetooth initialization
  Bluetooth.begin(9600); // Set baud rate to 9600 bps
  Bluetooth.setTimeout(1); // Set a timeout for Bluetooth communication
  delay(1000); // Allow time for Bluetooth module to initialize
}

// Function to read distance from the ultrasonic sensor
void readPing()
{
  delay(70); // Wait for the sensor to stabilize
  distance = sonar.ping_cm(); // Measure distance in centimeters
}

// Main loop function runs repeatedly
void loop()
{
  // Check if the robot is in automatic mode (data1 == 1)
  if (data1 == 1)
  {
    // Check for incoming Bluetooth data
    if (Bluetooth.available() > 0)
    {
      // Read incoming data as a string
      String dataIn = Bluetooth.readString();

      // Check if the received command is for mopping
      if (dataIn.startsWith("MP1"))
      {
        digitalWrite(mopping, LOW); // Turn off mopping
        delay(1000);
        Serial.print(dataIn);
      }
      else if (dataIn.startsWith("MP0"))
      {
        digitalWrite(mopping, HIGH); // Turn on mopping
        delay(1000);
        Serial.print(dataIn);
      }
    }

    // Read distance from the ultrasonic sensor
    readPing();

    // Perform actions based on the measured distance
    if (distance <= 20)
    {
      // If distance is less than or equal to 20cm, perform specific motor actions
      digitalWrite(motor22, HIGH);
      digitalWrite(motor2, LOW);
      digitalWrite(motor1, LOW);
      digitalWrite(motor11, HIGH);
      delay(1500);
      digitalWrite(motor22, LOW);
      digitalWrite(motor2, HIGH);
      digitalWrite(motor1, LOW);
      digitalWrite(motor11, HIGH);
      delay(1000);
    }
    else if (distance > 25)
    {
      // If distance is greater than 25cm, perform different motor actions
      digitalWrite(motor22, LOW);
      digitalWrite(motor2, HIGH);
      digitalWrite(motor1, HIGH);
      digitalWrite(motor11, LOW);
    }
  }

  // Check for additional Bluetooth commands in both automatic and manual modes
  if (Bluetooth.available() > 0)
  {
    // Read incoming data as a string
    String dataIn = Bluetooth.readString();

    // Process various commands based on received data
    if (dataIn.startsWith("MP1"))
    {
      digitalWrite(mopping, LOW); // Turn off mopping
      delay(1000);
      Serial.print(dataIn);
    }
    else if (dataIn.startsWith("MP0"))
    {
      digitalWrite(mopping, HIGH); // Turn on mopping
      delay(1000);
      Serial.print(dataIn);
    }
    else if (dataIn.startsWith("AUTO"))
    {
      data1 = 1; // Set automatic mode
    }
    else if (dataIn.startsWith("MANUAL"))
    {
      data1 = 0; // Set manual mode
    }

    // Perform actions based on received commands in manual mode (data1 == 0)
    if (data1 == 0)
    {
      if (dataIn.startsWith("BCK"))
      {
        // Move backward
        digitalWrite(motor22, HIGH);
        digitalWrite(motor2, LOW);
        digitalWrite(motor1, LOW);
        digitalWrite(motor11, HIGH);
        delay(1000);
        Serial.print(dataIn);
      }
      else if (dataIn.startsWith("FNT"))
      {
        // Move forward
        digitalWrite(motor22, LOW);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor1, HIGH);
        digitalWrite(motor11, LOW);
        delay(1000);
        Serial.print(dataIn);
      }
      else if (dataIn.startsWith("LFT"))
      {
        // Turn left
        digitalWrite(motor22, LOW);
        digitalWrite(motor2, HIGH);
        digitalWrite(motor1, LOW);
        digitalWrite(motor11, HIGH);
        delay(1000);
        Serial.print(dataIn);
      }
      else if (dataIn.startsWith("RGT"))
      {
        // Turn right
        digitalWrite(motor22, HIGH);
        digitalWrite(motor2, LOW);
        digitalWrite(motor1, HIGH);
        digitalWrite(motor11, LOW);
        delay(1000);
        Serial.print(dataIn);
      }
      else
      {
        // Stop all motors if no valid command is received
        digitalWrite(motor22, LOW);
        digitalWrite(motor2, LOW);
        digitalWrite(motor1, LOW);
        digitalWrite(motor11, LOW);
        dataIn = "0";
      }
    }
  }
}


 
