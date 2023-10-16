#include "header.h"


int counter = 1; //added one more for the crossroads


const int SPEED_MAX  = 100;
const int SPEED_MIN  = 20;
const int SPEED_HALT = 0;

// states
const int CALIBRATE   = 0;
const int GO_RIGHT    = 1;
const int GO_LEFT     = 2;
const int STOP        = 3;
const int GO_BLIND    = 4;
const int TURN_LEFT   = 5;


int state          = CALIBRATE;
int previous_state = CALIBRATE;


void stateCalibrate()
{
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
     motors.setSpeeds(-300, 300);
   else
     motors.setSpeeds(300, -300);
   reflectanceSensors.calibrate();
   delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);

  Serial.println("calibration complete");
  Serial0.println("calibration complete");
 
  button.waitForButton(); 

  state = GO_RIGHT;
}


void goRight()
{
  Serial.println("right");
  Serial0.println("right");
  motors.setSpeeds(SPEED_MAX, SPEED_MIN); 
}


void goLeft()
{
  Serial.println("left");
  Serial0.println("left");
  motors.setSpeeds(SPEED_MIN, SPEED_MAX);  
}


void stateBlind()
{
  Serial.println("going blind!");
  Serial0.println("going blind!");
  motors.setSpeeds(SPEED_MAX, SPEED_MAX);
}


void stateStop()
{
  Serial.println("stopping");
  Serial0.println("stopping");
  motors.setSpeeds(SPEED_HALT, SPEED_HALT);
  if( counter <= 0 )
  {
    while(1);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.println("end");
  Serial0.println("end");
  }
  delay( 5000 ); 
  counter = counter - 1;
}


void turnLeft()
{
  Serial.println("left");
  Serial0.println("left");
  motors.setSpeeds( -SPEED_MIN, SPEED_MAX);
}


void selectState()
{
  
  position            = reflectanceSensors.readLine(sensors);
  int error           = position - 2500;
  //int speedDifference = error / 4 + 6 * (error - lastError);
  //lastError           = error;

  //int m1Speed = SPEED_MAX + speedDifference;
  //int m2Speed = SPEED_HALT - speedDifference;

  //reflectanceSensors.read(sensorValues);
  //https://github.com/pololu/zumo-shield/blob/master/ZumoReflectanceSensorArray/examples/SensorCalibration/SensorCalibration.ino
  

  if( state == CALIBRATE )
  { 
    state = CALIBRATE; 
  }
  else if( previous_state == CALIBRATE )
  {
    state = GO_LEFT;
  }
  else if( previous_state == GO_LEFT )
  {
    if     ( error > 0 && 
             error < 2500 ){ state = GO_RIGHT; }
    else if( error < 0    ){ state = GO_LEFT; }
    else if( error == 2500 ){ state = STOP; }
    else if( position < 2500 ){ state = TURN_LEFT; }
  }
  else if( previous_state == GO_RIGHT )
  {
    if     ( error > 0 && 
             error < 2500 ){ state = GO_RIGHT; }
    else if( error < 0    ){ state = GO_LEFT; }
    else if( error == 2500 ){ state = STOP; }
    else if( position < 2500 ){ state = TURN_LEFT; }
  }
  else if( previous_state == STOP ){ state = GO_BLIND; }
  else if( previous_state == GO_BLIND )
  {
    if( error == 2500 ){ state = GO_BLIND; }
    else{ state = GO_LEFT; }
  }
  else if( position < 2500 && previous_state == TURN_LEFT )
  {
    if( sensors[1] > 100){ state = TURN_LEFT; }
    else
      state = TURN_LEFT;
  }


  previous_state = state;

  switch(state)
  {
    case CALIBRATE:
      stateCalibrate();
      break;

    case GO_RIGHT:
      goRight();
      break;

    case GO_LEFT:
      goLeft();
      break;

    case STOP:
      stateStop();
      break;

    case GO_BLIND:
      stateBlind();
      break;

    case TURN_LEFT:
      stateBlind();
      break;


    default:
      goRight();
  }
}
