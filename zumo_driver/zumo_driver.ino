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

  float vb = analogRead(A1) * 3.2265 * 2;
  Serial.print("battery: ");  Serial.println(vb);
  Serial0.print("battery: "); Serial0.println(vb);
  if( vb < 4800 )
  {
    Serial.println("battery input too low: "); Serial.println(vb);
    Serial0.println("battery input too low: "); Serial0.println(vb);
    while(1)
        {
          pinMode(13, OUTPUT);
          digitalWrite(13, HIGH);
        }
  }

  button.waitForButton();  
}


void loop()
{
  
  position  = reflectanceSensors.readLine( sensors );
  int error = position - 2500;

  switch( state )
  {
    case FollowLine:
    {
      Serial.println("follow line");
      Serial0.println("follow line");
      
      int speedDifference = error / 4 + 6 * (error - lastError);
      lastError = error;
      
      int m1Speed = MAX_SPEED + speedDifference;
      int m2Speed = MAX_SPEED - speedDifference;

      if (m1Speed < 0)
        m1Speed = 0;
      if (m2Speed < 0)
        m2Speed = 0;
      if (m1Speed > MAX_SPEED)
        m1Speed = MAX_SPEED;
      if (m2Speed > MAX_SPEED)
        m2Speed = MAX_SPEED;

      motors.setSpeeds(m1Speed, m2Speed);

      // Calibrated sensor values will always range from 0 to 1000, 
      // with 0 being as or more reflective (i.e. whiter) than the most 
      // reflective surface encountered during calibration, and 1000 
      // being as or less reflective (i.e. blacker)
      
      int countSenses = 0;
      for( int i = 0; i < 6; i++ )
      {
        if( sensors[ i ] > 200 )
        {
          countSenses = countSenses + 1;
        }
      }
      if( countSenses == 6 )
      {
        state = IntercectionTurn;
        break;
      }

      int endOfLineCount = 0;
      for( int i = 0; i < 6; i++ )
      {
        if( sensors[ i ] < 200 )
        {
          endOfLineCount = endOfLineCount + 1;
        }
      }
      if( endOfLineCount == 6 )
      {
        state = Stop;
        break;
      }

      state = FollowLine;

      break;
    } 



    case Stop:
    {
      Serial.println( "stopping" );
      Serial0.println( "stopping" );
      motors.setSpeeds( SPEED_HALT, SPEED_HALT );

      if( counter == 1 )
      {
        state = IntercectionTurn;
      }
      else if( counter == 2 )
        state = LeapOfFaith;
      else if( counter <= 0 )
      {
        while(1)
        {
          pinMode(13, OUTPUT);
          digitalWrite(13, HIGH);
        }
      }
      delay( 1000 ); 
      counter = counter - 1;

      break;
    }



    case LeapOfFaith:
    {
      Serial.println("going blind!");
      Serial0.println("going blind!");
      motors.setSpeeds( MAX_SPEED, MAX_SPEED );

      int newLine = 0;
      for( int i = 0; i < 6; i++ )
      {
        if( sensors[ i ] > 200 )
        {
          newLine = newLine + 1;
        }
      }
      if( newLine == 6 )
      {
        state = FollowLine;
        break;
      }

      state = LeapOfFaith;

      break;
    }



    case IntercectionTurn:
    {
      Serial.println( "intercection" );
      Serial0.println( "intercection" );
      motors.setSpeeds( -MAX_SPEED, MAX_SPEED);

      int countOutOfLine = 0;
      for( int i = 0; i < 6; i++ )
      {
        if( sensors[ i ] > 200 )
        {
          countOutOfLine = countOutOfLine + 1;
        }
      }
      if( countOutOfLine > 3 )
      {
        state = IntercectionTurn;
        break;
      }

      state = FollowLine;

      break;
    }
  }

  delay(60);
}

// r3
// working pass
// Artyom Kurguzkin, Andrew, Vone
