#include "header.h"

#define SPEED_MAX   200
#define SPEED_HALT  0


#define STATE_LEFT  0
#define STATE_RIGHT 1


int state       = STATE_RIGHT;
int next_state  = STATE_RIGHT;


void stateLeft()
{
  Serial.println("left");
  Serial0.println("left");
  motors.setSpeeds(SPEED_HALT, SPEED_MAX);  
}


void stateRight()
{
  Serial.println("right");
  Serial0.println("right");
  motors.setSpeeds(SPEED_MAX, SPEED_HALT);  
}


void selectState()
{
  
  position            = reflectanceSensors.readLine(sensors);
  int error           = position - 2500;
  //int speedDifference = error / 4 + 6 * (error - lastError);
  lastError           = error;

  //int m1Speed = SPEED_MAX + speedDifference;
  //int m2Speed = SPEED_HALT - speedDifference;


  if( error > 0 ){ state = STATE_LEFT; }
  else{ state = STATE_RIGHT; }

  
  state = next_state;
  
  switch(state)
  {
    case STATE_LEFT:
      stateLeft();
      break;

    case STATE_RIGHT:
      stateRight();
      break;

    default:
      stateRight();
  }
}
