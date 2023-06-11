/*
   1- Author: Francis Pagulayan - 665900279
   2- Lab: Lab 7 - Interrupts 
   3- Description: This Arduino sketch implements a simple game where a number is randomly generated and displayed on a 7-segment display. 
   The goal of the game is for the player to press a button when the displayed number matches the randomly generated number. 
   The code uses the SevSeg library to control the 7-segment display and defines an interrupt service routine for the button press. 
   The loop function generates a random number and displays it on the 7-segment display, and then increments the current number and displays it on the display, 
   looping back to 0 if it reaches 9. If the player presses the button when the displayed number matches the randomly generated number, a message is displayed and the game restarts.
   4-Lab Questions:
What is the purpose of a current-limiting resistor in a seven-segment display circuit?
  The purpose of a current-limiting resistor in a seven-segment display circuit is to limit the current flowing through the LEDs of the display to a safe level. 
  Without a current-limiting resistor, too much current can flow through the LEDs, causing them to burn out or become damaged. 
  The resistor is placed in series with the LED to restrict the current flow to a level that will not cause damage to the LED or the circuit. 
  The value of the resistor is chosen based on the forward voltage drop of the LED and the desired current flow through the LED.
Can you display special characters? If yes, How?
  Yes to an extent. You can display characters supported by a 7 segment display such as A, b, C, c, d, E, F, H, h, L, l, O, o, P, S, and 1-9. 
  This is achievable by using the character representation in code such as B11101110 for A and writing that code to the display.
What are some common applications of seven-segment displays?
   5- References: http://www.learningaboutelectronics.com/Articles/How-to-display-any-character-on-a-7-segment-LED-display.php
   https://core-electronics.com.au/guides/arduino-workshop-for-beginners/
   https://lastminuteengineers.com/seven-segment-arduino-tutorial/
   6- Demo: Demonstration uploaded to VoiceThread
*/


#include "SevSeg.h"
SevSeg sevseg;

const int buttonPin = 2; 
int toggleState;
int lastButtonState = HIGH;
int buttonState = HIGH;
long unsigned int lastPress;
volatile int buttonFlag = 0;
int debounceTime = 50;

int randomNumber = 0; // Variable to store the randomly generated number
int currentNumber = 0; // Variable to store the current number on the display
bool gameOver = true; // Flag to indicate if the game is over or not

void ISR_button() {
  if (millis() - lastPress > debounceTime && (buttonState != lastButtonState)) { // Check for debounce
    lastPress = millis();
    lastButtonState = buttonState;
    buttonFlag = 1;
  }
}

void checkButton() {
  if (buttonFlag) {
    buttonFlag = 0;
    if (currentNumber-1 == randomNumber) {
      Serial.println("Congratulations! You pressed at the correct number.");
      gameOver = true;
      Serial.println("Game is restarting...");
      delay(1000);
    } else {
      Serial.println("Oops! You stopped at the wrong number. Resuming the game.");
    }
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Initialize random seed
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button, CHANGE);
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {11, 10, 8, 7, 6, 4, 5, 9};
  bool resistorsOnSegments = true;
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void loop() {
  // Read the state of the button
  buttonState = digitalRead(buttonPin);
  if (gameOver) {
    randomNumber = random(0, 10); // Generate a random number between 0 and 9
    Serial.print("Random number: ");
    Serial.println(randomNumber);
    currentNumber = 0; // Reset the current number to 0
    gameOver = false;
  }
  sevseg.setNumber(currentNumber);
  sevseg.refreshDisplay();
  currentNumber++;
  if (currentNumber > 9) {
    currentNumber = 0; // Loop back to 0 if the display reaches 9
  }
  delay(500); // Increment the display twice every second
  
  checkButton(); // Check if the button is pressed
}

