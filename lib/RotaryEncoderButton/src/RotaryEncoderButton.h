#ifndef _ROTARYENCODERBUTTON_H_
#define _ROTARYENCODERBUTTON_H_

#include <Arduino.h>

#include <RotaryEncoder.h>

class RotaryEncoderButton : public RotaryEncoder
{
private:
  bool _buttonVal;
  bool _buttonLast;
  bool _DCwaiting;
  bool _DConUp;
  bool _singleOK;
  long _downTime;
  long _upTime;
  bool _ignoreUp;
  bool _waitForUp;
  bool _holdEventPast;
  bool _longHoldEventPast;

  int _pinButton;
  int _debounce;
  int _DCgap;
  int _holdTime;
  int _longHoldTime;

public:
  signed char newPos = 0, oldPos = 0;

  RotaryEncoderButton(int pin1,
                      int pin2,
                      int pinButton,
                      int debounce = 50,
                      int DCgap = 500,
                      int holdTime = 2000,
                      int longHoldTime = 5000);

  int checkButton();
};
#endif
