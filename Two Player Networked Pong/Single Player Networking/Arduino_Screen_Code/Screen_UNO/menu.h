
class Menu {
public:
  uint8_t frame;
  uint8_t size = 3;  // Size of cursor

  uint8_t single_button_x_position = 39; // The x position of the single player button
  uint8_t single_button_y_position = 6;  // The y position of the single player button
  uint8_t single_button_width = 50; // The width of the single player button
  uint8_t single_button_height = 16; // The height of the single player button
  uint8_t single_button_radius = 3; // The radius of the corners of the single player button

  uint8_t multi_button_x_position = 39; // The x position of the multiplayer button
  uint8_t multi_button_y_position = 24;  // The y position of the multiplayer button
  uint8_t multi_button_width = 50; // The width of the multiplayer button
  uint8_t multi_button_height = 16; // The height of the multiplayer button
  uint8_t multi_button_radius = 3; // The radius of the corners of the multiplayer button

  uint8_t single_icon_x_offset = 24; // The x coordinate offset for the singeplayer button icon
  uint8_t single_icon_y_offset = 3; // The y coordinate offset for the singeplayer button icon

  uint8_t multi_icon_x_offset = 22; // The x coordinate offset for the multiplayer button icon
  uint8_t multi_icon_y_offset = 21; // The y coordinate offset for the multiplayer button icon

  uint8_t settings_x = 39; // The x position for the "settings" button
  uint8_t settings_y = 42;  // The y position for the "settings" button
  uint8_t settings_width = 50; // The width of the settings button
  uint8_t settings_height = 16; // The height of the settings button
  uint8_t settings_radius = 3; // The radius of the settings button


  int width, height;         // Screen dimensions
  Adafruit_SSD1306 display;  // Initialise the display

};          // close menu class
Menu menu;  //initialise an instance of the menu
