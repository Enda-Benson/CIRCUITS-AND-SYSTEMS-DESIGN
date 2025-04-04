#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // Change to 0x3D for some displays

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Rectangle dimensions
int rectWidth = 40;
int rectHeight = 20;
int x_p;
int y_p;
int final_x_position;
int final_y_position;
int click = 0;
int click_cursor = 2;
int no_click_cursor = 1;

// Rectangle position (centered)
int x = (display.width() - rectWidth) / 2;
int y = (display.height() - rectHeight) / 2;

// Function 
// 
// to draw the rectangle and play button **(NO display.display() here)**
void drawPlayButton() {
    display.drawRect(x, y, rectWidth, rectHeight, SSD1306_WHITE);

    int triX1 = x + rectWidth / 4;  
    int triY1 = y + rectHeight / 4;  

    int triX2 = x + rectWidth / 4;  
    int triY2 = y + 3 * (rectHeight / 4);  

    int triX3 = x + 3 * (rectWidth / 4);  
    int triY3 = y + rectHeight / 2;  

    display.fillTriangle(triX1, triY1, triX2, triY2, triX3, triY3, SSD1306_WHITE);
}





// Function to draw the cursor **(NO display.display() here)**
void drawCursor(int x, int y, bool click) {
    int size = click ? click_cursor : no_click_cursor; // Cursor size based on click state
    int outlineSize = size + 2; // Outline is slightly larger for erasing old pixels

    // Erase the previous cursor (outline in BLACK)
    for (int dx = -outlineSize; dx <= outlineSize; dx++) {
        for (int dy = -outlineSize; dy <= outlineSize; dy++) {
            display.drawPixel(x + dx, y + dy, SSD1306_BLACK);
        }
    }

    // Draw the cursor when not clicked (filled cross shape)
    if (!click) {
        for (int dx = -size; dx <= size; dx++) {
            for (int dy = -size; dy <= size; dy++) {
                display.drawPixel(x + dx, y + dy, SSD1306_WHITE); // Filled cursor
            }
        }
    }
    // Draw the smaller cross when clicked (not filled)
    else {
        display.drawPixel(x, y, SSD1306_WHITE);
        display.drawPixel(x + size, y, SSD1306_WHITE);
        display.drawPixel(x - size, y, SSD1306_WHITE);
        display.drawPixel(x, y + size, SSD1306_WHITE);
        display.drawPixel(x, y - size, SSD1306_WHITE);
    }
}


void setup() {
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(2, INPUT_PULLUP);

    display.clearDisplay();
    display.display();
}

void loop() {
    x_p = analogRead(A0);
    y_p = analogRead(A1);
    click = digitalRead(2);
    
    final_x_position = ((((x_p - 333) / 5.5) + 64));
    final_y_position = ((((y_p - 333) / 24) + 16));

    display.clearDisplay();  // Clear the screen ONCE per loop

    // Draw everything before updating the screen
    
    drawPlayButton();
    drawCursor(final_x_position, final_y_position, click);
    display.display();  // Only update the screen ONCE
    
    //&& ((final_x_position > 56) && (final_x_position < 72)) && ((final_y_position > 8) && (final_x_position < 24))
    if((!click) && ((final_x_position > 56) && (final_x_position < 72)) && ((final_y_position > 8) && (final_y_position < 24))){
      loading();
    }
}




//NEEDS CLEANING UP
void loading(){
  while(1){
      display.clearDisplay();
      display.display();
      delay(1000);
      display.fillCircle(30, 16, 3, SSD1306_WHITE);
      display.display();
      delay(1000);  
      display.fillCircle(60, 16, 3, SSD1306_WHITE);
      display.display();
      delay(1000);
      display.fillCircle(90, 16, 3, SSD1306_WHITE);
      display.display();
      delay(1000);
      display.clearDisplay();
      Serial.println("loading...");
  }
}