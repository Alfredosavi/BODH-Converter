#ifndef _DisplayOLED_H_
#define _DisplayOLED_H_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

class DisplayOLED : public Adafruit_SSD1306
{
public:
  DisplayOLED(uint8_t w, uint8_t h, TwoWire *twi = &Wire, int8_t rst_pin = -1);

  void beginDisplay(DisplayOLED *display, uint8_t vcs, uint8_t screen_address);
  void welcomeOLED(DisplayOLED *display);

  void centerDisplay(DisplayOLED *display);
  void limitsDisplay(DisplayOLED *display);
  void printpage(DisplayOLED *display, uint8_t page);

protected:
  TwoWire *wire;
};
#endif
