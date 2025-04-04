#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // Change to 0x3D for some displays

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int x_p;
int y_p;
int count = 0;
int click = 0;
int click_cursor = 2;
int no_click_cursor = 1;


void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();


  pinMode(A0, INPUT);       //this is not really necessary I don't think
  pinMode(A1, INPUT);
  pinMode(2, INPUT_PULLUP); //for some reason the joystick click would not work with just being an input and had to be an input_pullup
}

// Function to draw a dot at a given position
void drawDot(int x, int y) {

  if(!click){
  display.clearDisplay();   // Clear the screen
  display.drawPixel(x, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x + no_click_cursor, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x - no_click_cursor, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x, y + no_click_cursor, SSD1306_WHITE); // Draw dot
  display.drawPixel(x, y - no_click_cursor, SSD1306_WHITE); // Draw dot
  display.display();        // Update the display
  }


  //there is defintely a bette/cleaner way to do this
  else{
  display.clearDisplay();   // Clear the screen
  display.drawPixel(x, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x + click_cursor, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x - click_cursor, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x, y + click_cursor, SSD1306_WHITE); // Draw dot
  display.drawPixel(x, y - click_cursor, SSD1306_WHITE); // Draw dot display.drawPixel(x + no_click_cursor, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x - no_click_cursor, y, SSD1306_WHITE); // Draw dot
  display.drawPixel(x, y + no_click_cursor, SSD1306_WHITE); // Draw dot
  display.drawPixel(x, y - no_click_cursor, SSD1306_WHITE); // Draw dot
  display.display();        // Update the display


  }
}

void loop() {
  count++;
  x_p = analogRead(A0);
  y_p = analogRead(A1);
  click = digitalRead(2);

  if((count % 10) == 0){
  Serial.print("click = ");
  Serial.println(click);
  }
  
  drawDot(((((x_p - 333) / 5.5) + 64)), ((((y_p - 333) / 24) + 16))); // Move dot across the middle of the screen

}
