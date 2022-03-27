/****************************************************************
 *  header
 *
*****************************************************************/

#ifndef ParserGCode_h
#define ParserGCode_h

#include "Arduino.h"

class ParserGCode
{
  public:
    ParserGCode(int FETPin);
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