/*
Line Follower library

2017 Talos Electronics
Autor: César Vargas
*/

#ifndef LineFollower_H
#define LineFollower_H

#include "Car4W.h"
#include "TCRT5000.h"

class LineFollower
{
  
private:

	int   nSensor[5] 		= {0, 0, 0, 0, 0};
	float fError 	 		= 0;
	float fP 				= 0;
	float fI 				= 0;
	float fD 				= 0;
	float fPID 				= 0;
  	float fLast_Error	 	= 0;
  	float fLast_I 			= 0;

  	TCRT5000 Sensor1;
  	TCRT5000 Sensor2;
  	TCRT5000 Sensor3;
  	TCRT5000 Sensor4;
  	TCRT5000 Sensor5;

  	Car4W RobotChasis;

public:

	LineFollower(	int MotorFrontLeft_A	, int MotorFrontLeft_B	, int MotorFrontLeft_PWM		,
 		  			int MotorFrontRight_A	, int MotorFrontRight_B	, int MotorFrontRight_PWM		,
 					int MotorRearLeft_A		, int MotorRearLeft_B	, int MotorRearLeft_PWM			,
 		  			int MotorRearRight_A	, int MotorRearRight_B	, int MotorRearRight_PWM		,	
 		  			int S1, int S2, int S3, int S4, int S5 											):
 					RobotChasis(MotorFrontLeft_A	, MotorFrontLeft_B		, MotorFrontLeft_PWM	,
 								MotorFrontRight_A	, MotorFrontRight_B		, MotorFrontRight_PWM	,
 								MotorRearLeft_A		, MotorRearLeft_B		, MotorRearLeft_PWM		,
 								MotorRearRight_A	, MotorRearRight_B		, MotorRearRight_PWM)	, 
 					Sensor1 (S1),
 					Sensor2 (S2),
 					Sensor3 (S3),
 					Sensor4 (S4),
 					Sensor5 (S5){}
	
	~LineFollower();
	
	void Setup();
	void GetSensorValues();
	void LineFollowerMode(int KP, int KI, int KD, int Speed);
};

#endif


