#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Endereço 0x27 confirmado pelo scanner!
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Inicializa I2C nos pinos D2 (SDA) e D1 (SCL)
  Wire.begin(D2, D1);
  
  lcd.init();          // Inicializa o LCD
  lcd.clear();         // Limpa a tela
  lcd.backlight();     // Liga a luz de fundo
  
  // Escreve na Linha 1
  lcd.setCursor(0, 0);
  lcd.print("FUNCIONOU!");

  // Escreve na Linha 2
  lcd.setCursor(0, 1);
  lcd.print("ESP8266 + I2C");
}

void loop() {
  // Nada aqui, apenas mantem o texto na tela
}
