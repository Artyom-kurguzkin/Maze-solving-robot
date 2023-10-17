


int counter = 2; 


int SPEED_MAX  = 150;
int SPEED_MIN  = 20;
int SPEED_HALT = 0;

// states
const int GO_RIGHT    = 1;
const int GO_LEFT     = 2;
const int STOP        = 3;
const int GO_BLIND    = 4;
const int TURN_LEFT   = 5;


int state          = GO_RIGHT;
int previous_state = GO_RIGHT;


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

  if( counter = 1 )
  {
    previous_state = TURN_LEFT;
  }
  else if( counter <= 0 )
  {
    while(1)
    {
      pinMode(13, OUTPUT);
      digitalWrite(13, HIGH);
      Serial.println("end");
      Serial0.println("end");
    }
  }
  delay( 2000 ); 
  counter = counter - 1;
}


void turnLeft()
{
  Serial.println("left");
  Serial0.println("left");
  motors.setSpeeds( -150, SPEED_MAX);
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
    

  if( previous_state == GO_LEFT )
  {
    if( error == 2500 && ( counter != 1 )  ){ state = STOP; }
    else if( position == 2500 ){ state = TURN_LEFT; }
    else if( error > 0 && 
             error < 2500 ){ state = GO_RIGHT; }
    else if( error < 0    ){ state = GO_LEFT; }
  }
  

  else if( previous_state == GO_RIGHT )
  {
    if( error == 2500 && ( counter != 1 )  ){ state = STOP; }
    else if( position == 2500 ){ state = TURN_LEFT; }
    else if( error > 0 && 
             error < 2500 ){ state = GO_RIGHT; }
    else if( error < 0    ){ state = GO_LEFT; }
  }
  

  else if( previous_state == STOP && counter == 1 ){ state = GO_BLIND; } 
  

  else if( previous_state == GO_BLIND )
  {
    if( error > 2000 ){ state = GO_BLIND; }           
    else{ state = STOP; }
  }
  

  else if( previous_state == TURN_LEFT )
  {
    if( position > 2500 ){ state = TURN_LEFT; } // I don't think this statement ever catches
    else
      state = GO_LEFT; // r18
  }


  previous_state = state;

  switch(state)
  {
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
      turnLeft();
      break;


    default:
      goRight();
  }
}
