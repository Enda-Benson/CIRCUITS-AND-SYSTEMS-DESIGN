#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

#define PLAYER_ID 1   // Set to 1 for Player 1 or 2 for Player 2

#define BUTTON_PIN 2                  // Use digital pin 2 for the button
volatile bool buttonPressed = false;  // Flag for button press

// Interrupt Service Routine for the button
void buttonISR() {
  buttonPressed = true;
}

// WiFi credentials
char ssid[] = "giveTeam1fullmarks";
char pass[] = "giveTeam1fullmarks";

// Server details – Arduino acting as an AP (server) typically uses IP 192.168.4.1
IPAddress serverIP(192, 168, 4, 1);
const uint16_t serverPort = 5200;

// Define static IP addresses for each player based on PLAYER_ID
#if PLAYER_ID == 1
IPAddress localIP(192, 168, 4, 2);
#elif PLAYER_ID == 2
IPAddress localIP(192, 168, 4, 3);
#else
// If no valid player is defined, use DHCP.
#endif

// Define network configuration (gateway, subnet, DNS)
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 4, 1);

// WiFi client instance
WiFiClient client;

// Sensor and threshold variables
float x, y, z;
const float threshold = 0.25;
const float neg_threshold = -0.25;

//----------------------------------------------
// Setup WiFi with static IP (if defined) and connect
//----------------------------------------------
void setupWiFi() {
  Serial.println("Connecting to WiFi...");
  
  #if PLAYER_ID == 1 || PLAYER_ID == 2
    // Configure the static IP for this Nano
    WiFi.config(localIP, dns, gateway, subnet);
  #endif
  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");
    delay(5000);
  }
  
  Serial.println("WiFi connected.");
  Serial.print("Nano (Player ");
  Serial.print(PLAYER_ID);
  Serial.print(") IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

//----------------------------------------------
// Connect to the server at the given IP and port
//----------------------------------------------
void connectToServer() {
  Serial.print("Connecting to server at ");
  Serial.print(serverIP);
  Serial.print(":");
  Serial.println(serverPort);
  
  while (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection to server failed, retrying...");
    delay(5000);
  }
  Serial.println("Connected to server!");
}

//----------------------------------------------
// Initialize the IMU sensor
//----------------------------------------------
void setupSensors() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      delay(1000);
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
}

//----------------------------------------------
// Check WiFi and server connection; reconnect if lost
//----------------------------------------------
void checkConnections() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    setupWiFi();
  }
  
  if (!client.connected()) {
    Serial.println("Lost connection to server. Reconnecting...");
    connectToServer();
  }
}

//----------------------------------------------
// Read sensor data and send commands to the server
//----------------------------------------------
void readAndSendData() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }
  
  if (x > threshold) {
    client.print('0');
    Serial.println("Down");
  }
  else if (x < neg_threshold) {
    client.print('1');
    Serial.println("Up");
  }
  if (y > threshold) {
    client.print('3');
    Serial.println("Right");
  }
  else if (y < neg_threshold) {
    client.print('4');
    Serial.println("Left");
  }
  else if((x < threshold) && (x > neg_threshold) && (y < threshold) && (y > neg_threshold)){
    client.print('6');
    Serial.println("rest");
  }


  if (buttonPressed) {
    buttonPressed = false;
    client.print('2');
    Serial.println("Button was pressed!");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    delay(1000);
    
  Serial.println("Nano Client Started");
  
  // Setup button with interrupt
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
  Serial.println("Button interrupt ready.");
  
  // Initialize sensor(s)
  setupSensors();
  
  // Connect to WiFi
  setupWiFi();
  
  // Connect to the server
  connectToServer();
}

void loop() {
  checkConnections();
  readAndSendData();
  delay(100);
}
