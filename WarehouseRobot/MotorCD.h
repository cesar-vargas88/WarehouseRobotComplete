/*
MotorCD library

2017 Talos Electronics
Autor: César Vargas
*/

#ifndef MotorCD_H
#define MotorCD_H

class MotorCD
{
	private:

	  int   nA;			// A Signal to H Bridge 
	  int	nB;			// B Signal to H Bridge 
	  int   nPWM;		// Enable signal to H Bridge 
	  
	public:

		MotorCD(int a, int b, int pwm): nA(a), nB(b), nPWM(pwm){}		// Constructor for MotorCD class
		~MotorCD();														// Destructor for MotorCD class
		
		void Setup();													// Initializes the GPIO
		void Forward(int Speed);										// Enables motor to go forward
		void Backward(int Speed);										// Enables motor to go backward
		void Stop();													// Enables motor to stop
};

#endif


