#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>


#define LED_PIN 13

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;

int previousPosition = 2500;

const float INITIAL_SPEED = 150;
const float MAX_SPEED = 150;

float robotSpeed = INITIAL_SPEED;

void setup() {
  // put your setup code here, to run once:
//  pinMode(LED_PIN, OUTPUT);
//
    Serial.begin(9600);
  Serial.print("Hello");

  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);


  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  
//  motors.setLeftSpeed(50);
//  motors.setRightSpeed(50);

  unsigned int sensors[6];

  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensors);

//  delay(10);
  
  Serial.println(position);

  if ((position - previousPosition) > 250 || (position - previousPosition) < -250) {
    robotSpeed = INITIAL_SPEED;
  }

  if (false) { //(position - previousPosition) > 500 || (position - previousPosition) < -500) {
  Serial.println("AAAAA");
  } else {
    if (position == 5000) // we went too far left
    {
  Serial.println("BBBBB");
      motors.setRightSpeed(robotSpeed);
      motors.setLeftSpeed(robotSpeed);
    } else if (position == 0) // we went too far right
    {
  Serial.println("CCCCC");
      motors.setRightSpeed(robotSpeed);
      motors.setLeftSpeed(robotSpeed);
    } else {
      
      float relativeVariable = (position - 2500.0) / 2500.0;
      relativeVariable += 1;

      if (relativeVariable == 0) {
  Serial.println("DDDDD");
        motors.setRightSpeed(robotSpeed);
        motors.setLeftSpeed(robotSpeed);
      } else {
        if (relativeVariable < 1) {
  Serial.println("EEEEE");
          motors.setRightSpeed(robotSpeed * relativeVariable);
          motors.setLeftSpeed(0); // * -relativeVariable);
        } else {
  Serial.println("FFFFF");
          motors.setRightSpeed(0); // * -relativeVariable);
          motors.setLeftSpeed(robotSpeed * relativeVariable);
        }
      }
    }
  }


//  robotSpeed *= 1.05;
//
//  if (robotSpeed > MAX_SPEED) {
//    robotSpeed = MAX_SPEED;
//  }
  
  previousPosition = position;


//     motors.setRightSpeed(MAX_SPEED * relativeVariable);
//     motors.setLeftSpeed(MAX_SPEED * relativeVariable);

//  if(position < 2500){
//   
//    
//     motors.setRightSpeed(MAX_SPEED * relativeVariable);
//     motors.setLeftSpeed(MAX_SPEED * (-relativeVariable));
//    }
//  else{
//     motors.setLeftSpeed(MAX_SPEED + 10);
//     motors.setRightSpeed(MAX_SPEED);
//    }
   
//  qtra.read(sensorValues);

}
