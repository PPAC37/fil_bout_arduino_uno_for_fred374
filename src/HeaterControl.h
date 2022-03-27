/****************************************************************
 * Heater control library header
 *
 * 
 * Sources : Mini-Thermostat © GPL3+ by salhar1 https://create.arduino.cc/projecthub/salhar1/mini-thermostat-e732ed?ref=search&ref_id=NTC%20Thermistor&offset=3
*****************************************************************/

#ifndef HeaterControl_h
#define HeaterControl_h

#include "Arduino.h"

class HeaterControl
{
  public:
    HeaterControl(int FETPin);
    int Control_PID(double dCurrentTemperature, int dt, int ThresholdTemp, double MaxTemp, double MinTemp, double SetTemp, double K_P_ctrl, double K_I_ctrl, double K_D_ctrl); //for ATMega328P
    void TurnOff();
   
  private:
    //received
    int _FETPin;
    
    //internal
    float previous_error = 0; //Initialize PID variables
    float s_integral = 0; //Initialize PID variables
};

#endif
