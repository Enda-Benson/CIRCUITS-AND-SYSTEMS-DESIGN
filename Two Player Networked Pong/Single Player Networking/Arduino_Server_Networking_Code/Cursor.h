
class Cursor {
public:

  int size = 3;  // Size of cursor

  int x_position = 64;  // X Position of the cursor, initialised to the centre of the screen
  int y_position = 32;  // Y Position of the cursor, initialised to the centre of the screen
  bool Animation = false; // Are we playing the cursor animation 
  int AnimationFrame; // The frame of the animation 

  int width, height;         // Screen dimensions
  Adafruit_SSD1306 display;  // Initialise the display

  void start(int W, int H, Adafruit_SSD1306 screen) {
    width = W;
    height = H;
    display = screen;
  }


};              // close cursor class
Cursor cursor;  //initialise an instance of the ball class (initialise a ball)
