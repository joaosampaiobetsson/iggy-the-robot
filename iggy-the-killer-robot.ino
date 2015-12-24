#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <NewPing.h>
#include <SoftwareServo.h>
#include <Pushbutton.h>

#define TRIGGER_PIN  2   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6   // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define SERVO_PIN    11

#define LED_PIN 13

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
SoftwareServo myservo;  // create servo object to control a servo

int songIndex = 0;

int pos = 0;    // variable to store the servo position

bool targetLocked = false;
int rotationStep = 1;

bool turningAround = false;
int turningCount = 0;

bool turningToKill = false;
int whereIsIt = 0;
int seekingCount = -1000;

bool attackMode = false;

const int MAX_TURN = 15;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  Serial.println("Hello");

  // Initialize the reflectance sensors module
  byte pins[] = {4, 5};
  reflectanceSensors.init(pins, 2);
  
  myservo.attach(SERVO_PIN);
  myservo.setMaximumPulse(2200);

  button.waitForButton();
  delay(3000);
}

float rotations() {
  Serial.print("POS: ");
  Serial.println(pos);
  float floatingPos = pos;
  return ((floatingPos-90.0)/180.0) * 15.0;
}

void loop() {
  // put your main code here, to run repeatedly:
  SoftwareServo::refresh();

//  movement();
  if (attackMode == true) {
    // attack!
    motors.setSpeeds(325,325);
  } else {
    if (targetLocked == false) {
      seekThem();
    }

    if (targetLocked == true) {
      destroyThem();
    }

    if (seekingCount == whereIsIt) {
      targetLocked = false;
      attackMode = true;
      motors.setSpeeds(0,0);
    }
  }

  if (attackMode == true) {
    checkBorders();
  }

}

void eyesight() {
  delay(20);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  int distance = sonar.ping_cm();

  if ((distance > 0) && (distance < 40)) {
    targetLocked = true;
    whereIsIt = rotations();

    myservo.write(90); 
        
    turningToKill = true;
    seekingCount = 0;
  } else {
    targetLocked = false;
  }
}

void seekThem() {
  if (targetLocked == false) {
    if (pos >= 180) {
      rotationStep = -5;
    } else if (pos <= 0) {
      rotationStep = 5;
    }

    pos += rotationStep;

    myservo.write(pos);              // tell servo to go to position in variable 'pos'
  }

  eyesight();

}

void destroyThem() {
  delay(20);
  if (whereIsIt < 0) {
    // turn right
    motors.setSpeeds(400,-400);
    seekingCount--;
  } else if (whereIsIt > 0) {
    // turn left
    motors.setSpeeds(-400,400);
    seekingCount++;
  } else {
    // hit them    
  }

}

void checkBorders() {
//  motors.setSpeeds(0,0);
  unsigned int sensors[2];

  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  reflectanceSensors.read(sensors);
  //sensors[0] left
  //sensors[1] right

//  if (((sensors[0] < 1950) || (sensors[1] < 1950)) && ((sensors[0] > 1600) || (sensors[1] > 1600))){
  if ((sensors[0] < 1950) || (sensors[1] < 1950)){
    // we are entering the borders

//      if (turningCount < MAX_TURN) {
//        motors.setSpeeds(-400,400);
//        turningCount++;
//        delay(20);
//      } else {
//        motors.setSpeeds(0,0);
//        attackMode = false;
//        targetLocked = false;
//        seekingCount = -1000;
//        turningCount = 0;
//      }

        if (sensors[0] < (sensors[1])) {
          // rotate left
          while (turningCount < MAX_TURN) {
            motors.setSpeeds(400,-400);
            turningCount++;
            delay(20);
          }
        } else {
          // rotate right
          while (turningCount < MAX_TURN) {
            motors.setSpeeds(-400,400);
            turningCount++;
            delay(20);
          }
        }

        motors.setSpeeds(0,0);
        attackMode = false;
        targetLocked = false;
        seekingCount = -1000;
        turningCount = 0;

  } else {
    // we are inside the arena
  }

}


