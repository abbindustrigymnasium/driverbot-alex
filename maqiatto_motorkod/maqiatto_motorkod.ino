// Including necessary libraries
#include <ESP8266WiFi.h>    
#include <PubSubClient.h>   
#include <Servo.h>          

// WiFi credentials
const char* WIFI_SSID = "ABBgym_2.4";        
const char* WIFI_PASSWORD = "mittwifiarsabra"; 

// MQTT broker details
const char* MQTT_SERVER = "maqiatto.com";                          
const int MQTT_PORT = 1883;                                         
const char* MQTT_USERNAME = "alex.widell.orton@hitachigymnasiet.se"; 
const char* MQTT_PASSWORD = "Forre";                               
const char* CONTROL_TOPIC = "alex.widell.orton@hitachigymnasiet.se/banan"; 
String clientId = String(random(0xffff), HEX);                      // Generating a unique client ID for MQTT

// Pin definitions for servo, motor and sensor
#define motorPinRightSpeed 5
#define motorPinRightDir 0   
#define SERVO_PIN D5        
#define TRIG_PIN D7          
#define ECHO_PIN D8          

// Creating instances of necessary objects
WiFiClient espClient;       
PubSubClient client(espClient); 
Servo steeringServo;        

long duration;      // Variable to store duration of ultrasonic sensor pulse
int distance;       // Variable to store distance measured by ultrasonic sensor
bool isMovingBackward = false; // Variable to track if the car is moving backward

// Function to set up WiFi connection
void setup_wifi() {
    // Displaying connection attempt to WiFi network
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    // Connecting to WiFi network
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Waiting for WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Displaying WiFi connection details
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// Callback function to handle incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
    // Displaying received MQTT message
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Extracting message from payload
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);

    // Handling different messages to control the car
    if (message == "up") {
        moveForward();
    } else if (message == "down") {
        isMovingBackward = true;
        moveBackward();
    } else if (message == "left") {
        turnLeft();
    } else if (message == "right") {
        turnRight();
    } else if (message == "stop_vertical") {
        stopCar();
    } else if (message == "stop_horizontal") {
        resetSteering();
    }
}

// Function to reconnect to MQTT broker
void reconnect() {
    // Attempting to connect to MQTT broker
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("connected");
            client.subscribe(CONTROL_TOPIC);
        } else {
            // Displaying connection failure and retrying after delay
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// Setup function runs once at startup
void setup() {
    Serial.begin(9600); 
    setup_wifi();        
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback); // Setting up MQTT callback function

    // Configuring pin modes
    pinMode(motorPinRightSpeed, OUTPUT);
    pinMode(motorPinRightDir, OUTPUT);   
    pinMode(TRIG_PIN, OUTPUT);        
    pinMode(ECHO_PIN, INPUT);            

    steeringServo.attach(SERVO_PIN); 
}

// Loop function runs repeatedly
void loop() {
    if (!client.connected()) {
        reconnect(); // Reconnecting to MQTT broker if connection is lost
    }
    client.loop(); // Handling MQTT communication

    if (isMovingBackward) {
        checkDistanceAndMoveBackward(); // Checking distance and moving backward if needed
    }
}

// Function to move the car forward
void moveForward() {
    isMovingBackward = false; // Resetting backward movement flag
    digitalWrite(motorPinRightDir, LOW); 
    analogWrite(motorPinRightSpeed, 1024); 
}

// Function to start moving the car backward
void moveBackward() {
    isMovingBackward = true; 
}

// Function to check distance and move backward accordingly
void checkDistanceAndMoveBackward() {
    // Sending ultrasonic pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Reading echo duration and calculating distance
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    // Printing distance for debugging
    Serial.print("Distance: ");
    Serial.println(distance);

    // Adjusting car speed based on distance
    if (distance > 10) {
        digitalWrite(motorPinRightDir, HIGH);
        analogWrite(motorPinRightSpeed, 512); 
    } else {
        stopCar(); // Stopping car if distance is too close
    }
}

// Function to turn the car left
void turnLeft() {
    steeringServo.write(0); 
}

// Function to turn the car right
void turnRight() {
    steeringServo.write(180); 
}

// Function to stop the car
void stopCar() {
    isMovingBackward = false; // Resetting backward movement flag
    analogWrite(motorPinRightSpeed, 0); 
}

// Function to reset the steering to center position
void resetSteering() {
    steeringServo.write(90); 
}
