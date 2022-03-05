#include "RotaryEncoderButton.h"

RotaryEncoderButton::RotaryEncoderButton(
    int pin1,
    int pin2,
    int pinButton,
    int debounce,
    int DCgap,
    int holdTime,
    int longHoldTime) : RotaryEncoder(pin1, pin2)
{
  _pinButton = pinButton;
  _debounce = debounce;
  _DCgap = DCgap;
  _holdTime = holdTime;
  _longHoldTime = longHoldTime;

  // Initialize the pin
  pinMode(_pinButton, INPUT_PULLUP);

  // Initialize the state
  _buttonVal = true;          // Value read from button
  _buttonLast = true;         // Buffered value of the button's previous state
  _DCwaiting = false;         // Whether we're waiting for a double click (down)
  _DConUp = false;            // Whether to register a double click on next release, or whether to wait and click
  _singleOK = true;           // Whether it's OK to do a single click
  _downTime = -1;             // Time the button was pressed down
  _upTime = -1;               // Time the button was released
  _ignoreUp = false;          // Whether to ignore the button release because the click+hold was triggered
  _waitForUp = false;         // When held, whether to wait for the up event
  _holdEventPast = false;     // Whether or not the hold event happened already
  _longHoldEventPast = false; // Whether or not the long hold event happened already
}

int RotaryEncoderButton::checkButton()
{
  int event = 0;
  _buttonVal = digitalRead(_pinButton);

  // Button pressed down
  if (_buttonVal == LOW && _buttonLast == HIGH &&
      (millis() - _upTime) > (unsigned long)_debounce)
  {
    // If we're waiting for a double click, then we're done
    _downTime = millis();
    _ignoreUp = false;
    _waitForUp = false;
    _singleOK = true;
    _holdEventPast = false;
    _longHoldEventPast = false;

    if ((millis() - _upTime) < (unsigned long)_DCgap &&
        _DConUp == false && _DCwaiting == true)
      _DConUp = true;

    else
      _DConUp = false;

    _DCwaiting = false;
  }

  // Button released
  else if (_buttonVal == HIGH && _buttonLast == LOW &&
           (millis() - _downTime) > (unsigned long)_debounce)
  {
    if (not _ignoreUp)
    {
      _upTime = millis();

      if (_DConUp == false)
        _DCwaiting = true;

      else
      {
        event = 2;
        _DConUp = false;
        _DCwaiting = false;
        _singleOK = false;
      }
    }
  }

  // Test for normal click event: _DCgap expired
  if (_buttonVal == HIGH && (millis() - _upTime) >= (unsigned long)_DCgap && _DCwaiting == true &&
      _DConUp == false &&
      _singleOK == true &&
      event != 2)
  {
    event = 1;
    _DCwaiting = false;
  }

  // Test for HOLD
  if (_buttonVal == LOW && (millis() - _downTime) >= (unsigned long)_holdTime)
  {
    // Trigger "normal" HOLD
    if (not _holdEventPast)
    {
      event = 3;
      _waitForUp = true;
      _ignoreUp = true;
      _DConUp = false;
      _DCwaiting = false;
      _holdEventPast = true;
    }

    // Trigger "long" HOLD
    if ((millis() - _downTime) >= (unsigned long)_longHoldTime)
    {
      if (not _longHoldEventPast)
      {
        event = 4;
        _longHoldEventPast = true;
      }
    }
  }

  _buttonLast = _buttonVal;

  return event;
}
