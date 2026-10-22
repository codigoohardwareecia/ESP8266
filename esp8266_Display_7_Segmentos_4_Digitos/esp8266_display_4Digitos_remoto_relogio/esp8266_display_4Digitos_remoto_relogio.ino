#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TM1637Display.h>

// Configurações do Wi-Fi
const char* ssid     = "Sua Rede";
const char* password = "Sua Senha";

// Pinos do Display
#define CLK D1
#define DIO D2

TM1637Display display(CLK, DIO);

// Configuração do servidor de hora (NTP)
WiFiUDP ntpUDP;
// Fuso horário do Brasil (UTC-3) -> -3 horas * 3600 segundos = -10800
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", -10800, 60000);

void setup() {
  Serial.begin(115200);

  // Inicializa o display
  display.setBrightness(0x7F); // Brilho médio/alto (0x00 a 0x0f)

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado com sucesso!");

  timeClient.begin();
}

void loop() {
  // Atualiza o horário via internet
  timeClient.update();

  int horas = timeClient.getHours();
  int minutos = timeClient.getMinutes();

  // Prepara o valor para o display (ex: 14h30 vira 1430)
  int tempoExibicao = (horas * 100) + minutos;

  // Efeito de piscar os dois pontos ":" a cada segundo
  // Segundos pares: acende os dois pontos (0x40)
  // Segundos ímpares: apaga os dois pontos (0x00)
  uint8_t pontoCentral = (timeClient.getSeconds() % 2 == 0) ? 0x40 : 0x00;

  // Exibe a hora formatada
  display.showNumberDecEx(tempoExibicao, pontoCentral, true);

  delay(100); 
}