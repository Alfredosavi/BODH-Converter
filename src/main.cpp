#include <Arduino.h>

#include "../lib/RotaryEncoderButton/src/RotaryEncoderButton.h"

#define ROW_0 2
#define ROW_1 3
#define ROW_2 4
#define ROW_3 5
#define COL_0 7
#define COL_1 8
#define COL_2 10
#define COL_3 11

#define BUTTON_ENCODER 6

#define ROTARYMIN 0 // Quant MENUS MIN and MAX - total = 7
#define ROTARYMAX 6

// **********************************************
// ***************** FUNCTIONS ******************
void readKeyboardMatrix();      // Função para varrer o keyboard
void storeDigit(uint8_t value); // Função para armazenar o digit do keyboard
void numero();

// **********************************************
// ******************* CLASS ********************
RotaryEncoderButton encoder(A3, A2, BUTTON_ENCODER);

// **********************************************
// ***************** VARIABLES ******************
char sequence = 0x00; // Para varredura em sequencia do keyboard
char counter = 0x00;  // Variável de delay para ficar chamando a função de varredura do keyboard
int digit = 0x00;

// **********************************************
// **************** ISR TIMER2 ******************
ISR(TIMER2_OVF_vect)
{
  TCNT2 = 100; // Reiniciando o registrador Timer2
  counter++;

  if (counter == 0x05)
  {
    counter = 0x00;
    readKeyboardMatrix();
  }
}

void setup()
{
  Serial.begin(9600); // TODO: Remover
  Serial.println("Begin");

  pinMode(ROW_0, INPUT_PULLUP);
  pinMode(ROW_1, INPUT_PULLUP);
  pinMode(ROW_2, INPUT_PULLUP);
  pinMode(ROW_3, INPUT_PULLUP);

  pinMode(COL_0, OUTPUT);
  pinMode(COL_1, OUTPUT);
  pinMode(COL_2, OUTPUT);
  pinMode(COL_3, OUTPUT);

  digitalWrite(COL_0, HIGH);
  digitalWrite(COL_1, HIGH);
  digitalWrite(COL_2, HIGH);

  TCCR2A = 0x00; // Timer em modo normal
  TCCR2B = 0x07; // Prescaler 1:1024
  TCNT2 = 100;   // 10ms overflow
  TIMSK2 = 0x01; // Habilitando interrupção do Timer2

  encoder.setPosition(0);
}

void loop()
{
  encoder.tick();

  encoder.newPos = encoder.getPosition();
  if (encoder.newPos != encoder.oldPos)
  {
    encoder.oldPos = encoder.newPos;
    Serial.print("Position: ");
    Serial.println(encoder.newPos);
  }
}

// FUNCTIONS KEYBOARD MATRIX
void readKeyboardMatrix()
{
  if (digitalRead(COL_0) && sequence == 0x00)
  {
    sequence = 0x01;
    digitalWrite(COL_0, LOW);
    digitalWrite(COL_1, HIGH);
    digitalWrite(COL_2, HIGH);
    digitalWrite(COL_3, HIGH);

    if (!digitalRead(ROW_0))
      storeDigit(0x31); // 1
    else if (!digitalRead(ROW_1))
      storeDigit(0x34); // 4
    else if (!digitalRead(ROW_2))
      storeDigit(0x37); // 7
    else if (!digitalRead(ROW_3))
      storeDigit(0x30); // 0
  }

  else if (digitalRead(COL_1) && sequence == 0x01)
  {
    sequence = 0x02;
    digitalWrite(COL_0, HIGH);
    digitalWrite(COL_1, LOW);
    digitalWrite(COL_2, HIGH);
    digitalWrite(COL_3, HIGH);

    if (!digitalRead(ROW_0))
      storeDigit(0x32); // 2
    else if (!digitalRead(ROW_1))
      storeDigit(0x35); // 5
    else if (!digitalRead(ROW_2))
      storeDigit(0x38); // 8
    else if (!digitalRead(ROW_3))
      storeDigit(0x46); // F
  }

  else if (digitalRead(COL_2) && sequence == 0x02)
  {
    sequence = 0x03;
    digitalWrite(COL_0, HIGH);
    digitalWrite(COL_1, HIGH);
    digitalWrite(COL_2, LOW);
    digitalWrite(COL_3, HIGH);

    if (!digitalRead(ROW_0))
      storeDigit(0x33); // 3
    else if (!digitalRead(ROW_1))
      storeDigit(0x36); // 6
    else if (!digitalRead(ROW_2))
      storeDigit(0x39); // 9
    else if (!digitalRead(ROW_3))
      storeDigit(0x45); // E
  }

  else if (digitalRead(COL_3) && sequence == 0x03)
  {
    sequence = 0x00;
    digitalWrite(COL_0, HIGH);
    digitalWrite(COL_1, HIGH);
    digitalWrite(COL_2, HIGH);
    digitalWrite(COL_3, LOW);

    if (!digitalRead(ROW_0))
      storeDigit(0x41); // A
    else if (!digitalRead(ROW_1))
      storeDigit(0x42); // B
    else if (!digitalRead(ROW_2))
      storeDigit(0x43); // C
    else if (!digitalRead(ROW_3))
      storeDigit(0x44); // D
  }
}

void storeDigit(uint8_t value)
{
  TIMSK2 = 0x00; // Desativando interrupção Timer2
  delay(350);    // Tempo anti-rebond

  digit = value;
  Serial.println((char)value); // TODO: Remover

  TIMSK2 = 0x01; // Habilitando interrupção Timer2
}
