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

unsigned int sensors[6];

int position = 0;


void selectState();

#endif