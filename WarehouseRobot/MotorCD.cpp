/*
MotorCD library

2017 Talos Electronics
Autor: César Vargas
*/

#include <Arduino.h>
#include "MotorCD.h"

MotorCD::~MotorCD()
{

}

void MotorCD::Setup()
{
	pinMode(nA, 	OUTPUT);
	pinMode(nB, 	OUTPUT);
	pinMode(nPWM, 	OUTPUT);
}

void MotorCD::Forward(int Speed)
{
	analogWrite(nPWM, Speed);
 	digitalWrite(nA, HIGH);
  	digitalWrite(nB, LOW);
}

void MotorCD::Backward(int Speed)
{
	analogWrite(nPWM, Speed);
 	digitalWrite(nA, LOW);
  	digitalWrite(nB, HIGH);
}

void MotorCD::Stop()
{
	analogWrite(nPWM, 0);
 	digitalWrite(nA, LOW);
  	digitalWrite(nB, LOW);
}


