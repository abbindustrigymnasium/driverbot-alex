#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1

void setup() {
  // put your setup code here, to run once:
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);

  Serial.begin(115200);
}
// loop som kör motorn åt ett visst håll i hastigheten 1024
void loop() {
  int speed = 1024;
  int dir = 0;
// nu stannar motorn och printar farten åt vardera håll
  delay(2200);
  digitalWrite(motorPinRightDir, 2);
  analogWrite(motorPinRightSpeed, speed);
      delay(2200);      
  digitalWrite(motorPinRightDir, 1);
  analogWrite(motorPinRightSpeed, speed);
}
