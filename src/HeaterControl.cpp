/****************************************************************
 * Heater control library 
 *
 *  Return codes:
 *   1: in range
 *   0: not in range
 *   -1: error
 * Sources : Mini-Thermostat © GPL3+ by salhar1 https://create.arduino.cc/projecthub/salhar1/mini-thermostat-e732ed?ref=search&ref_id=NTC%20Thermistor&offset=3
*****************************************************************/

#include "Arduino.h"
#include "HeaterControl.h"

HeaterControl::HeaterControl(int FETPin)
{
  pinMode(FETPin, OUTPUT);
  _FETPin = FETPin;
}

int HeaterControl::Control_PID(double dCurrentTemperature, int dt, int ThresholdTemp, double MaxTemp, double MinTemp, double SetTemp, double K_P_ctrl, double K_I_ctrl, double K_D_ctrl)
{
  bool bInRange =0;
  
  //Overheat protection
  if (dCurrentTemperature > MaxTemp) {
    analogWrite(_FETPin, 0); //turn off heater
    return -1; //return with overheat code
  }

  //In range? If yes, return with in range code
  if (dCurrentTemperature >= ThresholdTemp && dCurrentTemperature < (ThresholdTemp + 2)) { bInRange = 1; }

  //PID subroutine
  float err = SetTemp - dCurrentTemperature;
  s_integral += err * dt;
  float s_derivative = (err - previous_error) / dt;
  int U_in_ctrl = (K_P_ctrl * err + K_I_ctrl * s_integral + K_D_ctrl * s_derivative) / (MaxTemp - MinTemp) * 255;
  previous_error = err;

  // put voltage to output and write value to serial monitor
  if (U_in_ctrl <= 255) 
  {
    if (U_in_ctrl > 0) {
      analogWrite(_FETPin, U_in_ctrl);
    }
    else
    {
      analogWrite(_FETPin, 1);
    }
  }
  else 
  {
    analogWrite(_FETPin, 255);
  }

  return bInRange;
}


void HeaterControl::TurnOff()
{
  analogWrite(_FETPin, 0); //turn off heater 
}

