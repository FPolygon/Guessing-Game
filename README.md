# Guessing-Game
This Arduino sketch implements a simple game where a number is randomly generated and displayed on a 7-segment display. 
   The goal of the game is for the player to press a button when the displayed number matches the randomly generated number. 
   The code uses the SevSeg library to control the 7-segment display and defines an interrupt service routine for the button press. 
   The loop function generates a random number and displays it on the 7-segment display, and then increments the current number and displays it on the display, 
   looping back to 0 if it reaches 9. If the player presses the button when the displayed number matches the randomly generated number, a message is displayed and the game restarts.
