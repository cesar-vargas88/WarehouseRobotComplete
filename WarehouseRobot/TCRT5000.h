/*
TCRT500 sensor library

2017 Talos Electronics
Autor: César Vargas
*/

#ifndef TCRT5000_H
#define TCRT5000_H

class TCRT5000
{
  
	private:

	 	int   nPinDigital;	// TCRT5000 Digital Output
	  
	public:

		TCRT5000(int Pin) : nPinDigital(Pin) {}		// Constructor for MotorCD class
		~TCRT5000();								// Destructor for MotorCD class

		void Setup();								// Initializes the GPIO 
		int getDigitalValue();						// Get the digital value of the TCRT5000 sensor
};

#endif

