// AvenueE 2023 Team 14 StreetSmart Detect and Avoid Project
// Authors: Jacob Marinas, Tian Hong Zhu Zhou (Alex), Ruben Leon
// Date: 09/12/2023

// Track speed, set accel
float speed = 0;
float accelerationRate = 1.100;
float neutralRate = 0.995;
float brakeRate = 0.940;

// Joystick pins / values
const int HORZ = A0;
const int VERT = A1;
int x = 0;
int y = 0;
int pos = 0;

// Pedal pins
const int brakePedal = A2;
const int gasPedal = A3;
bool brake = false;
bool gas = false;

// DC motor pins
const int enableMotor = 5;
const int dirA = 3;
const int dirB = 4;


void setup() {
  // put your setup code here, to run once:
  // Pedal
  pinMode(brakePedal, INPUT_PULLUP);
  pinMode(gasPedal, INPUT_PULLUP);

  // Dc Motor
  pinMode(enableMotor,OUTPUT);
  pinMode(dirA,OUTPUT);
  pinMode(dirB,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Drive subsystem: Jacob
  // Uses joystick, buttons, and dc motors
  // Manages driver input and driving output

  // Update joystick state
  // Read input, scale from pos to pos / neg
  x = analogRead(VERT) - 512; 
  y = analogRead(HORZ) - 512;

  // Calculate degrees, using tan to get radians, convert to degrees
  float radians = atan2(y, x);
  float degrees = radians * 180 / 3.14159 + 180;
  //Serial.println(degrees);

  //printJoystick();

  // Update pedal state
  brake = digitalRead(brakePedal) == LOW;
  gas = digitalRead(gasPedal) == LOW;

  printPedals();

  // Check pedal state, update speed state
  if (gas && !brake) {
    incSpeed();
  } else if (!gas && brake) {
    decSpeed();
  } else {
    neutralSpeed();
  }

  printRates();
  printSpeed();

  // Set motor speed / direction
  motorDirection(true);
  analogWrite(enableMotor, map(speed, 0, 80, 0, 255));


  // Collect distance value for Ultra Sonic Distance Sensor (for all three UDS)
  // Store boolean if distance is below certain threshold (for front, left, and right sensor)

  // 
  // Tone
  // Delay
  // no tone
  // delay

  // Avoid subsystem
  // Uses ultrasonic distance sensor, dc motors, LED
  // If the vehicle is close to an obstacle, it will override driver input, and react to avoid the obstacle accordingly,
  // turning on an LED to indicate that it is overriding controls

  delay(200);
}

void printJoystick() {
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
}

void printPedals() {
  Serial.print("Gas: ");
  Serial.println(gas);
  Serial.print("Brake: ");
  Serial.println(brake);
}

void motorDirection(bool clockwise) {
  // Helper method to set motor direction
  if (clockwise) {
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, LOW);
  } else {
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, HIGH);
  }
}

// Helper methods for updateSpeed()
void incSpeed() {
  updateSpeed(2);
}

void decSpeed() {
  updateSpeed(0);
}

void neutralSpeed() {
  updateSpeed(1);
}

void updateSpeed(int state) {
  // Update speed based on state
  // 0: Braking
  // 1: Neutral
  // 2: Accelerating
  switch (state) {
    // Convert rates to integers to use the map() function to scale rates accordingly
    case 0:
      //int tempBrake = speed * 100;
      //brakeRate = map(speed, 0, 800, 80, 95) * 0.01;
      speed *= brakeRate;
      break;
    case 1:
      speed *= neutralRate;
      break;
    case 2:
      if (speed < 1) {
        speed = 1;
      }
      int tempSpeed = speed * 100;
      accelerationRate = map(speed, 0, 800, 110, 10) * 0.01;
      speed *= accelerationRate;
      break;
  }
}

void printSpeed() {
  Serial.print("Speed: ");
  Serial.println(round(speed));
}

void printRates() {
  Serial.print("Acceleration: ");
  Serial.println(accelerationRate);
  Serial.print("Neutral: ");
  Serial.println(neutralRate);
  Serial.print("Brake: ");
  Serial.println(brakeRate);
}
