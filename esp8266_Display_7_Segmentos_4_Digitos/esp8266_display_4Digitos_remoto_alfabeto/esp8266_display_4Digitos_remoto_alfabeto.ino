#include <TM1637Display.h>

#define CLK D1
#define DIO D2

TM1637Display display(CLK, DIO);

// Mapeamento de A a Z para 7 segmentos
const uint8_t ALFABETO[26] = {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // A
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,                  // b
  SEG_A | SEG_D | SEG_E | SEG_F,                          // C
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,                  // d
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,                  // E
  SEG_A | SEG_E | SEG_F | SEG_G,                          // F
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F,                  // G
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,                  // H
  SEG_B | SEG_C,                                          // I
  SEG_B | SEG_C | SEG_D | SEG_E,                          // J
  SEG_A | SEG_C | SEG_E | SEG_F | SEG_G,                  // K (adaptado)
  SEG_D | SEG_E | SEG_F,                                  // L
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F,                  // M (adaptado)
  SEG_C | SEG_E | SEG_G,                                  // n
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,          // O
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,                  // P
  SEG_A | SEG_B | SEG_C | SEG_F | SEG_G,                  // q
  SEG_E | SEG_G,                                          // r
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                  // S
  SEG_D | SEG_E | SEG_F | SEG_G,                          // t
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,                  // U
  SEG_C | SEG_D | SEG_E,                                  // v
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,                  // W (adaptado)
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,                  // X (adaptado)
  SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,                  // y
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G                   // Z
};

void setup() {
  display.setBrightness(0x0a);
  display.clear();
}

void loop() {
  // Mostra cada letra de A a Z no primeiro dígito do display
  for (int i = 0; i < 26; i++) {
    // Exibe a letra no dígito 0 (primeiro pino da esquerda)
    display.setSegments(&ALFABETO[i], 1, 0); 
    delay(800); // Aguarda quase 1 segundo antes de mudar de letra
  }
}