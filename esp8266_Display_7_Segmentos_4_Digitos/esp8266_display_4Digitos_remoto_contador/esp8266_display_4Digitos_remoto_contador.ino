#include <Arduino.h>
#include <TM1637Display.h>

// Pinos conectados ao ESP8266
#define CLK D1
#define DIO D2

// Inicializa a biblioteca com os pinos definidos
TM1637Display display(CLK, DIO);

void setup() {
  // Ajusta o brilho do display (0 = mínimo, 7 = máximo)
  display.setBrightness(0x7f); 
}

void loop() {
  // Exemplo 1: Mostra o número 1234
  display.showNumberDec(1234, false);
  delay(2000);

  // Exemplo 2: Contador simples de 0 a 10
  for (int i = 0; i <= 100; i++) {
    display.showNumberDec(i, true); // true coloca zeros à esquerda se necessário
    delay(1000);
  }

  // Exemplo 3: Apaga a tela
  display.clear();
  delay(1000);
}