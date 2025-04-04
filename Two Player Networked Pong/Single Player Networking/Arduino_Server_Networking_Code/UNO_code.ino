/*#include <string.h>
#include "screen.h"
#include "Ball.h"
#include "Cursor.h"
#include "menu.h"
#include "dot.h"


int firstRun = 0;
int state = 0;
int stickX = A2;
int stickY = A1;   //pin that takes in Y value of joyStick
int clickPin = 2;  //stick that takes in stickClicks

int playerH;        //Height of the paddles/pongs
int playerW;        //Width of the paddles/pongs
int playerX, botX;  //X position for player
int playerY, botY;  //Pong y positions

int vy = 2;
int d = 1;  // d is for direction the ball will spawn in
bool ballInPlay = false;
bool clicked = true;

int width = 128;
int height = 64;

//***************************************************************************************


// CHANGED JOYSTICK CLICK FOR UI - WILL PROBABLY NOT WORK FOR THE PONG GAME IF WE USE THE SAME ONE
void joystickClick() {

  if (state == 0) {
    cursor.AnimationFrame = 0;
    cursor.size = 2;
    clicked = !clicked;
    return;
  }

  // Functionality if we are playing pong
  else {
    clicked == true;
    return;
  }
}  //interrupt function for click detection


void setup() {

  Serial.begin(9600);
  setupOLED();     // screen.h
  setupPlayers();  //intialise global variables


  attachInterrupt(digitalPinToInterrupt(clickPin), joystickClick, RISING);
  drawMap();  //-
  initialiseCursor();

  pinMode(8, INPUT);
  pinMode(9, INPUT);

}


void loop() {
  display.clearDisplay();

  int up;
  int down;


  up = digitalRead(8);
  if(up == HIGH){
    cursor.x_position++;
  }

  display.fillCircle(cursor.x_position, cursor.y_position, 3, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes

  display.display();
}
*/