#include <string.h>
#include "screen.h"
#include "Ball.h"
#include "Cursor.h"
#include "menu.h"
#include "dot.h"

uint8_t up;
uint8_t down;
uint8_t left;
uint8_t right;

uint8_t firstRun = 0;
uint8_t move = 0;
uint8_t state = 0;
uint8_t clickPin = 2;  //stick that takes in stickClicks

int playerH;        //Height of the paddles/pongs
int playerW;        //Width of the paddles/pongs
int playerX, botX;  //X position for player
int playerY, botY;  //Pong y positions

int vy = 3;
int d = 1;  // d is for direction the ball will spawn in
bool ballInPlay = false;
bool clicked = false;

uint8_t width = 128;
uint8_t height = 64;

//***************************************************************************************


// CHANGED JOYSTICK CLICK FOR UI - WILL PROBABLY NOT WORK FOR THE PONG GAME IF WE USE THE SAME ONE
void joystickClick() {
  Serial.println("I FOUND GRACE IN THE BLACK WATER");
  clicked = true;
}  //interrupt function for click detection


void setup() {
  Serial.begin(9600);

  setupOLED();     // screen.h
  setupPlayers();  //intialise global variables

  initialiseCursor();

  attachInterrupt(digitalPinToInterrupt(clickPin), joystickClick, RISING);

  pinMode(8, INPUT);
  pinMode(7, INPUT);
  pinMode(9, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}


void loop() {
  display.clearDisplay();
  up = digitalRead(8);
  down = digitalRead(7);

  if (state == 0) {
    left = digitalRead(9);
    right = digitalRead(3);

    drawMenu();
    drawCursor();
    if (clicked) {
      clickAnimation();
    }
    buttonDetect();
  }

  if (state == 1) {

    // The ball.start function was causing some problems for the main menu so I just put it in here and it will run one time when the single player scene is entered
    if (firstRun == 0) {
      ballstart(playerX + playerW, vy, width, height, display);  //give ball class all the information it needs.
      firstRun++;
    }

    int ballX = ball.getX();
    bool collision = false;

    if (clicked) {
      spawnBall();
      clicked = false;
    }

    menuBorder();
    movementPlayer();  //update playerY every second loop
    movementBot();     //update botY

    drawMap();
    showPlayers();

    collision = collided();  //check if the ball has collided with the pongs

    if (ballInPlay) {
      if (ballX >= width || ballX <= 0) {  // if the ball leaves frame
        ballInPlay = false;                //set play to false so that ball can be "spawned"
        ball.active = false;               //dont show ball
      }
      ball.update(collision);  //update postion of and show ball
    }
  }

  display.display();
}
