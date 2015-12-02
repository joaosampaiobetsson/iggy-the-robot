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

const int MAX_SPEED = 150;

void setup() {
  // put your setup code here, to run once:
//  pinMode(LED_PIN, OUTPUT);
//
//    Serial.begin(9600);
//  Serial.print("Hello");

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

  delay(10);
  
  Serial.begin(9600);
  Serial.print(position);
  Serial.println();

  float relativeVariable = (position - 2500.0) / 2500.0;


  relativeVariable += 1;

  if (relativeVariable < 1) {
     motors.setRightSpeed(MAX_SPEED * relativeVariable);
     motors.setLeftSpeed(0); // * -relativeVariable);
  } else {
     motors.setRightSpeed(0); // * -relativeVariable);
     motors.setLeftSpeed(MAX_SPEED * relativeVariable);
  }

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
