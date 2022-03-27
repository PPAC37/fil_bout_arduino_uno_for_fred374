/**
* Sources : Mini-Thermostat © GPL3+ by salhar1 https://create.arduino.cc/projecthub/salhar1/mini-thermostat-e732ed?ref=search&ref_id=NTC%20Thermistor&offset=3

*/
#include "Arduino.h"
#include "Thermistor.h"

Thermistor::Thermistor(int ThermistorPin)
{
  pinMode(ThermistorPin, INPUT);
  thermistorPin = ThermistorPin;
}

double Thermistor::Thermistor_Read(int TemperatureOffset, double T0, double R0, double B)
{
  //Thermistor code with a generalized equation
  //Steinhart-Hart equation: T[K]=1/(a+b*log(R)+c*(log(R))^3)
  //where a=1/T_0-(1/B)*log(R_0), b=1/B and c = 0

  //take a reading:
  int RawADC = analogRead(thermistorPin);
  double Temp;

  double R;
  R = 10000.0 * (1024.0 / RawADC - 1);
  Temp = 1 / (1 / T0 + 1 / B * log(R / R0));
  Temp = Temp - 273.15;            // Convert Kelvin to Celsius
  return Temp;
}