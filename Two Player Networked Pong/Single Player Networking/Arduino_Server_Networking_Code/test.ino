int UPPER_DEADZONE = 750;  // The amount the joystick must be moved to increase coordinates
int LOWER_DEADZONE = 350;  // The amount the joystick must be moved to decrease coordinates
int SCREEN_PADDING = 7;    // This is how close the cursor is able to get to the "edge" of the screen

void drawPong(int x, int y) {
  //code that draws the paddle in based on where the bottom right corner should be
  display.fillRect(x, y - playerH, playerW, playerH, SSD1306_WHITE);
}

void movementPlayer() {
  int stickValue = analogRead(stickY);
  int m = 0;  //Directional multiplier, used to modify the velocity to be positvie/negative or Zero

  if (stickValue > 550 && playerY < height) {          //if stick is is being pushed up and there is screen space to move up
    m = 1;                                             //|set direction to be positive
    d = m;                                             // set d to be current m for ball spawn purposes
  } else if (stickValue < 450 && playerY > playerH) {  //if stick is is being pushed down and there is screen space to move down
    m = -1;                                            //|set direction to be negative
    d = m;
  }

  playerY = playerY + m * vy;
  //have player move "vy" pixels in the "m" direction
}


void drawMap() {
  //display.drawLine(playerX, 0, playerX, height, SSD1306_WHITE);
  //display.drawLine(botX, 0, botX, height, SSD1306_WHITE);
  display.drawLine(width / 2, 0, width / 2, height, SSD1306_WHITE);
}


void setupPlayers() {
  //this code initialises the global variables based on the screen dimensions
  playerH = 2 * floor(width / 17.5);
  playerW = 0.5 * floor(0.5 * playerH);

  playerX = floor(width / 35);
  botX = width - playerX;

  playerY = floor((height + playerH) / 2);
  botY = playerY;
}


void showPlayers() {
  drawPong(playerX, playerY);
  drawPong(botX - playerW, botY);
}


bool collided() {
  // store the current positon of the ball
  int ballX = ball.getX();
  int ballY = ball.getY();
  int rad = ball.r;

  //**************************************

  if ((ballX + rad >= botX - playerW) && (ballX < botX)) {  //if the ball is in or at the x position for the Bot paddle
    if (ballY <= botY && ballY >= botY - playerH) {         //if the ball is in or at the y positon of the bot paddle
      return true;
    }
  } else if (ballX + rad >= playerX && ballX - rad <= playerX + playerW) {  // if the ball is in or at the x position of the player paddle
    if (ballY <= playerY && ballY >= playerY - playerH) {                   // if the ball is in or at the y position of the player paddle
      return true;
    }
  }

  return false;  // ball coords are not the same as or in contact with the coords of either paddle, no collision
}


void movementBot() {
  int targetY,
    m;  //Directional multiplier, used to modify the velocity to be positvie/negative or Zero
  int curr = botY;

  int Y = ball.getY();
  int X = ball.getX();
  int slope = ball.getSlope();

  //find targetY

  if (!ballInPlay) {  // if the ball isnt in play move to center
    targetY = floor((height + playerH) / 2);
  } else {
    targetY = slope * (botX - X - playerW) + Y + floor(playerH / 2);  //find the expected position of the ball using y = m(x2 - x1) + y2

    //if target is off frame, set it to be the nearest extreme of the frame
    if (targetY < playerH) {
      targetY = playerH;
    } else if (targetY > height) {
      targetY = height;
    }
  }

  //now we have the desired y position of the bot (adjusted so that the ball will strike the centre of the paddel)
  //now determine which way you need to go to reach the target

  if (abs(targetY - curr) < vy) {  //if moving would take us further away from the target
    m = 0;                         //already at target dont move
  } else if (targetY > curr) {
    m = 1;  //go up
  } else {
    m = -1;  //go down
  }

  //now move the pong up or down

  if (botY >= 0 && botY <= height) {
    botY = botY + m * vy;
  } else {
    if (botY >= height) {
      botY = height - 1;
    } else {
      botY = 0;
    }
  }
}

void spawnBall() {
  if (!ballInPlay) {
    ball.spawn(floor(playerY - 0.5 * playerH), vy, d, playerX + playerW + ball.r); 
    ballInPlay = true;
  }
}

// First we initialise the cursor to appear at the centre of the screen
void initialiseCursor() {
  display.fillCircle(width / 2, height / 2, 1, SSD1306_WHITE);
  display.display();  // Update the screen
}


// Now we want to update the position of the cursor based on the user input
void drawCursor() {
  // First we need to find what position to draw the cursor at
  updateCursorPosition();
  // Update the size of the cursor depending on whether or not the joystick is being clicked
  updateCursorSize();
  display.fillCircle(cursor.x_position, cursor.y_position, cursor.size, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
  display.display();                                                                     // Update the screen
  cursor.size = 3;
}



void getCursorX() {
  int xPosition = analogRead(1);
  // We check to see if the joystick is outside of the DEADZONE, if it is, we can update accordingly
  if (xPosition > UPPER_DEADZONE) {
    if (cursor.x_position > width - 10) {
      return;
    }
    cursor.x_position = cursor.x_position + 3;
  } else if (xPosition < LOWER_DEADZONE) {
    if (cursor.x_position < 10) {
      return;
    }
    cursor.x_position = cursor.x_position - 3;
  } else {
    return;
  }
}


void getCursorY() {
  int yPosition = analogRead(2);
  // We check to see if the joystick is outside of the DEADZONE, if it is, we update accordingly
  if (yPosition > UPPER_DEADZONE) {

    // Check to see if we have hit the top of the screen
    if (cursor.y_position > height - 10) {
      return;
    }
    cursor.y_position = cursor.y_position + 3;
  } else if (yPosition < LOWER_DEADZONE) {

    // Check to see if we have hit the bottom of the screen
    if (cursor.y_position < 10) {
      return;
    }
    cursor.y_position = cursor.y_position - 3;
  } else {
    return;
  }
}


void updateCursorPosition() {
  getCursorX();  // Get the correct X position
  getCursorY();  // Get the correct Y position
}


void updateCursorSize() {
  if (clicked) {
    // If clicked play the click animation
    clickAnimation();
    return;
  }
}



void clickAnimation() {

  Serial.println(cursor.AnimationFrame);

  // First 3 frames of the animation
  if (cursor.AnimationFrame <= 3) {
    cursor.size = 2;
    display.fillCircle(cursor.x_position + 8, cursor.y_position + 8, 3, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position - 8, cursor.y_position + 8, 3, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position + 8, cursor.y_position - 8, 3, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position - 8, cursor.y_position - 8, 3, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    cursor.AnimationFrame++;
    return;
  }
  // Second three frames of the animation
  if (cursor.AnimationFrame >= 3 && cursor.AnimationFrame <= 6) {
    display.fillCircle(cursor.x_position + 12, cursor.y_position + 12, 2, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position - 12, cursor.y_position + 12, 2, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position + 12, cursor.y_position - 12, 2, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position - 12, cursor.y_position - 12, 2, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    cursor.AnimationFrame++;
    return;
  }
  // Last three frames of the animation
  if (cursor.AnimationFrame >= 6 && cursor.AnimationFrame <= 9) {
    display.fillCircle(cursor.x_position + 20, cursor.y_position + 20, 1, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position - 20, cursor.y_position + 20, 1, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position + 20, cursor.y_position - 20, 1, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    display.fillCircle(cursor.x_position - 20, cursor.y_position - 20, 1, SSD1306_WHITE);  // We draw the cursor at the correct coordiantes
    cursor.AnimationFrame++;
    return;
  }

  // If we reach the end of the animation we reset the click variable and set the frame back to 0
  else {
    clicked = !clicked;
    cursor.Animation = false;
    cursor.AnimationFrame = 0;
    return;
  }
}


// This just draws a nice clean border around the edge of the screen
void menuBorder() {
  display.drawLine(0, 0, width - 1, 0, SSD1306_WHITE);
  display.drawLine(0, 0, 0, height - 1, SSD1306_WHITE);
  display.drawLine(width - 1, height - 1, width - 1, 0, SSD1306_WHITE);
  display.drawLine(width - 1, height - 1, 0, height - 1, SSD1306_WHITE);
}

// Draws the single player button
void singlePlayerButton() {
  display.drawRoundRect(menu.single_button_x_position, menu.single_button_y_position, menu.single_button_width, menu.single_button_height, menu.single_button_radius, SSD1306_WHITE);
  display.fillRoundRect(menu.single_button_x_position + menu.single_icon_x_offset, menu.single_button_y_position + menu.single_icon_y_offset, 3, 10, 1, SSD1306_WHITE);
}

// Draws the multiplayer button
void multiPlayerButton() {
  display.drawRoundRect(menu.multi_button_x_position, menu.multi_button_y_position, menu.multi_button_width, menu.multi_button_height, menu.multi_button_radius, SSD1306_WHITE);
  display.fillRoundRect(menu.single_button_x_position + menu.multi_icon_x_offset, menu.single_button_y_position + menu.multi_icon_y_offset, 3, 10, 1, SSD1306_WHITE);
  display.fillRoundRect(menu.single_button_x_position + menu.multi_icon_x_offset + 4, menu.single_button_y_position + menu.multi_icon_y_offset, 3, 10, 1, SSD1306_WHITE);
}

// Draw the settings button
void settingsButton() {
  display.drawRoundRect(menu.settings_x, menu.settings_y, menu.settings_width, menu.settings_height, menu.settings_radius, SSD1306_WHITE);
}

// Draws the Main Menu
void drawMenu() {

  // Draw the settings button
  settingsButton();
  // Draw the border
  menuBorder();
  // Draw the single player button
  singlePlayerButton();
  // Draw the multiplayer button
  multiPlayerButton();
}









// Check to see if any button has been pressed
void buttonDetect() {
  if (state == 0) {
    if ((clicked) && (cursor.x_position > menu.settings_x && cursor.x_position < menu.settings_x + menu.settings_width)) {
      if (cursor.y_position > 6 && cursor.y_position < 16) {
        state = 1;
        clicked = false;
      } else if (cursor.y_position > 18 && cursor.y_position < 34) {
        //Serial.println("state set to 2");
        state = 2;
        clicked = false;
      } else if (cursor.y_position > 36 && cursor.y_position < 52) {
        state = 3;
        clicked = false;
      }
    } else return;
  }
  else{
  return;
  }
}



void ballstart(int X, int vy, int W, int H) {
  ball.width = W;
  ball.r = floor(ball.width / 70);
  ball.x = X + ball.r;
  ball.height = H;
  ball.dy = vy;
  ball.dx = 2;
  //ball.display = screen;
}

void show() {  //draw the ball
  if (ball.active) {
    //show ball at x,y with radius r
    display.fillCircle(ball.x, ball.y, ball.r, SSD1306_WHITE);
  }
}



void update(boolean collided) {

  if (collided) {  // if the ball hits a paddle change direction
    ball.count++;
    ball.dx = floor(pow(-1, ball.count));
  }

  //update the position of the ball
  ball.x = ball.x + ball.dx;
  ball.y = ball.y + ball.dy;

  //if ball hits the top or bottom of screen, bounce!
  if (ball.y > ball.height || ball.y < 0) {
    ball.dy = ball.dy * -1;  //invert the slope to bounce next frame
  }

  ball.show();
}