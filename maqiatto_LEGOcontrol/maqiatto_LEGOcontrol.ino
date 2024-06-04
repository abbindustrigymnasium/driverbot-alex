#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// WiFi and MQTT configuration
const char* WIFI_SSID = "ABBgym_2.4";
const char* WIFI_PASSWORD = "mittwifiarsabra";
const char* MQTT_SERVER = "maqiatto.com";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "alex.widell.orton@hitachigymnasiet.se";
const char* MQTT_PASSWORD = "Forre";
const char* CONTROL_TOPIC = "alex.widell.orton@hitachigymnasiet.se/banan"; // Adjust as needed

// Motor and servo pins
#define motorPinRightSpeed 5 //D1 // Connect to A+ on the motor shield
#define motorPinRightDir 0 //D2 // Connect to A- on the motor shield
#define SERVO_PIN D5

// Ultrasonic sensor pins
#define TRIG_PIN D6
#define ECHO_PIN D7

WiFiClient espClient;
PubSubClient client(espClient);
Servo steeringServo;

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Callback function for MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (message == "up") {
    moveForward();
  } else if (message == "down") {
    moveBackward();
  } else if (message == "left") {
    turnLeft();
  } else if (message == "right") {
    turnRight();
  } else if (message == "") {
    stopCar();
  }
}

// Function to reconnect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe(CONTROL_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Setup function
void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  steeringServo.attach(SERVO_PIN);
  pinMode(motorPinRightSpeed, OUTPUT); 
  pinMode(motorPinRightDir, OUTPUT); 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// Loop function
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// Function to move forward
void moveForward() {
  digitalWrite(motorPinRightDir, LOW);
  analogWrite(motorPinRightSpeed, 1024); 
}

// Function to move backward with distance check
void moveBackward() {
  long distance = measureDistance();
  if (distance < 20) { // Stop if an obstacle is closer than 20 cm
    stopCar();
    Serial.println("Obstacle detected! Stopping.");
  } else {
    digitalWrite(motorPinRightDir, HIGH);
    analogWrite(motorPinRightSpeed, 512); 
  }
}

// Function to turn left
void turnLeft() {
  steeringServo.write(0); // Adjust the angle as needed
}

// Function to turn right
void turnRight() {
  steeringServo.write(180); // Adjust the angle as needed
}

// Function to stop the car
void stopCar() {
  analogWrite(motorPinRightSpeed, 0);
}

// Function to measure distance using HY-SRF05 sensor
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Convert to centimeters
  return distance;
}
