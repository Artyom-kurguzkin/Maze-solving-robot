#include "zumo_state.h"

// setup a set of states here for the zumo to follow

/* -------- SET MOTOR VALUES---------- */

// These define statements allow us to set some useful values for later
// This type of construction is often used to make a system clearer to 
// understand

// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)

#define SPEED_MAX 200
#define SPEED_HALT 0

/* -----------STATE ENCODING---------- */

// These defines define the state encoding - a mapping from a name to a numerical code
// Add new state definitions by copying the lines here and incrementing the number by one.
// Note: make sure each encoding is unique

#define STATE_FORWARD 0
#define STATE_LEFT    1
#define STATE_RIGHT   2
#define STATE_HALT    3

/* ---------END STATE ENCODING---------- */

// Create a variable to hold a state.  This should be set to the default state.
int state = STATE_FORWARD;

// Create a variable to hold the next state.  This should also be set to the default state.
int next_state = STATE_FORWARD;

/* -----------STATE IMPLEMENTATION---------- */

// Each state should be implemented by a function.
// Each function should also report its state to the wireless serial 

// Serial  - usb connected serial interface
// Serial0 - wireless connected serial interface

// Each state implements a set of actions.  In this case, we can set the motor
// speeds.

void stateForward()
{
  Serial.println("forward");
  Serial0.println("forward");
  motors.setSpeeds(SPEED_MAX, SPEED_MAX);
}

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
  motors.setSpeeds(SPEED_MAX, SPEED_MAX);  
}

void stateHalt()
{
  Serial.println("halt");
  Serial0.println("halt");
  motors.setSpeeds(SPEED_HALT,SPEED_HALT);
}

/* -------END STATE IMPLEMENTATION---------- */

/* -----------STATE CHOICE---------- */

void selectState()
{

  // Update the position estimate and sensor value
  // position is an estimate of the relative position of the line
  // The value runs in the range of 0-5000.
  
  position = reflectanceSensors.readLine(sensors);

  // You can also access the individual sensor values using the following

  // sensors[0] - sensor 0
  // sensors[1] - sensor 1
  // sensors[2] - sensor 2
  // sensors[3] - sensor 3
  // sensors[4] - sensor 4
  // sensors[5] - sensor 5

  // Using a combination of these values and boolean logic, you should be able 
  // to develop a set of boolean logic equations that will control the 
  // Finite State Machine.

  // Choose the next state that the vehicle will be in.
  // Put your state selection logic here

  // Each state needs an if statement like this.  It will 
  // be true when the state matches our encoded state
  if(state == STATE_FORWARD)
  {
    // You choose the state to transition to by writing a 
    // boolean expression.  When this is true, we will 
    // change to this state
    
    if(position < 1000) // Put your condition here
    {
      // Write your desired state here
      next_state = STATE_LEFT;
    }
    
  }

  if(state == STATE_LEFT)
  {
    // You choose the state to transition to by writing a 
    // boolean expression.  When this is true, we will 
    // change to this state
    
    if(position > 2000) // Put your condition here
    {
      // Write your desired state here
      next_state = STATE_FORWARD;
    }
    
  }

  // Add as many of these as required to implement your state tests.
  
  // Once the state selection is complete, update the current state
  state = next_state;

  // This construct is called a switch statement.  You will learn more
  // about this in later topics.  For now, just copy the pattern if you need to add
  // extra states
  // This selects the state function to execute.  If you add any extra 
  // states, they need an entry here - just copy one of the existing blocks
  
  switch(state)
  {
    case STATE_FORWARD:
      stateForward();
      break;

    case STATE_LEFT:
      stateLeft();
      break;

    case STATE_RIGHT:
      stateRight();
      break;

    case STATE_HALT:
      stateHalt();
      break;

    default:
      stateHalt();
  }
}

/* -----------END STATE CHOICE---------- */

#include "zumo_driver.h"
