
class Cursor {
public:
  uint8_t size = 3;  // Size of cursor
  uint8_t x_position = 64;  // X Position of the cursor, initialised to the centre of the screen
  uint8_t y_position = 32;  // Y Position of the cursor, initialised to the centre of the screen
  bool Animation = false; // Are we playing the cursor animation 
  uint8_t AnimationFrame; // The frame of the animation 

};              // close cursor class
Cursor cursor;  //initialise an instance of the ball class (initialise a ball)
