#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

ESP8266WiFiMulti wifiMulti;

// ====================================================================
// 1. CONFIGURAÇÕES DE REDE E SENHA
// ====================================================================
const char* password_comum = "SENHA DO WIFI"; // Sua senha do Wi-Fi

// Nome da placa na rede local (Acesse via http://painel-lcd.local)
const char* hostname = "painel-lcd";

// Configurações de IP Fixo
IPAddress local_IP(192, 168, 15, 150);
IPAddress gateway(192, 168, 15, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

// ====================================================================
// INSTÂNCIAS
// ====================================================================
LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP8266WebServer server(80);

// Função auxiliar para atualizar as duas linhas da tela de uma vez
void atualizarLCD(String linha1, String linha2) {
  lcd.clear();

  // Escreve a primeira linha (preenche com espaços se tiver menos de 16 chars)
  lcd.setCursor(0, 0);
  lcd.print(linha1);

  // Escreve a segunda linha
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}

// Interface Web (Página HTML com 2 campos)
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1' charset='UTF-8'>";
  html += "<title>Painel LCD ESP8266</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 40px; background-color: #f4f4f9; }";
  html += ".card { background: white; padding: 30px; border-radius: 10px; display: inline-block; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 85%; max-width: 400px; }";
  html += "label { font-weight: bold; display: block; text-align: left; margin-top: 10px; color: #333; }";
  html += "input[type=text] { width: 100%; padding: 10px; margin: 5px 0 15px 0; border: 1px solid #ccc; border-radius: 5px; font-size: 16px; box-sizing: border-box; }";
  html += "input[type=submit] { background-color: #008CBA; color: white; padding: 12px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; width: 100%; font-weight: bold; }";
  html += "input[type=submit]:hover { background-color: #005f73; }";
  html += "</style></head><body>";
  html += "<div class='card'>";
  html += "<h2>Controle do LCD 16x2</h2>";
  html += "<form action='/print' method='POST'>";
  
  html += "<label>Linha 1 (Superior):</label>";
  html += "<input type='text' name='linha1' maxlength='16' placeholder='Ate 16 caracteres...'><br>";
  
  html += "<label>Linha 2 (Inferior):</label>";
  html += "<input type='text' name='linha2' maxlength='16' placeholder='Ate 16 caracteres...'><br>";
  
  html += "<input type='submit' value='Enviar para a Tela'>";
  html += "</form>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

// Processa o envio dos dados via Formulário ou API POST
void handlePost() {
  String txtLinha1 = "";
  String txtLinha2 = "";

  // Se o envio vier do formulário Web (com os dois campos)
  if (server.hasArg("linha1") || server.hasArg("linha2")) {
    txtLinha1 = server.arg("linha1");
    txtLinha2 = server.arg("linha2");
  } 
  // Se o envio vier via cURL / Raw Post simples
  else if (server.hasArg("plain")) {
    String corpo = server.arg("plain");
    if (corpo.length() <= 16) {
      txtLinha1 = corpo;
    } else {
      txtLinha1 = corpo.substring(0, 16);
      txtLinha2 = corpo.substring(16, 32);
    }
  } 
  else {
    server.send(400, "text/plain", "Dados ausentes!");
    return;
  }

  // Atualiza o visor estaticamente
  atualizarLCD(txtLinha1, txtLinha2);

  // Redireciona de volta para a página principal
  if (server.hasArg("linha1") || server.hasArg("linha2")) {
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(200, "text/plain", "Texto exibido no LCD com sucesso!");
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(D2, D1); 
  
  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Conectando...");

  WiFi.hostname(hostname);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("Falha ao configurar IP Fixo!");
  }

  wifiMulti.addAP("Rede1", password_comum);
  wifiMulti.addAP("Rede2", password_comum);

  Serial.print("Conectando ao Wi-Fi...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");

  if (MDNS.begin(hostname)) {
    Serial.println("mDNS ativo!");
  }

  // Exibe o SSID e IP ao inicializar
  atualizarLCD(WiFi.SSID().substring(0, 16), WiFi.localIP().toString());

  // Rotas do Servidor Web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/print", HTTP_POST, handlePost);

  server.begin();
  Serial.println("Servidor Web escutando...");
}

void loop() {
  if (wifiMulti.run() == WL_CONNECTED) {
    MDNS.update();
    server.handleClient();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reconectando...");
    delay(1000);
  }
}