/*
Line Follower library

2017 Talos Electronics
Autor: César Vargas
*/

#include "LineFollower.h"
#include <Arduino.h>

LineFollower::~LineFollower()
{

}

void LineFollower::Setup()
{
  	RobotChasis.Setup();

	Sensor1.Setup();
  	Sensor2.Setup();
  	Sensor3.Setup();
  	Sensor4.Setup();
  	Sensor5.Setup();
}

void LineFollower::LineFollowerMode(int KP, int KI, int KD, int Speed)
{
	GetSensorValues();

	fP = fError;
	fI = fI + fLast_I;
	fD = fError - fLast_Error;

	fPID = (KP * fP) + (KI * fI) + (KD * fD);

	fLast_I = fI;
	fLast_Error = fError;

	int SpeedMotorFrontLeft 	= Speed - fPID;
	int SpeedMotorRearLeft 		= Speed - fPID;
	int SpeedMotorFrontRight 	= Speed + fPID;
	int SpeedMotorRearRight 	= Speed + fPID;

	constrain(SpeedMotorFrontLeft	, 0, 255);
	constrain(SpeedMotorRearLeft	, 0, 255);
	constrain(SpeedMotorFrontRight	, 0, 255);
	constrain(SpeedMotorRearRight	, 0, 255);

	RobotChasis.Forward(SpeedMotorFrontLeft, SpeedMotorFrontRight, SpeedMotorRearLeft, SpeedMotorRearRight);
}

void LineFollower::GetSensorValues()
{
	nSensor[0] = Sensor1.getDigitalValue();
	nSensor[1] = Sensor2.getDigitalValue();
	nSensor[2] = Sensor3.getDigitalValue();
	nSensor[3] = Sensor4.getDigitalValue();
	nSensor[4] = Sensor5.getDigitalValue();

	if( (nSensor[0] == 0) && (nSensor[1] == 0) && (nSensor[2] == 0) && (nSensor[3] == 0) && (nSensor[4] == 0) )
	{
		fError = fError;
	}
	else
	{
		if     ( (nSensor[0] == 0) && (nSensor[1] == 0) && (nSensor[2] == 0) && (nSensor[3] == 0) && (nSensor[4] == 1) )
		{
			fError = 4;
		}
		else if( (nSensor[0] == 0) && (nSensor[1] == 0) && (nSensor[2] == 0) && (nSensor[3] == 1) && (nSensor[4] == 1) )
		{
			fError = 3;
		}
		else if( (nSensor[0] == 0) && (nSensor[1] == 0) && (nSensor[2] == 0) && (nSensor[3] == 1) && (nSensor[4] == 0) )
		{
			fError = 2;
		}
		else if( (nSensor[0] == 0) && (nSensor[1] == 0) && (nSensor[2] == 1) && (nSensor[3] == 1) && (nSensor[4] == 0) )
		{
			fError = 1;
		}
		else if( (nSensor[0] == 0) && (nSensor[1] == 0) && (nSensor[2] == 1) && (nSensor[3] == 0) && (nSensor[4] == 0) )
		{
			fError = 0;
		}
		else if( (nSensor[0] == 0) && (nSensor[1] == 1) && (nSensor[2] == 1) && (nSensor[3] == 0) && (nSensor[4] == 0) )
		{
			fError = -1;
		}
		else if( (nSensor[0] == 0) && (nSensor[1] == 1) && (nSensor[2] == 0) && (nSensor[3] == 0) && (nSensor[4] == 0) )
		{
			fError = -2;
		}
		else if( (nSensor[0] == 1) && (nSensor[1] == 1) && (nSensor[2] == 0) && (nSensor[3] == 0) && (nSensor[4] == 0) )
		{
			fError = -3;
		}
		else if( (nSensor[0] == 1) && (nSensor[1] == 0) && (nSensor[2] == 0) && (nSensor[3] == 0) && (nSensor[4] == 0) )
		{
			fError = -4;
		}
	}
}

