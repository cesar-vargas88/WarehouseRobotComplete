/*
TCRT500 sensor library

2017 Talos Electronics
Autor: César Vargas
*/

#include <Arduino.h>
#include "TCRT5000.h"

TCRT5000::~TCRT5000()
{

}

void TCRT5000::Setup()
{
	pinMode(nPinDigital, INPUT);
}

int TCRT5000::getDigitalValue()
{
	return digitalRead(nPinDigital);
}


