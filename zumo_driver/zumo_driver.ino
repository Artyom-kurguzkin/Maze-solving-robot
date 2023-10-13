#include "header.h"


void setup()
{
  Serial.begin(9600);
  Serial0.begin(115200);
  reflectanceSensors.init();
  button.waitForButton(); 
  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
   if((i > 10 && i <= 30) || (i > 50 && i <= 70))
     motors.setSpeeds(-200, 200);
   else
     motors.setSpeeds(200, -200);
   reflectanceSensors.calibrate();
   delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  // Wait for the user button to be pressed and released
  button.waitForButton(); 

  Serial.println("calibration complete");
  Serial0.println("calibration complete");
  digitalWrite(13, LOW);

  Serial.println("ready to begin");
  Serial0.println("ready to begin");
  button.waitForButton();
  Serial.println("start");
  Serial0.println("start");

  // Delay for one second
  delay(1000);
}


void loop()
{
  selectState();
  delay(50);
}
