#include <WiFiS3.h>
#include <string.h>
#include "screen.h"
#include "Ball.h"
#include "Cursor.h"
#include "menu.h"
#include "dot.h"

int count = 0;
char data_1;
int firstRun = 0;
int state = 1;
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


// WiFi details
char ssid[] = "giveTeam1fullmarks";
char pass[] = "giveTeam1fullmarks";

// Create a WiFi server on port 5200
WiFiServer server(5200);

// Declare clients for two players
WiFiClient client;
WiFiClient Player_1;
//WiFiClient Player_2;

// Define desired IP addresses for each player
IPAddress desiredPlayer1IP(192, 168, 4, 2);
//IPAddress desiredPlayer2IP(192, 168, 4, 3);

// Setup the Arduino as an Access Point and start the server.
void connectionSetup() {
  Serial.println("Setting up WiFi as Access Point...");
  WiFi.beginAP(ssid, pass);

  delay(10000);  // Allow time for the AP to initialize

  IPAddress ip = WiFi.localIP();
  Serial.print("Arduino Server IP Address: ");
  Serial.println(ip);

  server.begin();
}

// Waits for a client connection that matches the desired IP.
// The playerName parameter is used for logging purposes.
WiFiClient waitForClientWithIP(const IPAddress &desiredIP, const char *playerName) {

  Serial.print("Waiting for ");
  Serial.print(playerName);
  Serial.print(" with IP: ");
  Serial.println(desiredIP);

  while (true) {
    Serial.print(".");
    client = server.available();
    if (client) {
      IPAddress remoteIP = client.remoteIP();
      Serial.print("New connection from: ");
      Serial.println(remoteIP);
      if (remoteIP == desiredIP) {
        Serial.print(playerName);
        Serial.println(" connected!");
        return client;
      } else {
        Serial.print("Connection from ");
        Serial.print(remoteIP);
        Serial.println(" does not match desired IP.");
      }
    }
    delay(100);
  }
}

// Checks for incoming data from both players.
void checkServer() {
  // Check for data from Player 1
  if (Player_1 && Player_1.connected() && Player_1.available()) {
    data_1 = Player_1.read();
    Serial.print("Player 1 data (");
    Serial.print(Player_1.remoteIP());
    Serial.print("): ");
    Serial.println(data_1);
  }
  /*
  // Check for data from Player 2
  if (Player_2 && Player_2.connected() && Player_2.available()) {
    char data_2 = Player_2.read();
    Serial.print("Player 2 data (");
    Serial.print(Player_2.remoteIP());
    Serial.print("): ");
    Serial.println(data_2);
  }
  */
}

// Ensures that the client for a given player is still connected.
// If the connection is lost, it waits for a reconnection from the same desired IP.
void ensureClientConnected(WiFiClient &client, const IPAddress &desiredIP, const char *playerName) {
  if (!client.connected()) {
    Serial.print(playerName);
    Serial.println(" disconnected. Waiting for reconnection...");
    client = waitForClientWithIP(desiredIP, playerName);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  while (!Serial)
    delay(1000);

  Serial.println("Arduino Server Started");
  Serial.println("Setting up WiFi...");

  connectionSetup();

  // Wait for Player 1 (IP 192.168.4.2)
  Player_1 = waitForClientWithIP(desiredPlayer1IP, "Player 1");
  Serial.print("Player 1 connected with IP: ");
  Serial.println(Player_1.remoteIP());
}

void loop() {

  checkServer();

  if (client.connected()) {
    if (data_1 == '1') {
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
    }
    if (data_1 == '0') {
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
    }
    if (data_1 == '6'){
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(9, LOW);
    }
    if (data_1 == '3'){
      digitalWrite(9, HIGH);
      digitalWrite(3, LOW);
    }
    if (data_1 == '4'){
      digitalWrite(9, LOW);
      digitalWrite(3, HIGH);
    }
    if (data_1 == '2') {
      digitalWrite(2, HIGH);
    }
        
  }

  // Monitor connections and wait for reconnections if needed.
  ensureClientConnected(Player_1, desiredPlayer1IP, "Player 1");
  //ensureClientConnected(Player_2, desiredPlayer2IP, "Player 2");
  
}
