/*
 * Based on Polulu line-following example
 */

#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>

#define condition true

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

int lastError = 0;
int counter = 3;

unsigned int sensors[6];

int position = 0;


enum State{  FollowLine, 
             Stop, 
             LeapOfFaith,
             IntercectionTurn } state;

int MAX_SPEED  = 150;
int SPEED_MIN  = 20;
int SPEED_HALT = 0;

#endif
