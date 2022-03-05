#include "DisplayOLED.h"

DisplayOLED::DisplayOLED(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin) : Adafruit_SSD1306(w, h, &Wire, rst_pin)
{
}

/**
    @brief  Inicializa o display OLED.
    @param  display Objeto da classe DisplayOLED.
    @param  vcs
            VCC selection. Pass SSD1306_SWITCHCAPVCC to generate the display
            voltage (step up) from the 3.3V source, or SSD1306_EXTERNALVCC
            otherwise. Most situations with Adafruit SSD1306 breakouts will
            want SSD1306_SWITCHCAPVCC.
    @param  screen_address
            I2C address of corresponding SSD1306 display (or pass 0 to use
            default of 0x3C for 128x32 display, 0x3D for all others).
            SPI displays (hardware or software) do not use addresses, but
            this argument is still required (pass 0 or any value really,
            it will simply be ignored). Default if unspecified is 0.
    @return None (void).
*/
void DisplayOLED::beginDisplay(DisplayOLED *display, uint8_t vcs, uint8_t screen_address)
{
  if (!display->begin(vcs, screen_address))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display->clearDisplay();

  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(0, 0);
  display->display();
}

/**
    @brief  Imprimi na tela OLED nome e versão do software.
    @param display Objeto da classe DisplayOLED.
    @return None (void).
*/
void DisplayOLED::welcomeOLED(DisplayOLED *display)
{
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);

  display->setCursor(10, 10);
  display->println("CONVERTER");
  display->display();

  delay(2000);

  display->clearDisplay();
  display->setCursor(40, 10);
  display->println("BODH");
  display->display();

  delay(2500);
}

/**
    @brief  Exibe na tela OLED (128x32) o centro do display com linhas horizontais e verticais.
    @param display Objeto da classe DisplayOLED.
    @return None (void).
*/
void DisplayOLED::centerDisplay(DisplayOLED *display)
{
  display->drawLine(0, SSD1306_LCDHEIGHT / 2.0, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT / 2.0, SSD1306_WHITE); // LINE WIDTH
  display->drawLine(SSD1306_LCDWIDTH / 2.0, 0, SSD1306_LCDWIDTH / 2.0, SSD1306_LCDHEIGHT, SSD1306_WHITE);  // LINE HEIGHT

  display->display();
}

/**
    @brief  Exibe na tela OLED (128x32) os limites do display.
    @param display Objeto da classe DisplayOLED.
    @return None (void).
*/
void DisplayOLED::limitsDisplay(DisplayOLED *display)
{
  display->drawLine(0, 0, SSD1306_LCDWIDTH, 0, SSD1306_WHITE);                                         // LINE UP
  display->drawLine(0, 0, 0, SSD1306_LCDHEIGHT, SSD1306_WHITE);                                        // LINE LEFT
  display->drawLine(SSD1306_LCDWIDTH - 1, 0, SSD1306_LCDWIDTH - 1, SSD1306_LCDHEIGHT, SSD1306_WHITE);  // LINE RIGHT
  display->drawLine(0, SSD1306_LCDHEIGHT - 1, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT - 1, SSD1306_WHITE); // LINE DOWN

  display->display();
}

/**
    @brief  Imprimi no canto inferior direito a página atual.
    @param  page Número da página para ser exibida.
    @return None (void).
*/
void DisplayOLED::printpage(DisplayOLED *display, uint8_t page)
{
  display->setCursor(120, 25);
  display->print(page);

  display->display();
}
