/*
Car4W library

2017 Talos Electronics
Autor: César Vargas
*/

#ifndef Car4W_H
#define Car4W_H

#include "MotorCD.h"

class Car4W
{ 
	private:

		int nMotorFrontLeft_A;			// A Signal to H Bridge for Motor Front Left
		int nMotorFrontLeft_B;			// B Signal to H Bridge for Motor Front Left
		int nMotorFrontLeft_PWM;		// Enable signal to H Bridge for Motor Front Left

		int nMotorFrontRight_A;			// A Signal to H Bridge for Motor Front Right
		int nMotorFrontRight_B;			// B Signal to H Bridge for Motor Front Right
		int nMotorFrontRight_PWM;		// Enable signal to H Bridge for Motor Front Right

		int nMotorRearLeft_A;			// A Signal to H Bridge for Motor Rear Left
		int nMotorRearLeft_B;			// B Signal to H Bridge for Motor Rear Left
		int nMotorRearLeft_PWM;			// Enable signal to H Bridge for Motor Rear Left

		int nMotorRearRight_A;			// A Signal to H Bridge for Motor Rear Right
		int nMotorRearRight_B;			// B Signal to H Bridge for Motor Rear Right
		int nMotorRearRight_PWM;		// Enable signal to H Bridge for Motor Rear Right

		MotorCD MotorFrontLeft;			// Object for Motor Front Left
		MotorCD MotorFrontRight;		// Object for Motor Front Right	
		MotorCD MotorRearLeft;			// Object for Motor Rear Left
		MotorCD MotorRearRight;			// Object for Motor Rear Right

	public:

	 	Car4W(	int MotorFrontLeft_A	, int MotorFrontLeft_B	, int MotorFrontLeft_PWM	,
	 		  	int MotorFrontRight_A	, int MotorFrontRight_B	, int MotorFrontRight_PWM	,
	 			int MotorRearLeft_A		, int MotorRearLeft_B	, int MotorRearLeft_PWM		,
	 		  	int MotorRearRight_A	, int MotorRearRight_B	, int MotorRearRight_PWM)	:
	 			MotorFrontLeft 	(MotorFrontLeft_A	, MotorFrontLeft_B	, MotorFrontLeft_PWM 	),
	 			MotorFrontRight (MotorFrontRight_A	, MotorFrontRight_B	, MotorFrontRight_PWM 	),
	 			MotorRearLeft 	(MotorRearLeft_A	, MotorRearLeft_B	, MotorRearLeft_PWM		),
	 			MotorRearRight 	(MotorRearRight_A	, MotorRearRight_B	, MotorRearRight_PWM 	){}		// Constructor for MotorCD class

		~Car4W();		// Destructor for MotorCD class

		void Setup();	// Initializes the GPIO and objects

		void Forward	(int SpeedMotorRearLeft, int SpeedMotorRearRight, int SpeedMotorFrontLeft, int SpeedMotorFrontRight);	// Enables Car to go forward
		void Backward	(int SpeedMotorRearLeft, int SpeedMotorRearRight, int SpeedMotorFrontLeft, int SpeedMotorFrontRight);	// Enables Car to go Backward
		void Left 		(int SpeedMotorRearLeft, int SpeedMotorRearRight, int SpeedMotorFrontLeft, int SpeedMotorFrontRight);	// Enables Car to go Left
		void Right 		(int SpeedMotorRearLeft, int SpeedMotorRearRight, int SpeedMotorFrontLeft, int SpeedMotorFrontRight);	// Enables Car to go Right
		void Stop();																											// Enables Car to Stop
};

#endif

