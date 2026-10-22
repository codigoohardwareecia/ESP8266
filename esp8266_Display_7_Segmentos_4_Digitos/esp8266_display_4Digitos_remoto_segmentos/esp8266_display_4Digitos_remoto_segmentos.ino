#include <TM1637Display.h>

#define CLK D1
#define DIO D2

TM1637Display display(CLK, DIO);

/*
       -- a --
      |       |
      f       b
      |       |
       -- g --
      |       |
      e       c
      |       |
       -- d -- 
*/

// Palavra "HOJE"
const uint8_t PALAVRA_HOJE[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // H
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_B | SEG_C | SEG_D | SEG_E,                  // J
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

// Palavra "ERRO"
const uint8_t PALAVRA_ERRO[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,  // E
  SEG_E | SEG_G,                          // r
  SEG_E | SEG_G,                          // r
  SEG_C | SEG_D | SEG_E | SEG_G           // o
};

void setup() {
  display.setBrightness(0x0a);
}

void loop() {
  // Exibe "HOJE" (envia o array de 4 caracteres na posição 0)
  display.setSegments(PALAVRA_HOJE, 4, 0);
  delay(2000);

  // Exibe "ERRO"
  display.setSegments(PALAVRA_ERRO, 4, 0);
  delay(2000);
}

