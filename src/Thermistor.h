/* Thermistor control library header
* Sources : Mini-Thermostat © GPL3+ by salhar1 https://create.arduino.cc/projecthub/salhar1/mini-thermostat-e732ed?ref=search&ref_id=NTC%20Thermistor&offset=3

*/

#ifndef Thermistor_h
#define Thermistor_h

#include "Arduino.h"

class Thermistor
{
  public:
    Thermistor(int ThermistorPin); //constructor
    double Thermistor_Read(int TemperatureOffset);   
    double Thermistor_Read(int TemperatureOffset, double T0, double R0, double B);
    
  private:
    int thermistorPin;    
};

#endif
