#include "header.h"


void setup()
{
  Serial.begin(9600);
  Serial0.begin(115200);
  reflectanceSensors.init();
  button.waitForButton(); 
}


void loop()
{
  selectState();
  delay(120);
}
