#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> // Biblioteca para Hostname (mDNS)
#include <TM1637Display.h>

// 1. Configurações do Wi-Fi
const char* ssid     = "Sua Rede";
const char* password = "Sua Senha";

// 2. Configuração do Hostname (Acesse: http://meudisplay.local)
const char* hostName = "meudisplay";

// 3. Configuração de IP Fixo
IPAddress ip(192, 168, 15, 150);      // IP fixo desejado para o ESP
IPAddress gateway(192, 168, 15, 1);   // IP do seu Roteador
IPAddress subnet(255, 255, 255, 0);  // Máscara de rede padrão
IPAddress dns(192, 168, 1, 1);      // DNS

// Pinos do Display TM1637
#define CLK D1
#define DIO D2

TM1637Display display(CLK, DIO);
ESP8266WebServer server(80);

String textoAtual = "1234";
bool pontosAcesos = false;

// HTML Embutido com opção para os dois pontos
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Painel ESP8266 Display</title>
  <style>
    body { font-family: Arial, sans-serif; background: #121212; color: #fff; text-align: center; padding: 20px; }
    .card { background: #1e1e1e; padding: 30px; border-radius: 12px; display: inline-block; box-shadow: 0 4px 15px rgba(0,0,0,0.5); max-width: 350px; width: 100%; }
    h2 { color: #00e676; margin-bottom: 20px; }
    input[type="text"] { width: 80%; padding: 12px; font-size: 18px; border-radius: 6px; border: 1px solid #333; background: #2a2a2a; color: #fff; text-align: center; margin-bottom: 15px; }
    .checkbox-container { margin-bottom: 20px; font-size: 16px; cursor: pointer; display: block; }
    .checkbox-container input { width: 18px; height: 18px; vertical-align: middle; margin-right: 8px; }
    button { background: #00e676; color: #121212; border: none; padding: 12px 24px; font-size: 16px; font-weight: bold; border-radius: 6px; cursor: pointer; transition: 0.2s; }
    button:hover { background: #00b359; }
    .clear-btn { background: #ff5252; color: #fff; margin-left: 5px; }
    .clear-btn:hover { background: #d32f2f; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Display TM1637</h2>
    <form action="/set-display" method="POST">
      <input type="text" name="texto" maxlength="4" placeholder="Ex: 1234, HOJE, 25C" autocomplete="off">
      <br>
      <label class="checkbox-container">
        <input type="checkbox" name="pontos" value="1"> Acender dois pontos ( : )
      </label>
      <button type="submit">Enviar</button>
      <button type="button" class="clear-btn" onclick="limpar()">Limpar</button>
    </form>
  </div>
  <script>
    function limpar() {
      fetch('/set-display', { method: 'POST', headers: {'Content-Type': 'application/x-www-form-urlencoded'}, body: 'texto=' });
    }
  </script>
</body>
</html>
)rawliteral";

uint8_t charParaSegmento(char c) {
  switch (toupper(c)) {
    // --- NÚMEROS ---
    case '0': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
    case '1': return SEG_B | SEG_C;
    case '2': return SEG_A | SEG_B | SEG_D | SEG_E | SEG_G;
    case '3': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_G;
    case '4': return SEG_F | SEG_G | SEG_B | SEG_C;
    case '5': return SEG_A | SEG_F | SEG_G | SEG_C | SEG_D;
    case '6': return SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_G;
    case '7': return SEG_A | SEG_B | SEG_C;
    case '8': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
    case '9': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G;

    // --- ALFABETO COMPLETO (A-Z) ---
    case 'A': return SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G;
    case 'B': return SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
    case 'C': return SEG_A | SEG_D | SEG_E | SEG_F;
    case 'D': return SEG_B | SEG_C | SEG_D | SEG_E | SEG_G;
    case 'E': return SEG_A | SEG_D | SEG_E | SEG_F | SEG_G;
    case 'F': return SEG_A | SEG_E | SEG_F | SEG_G;
    case 'G': return SEG_A | SEG_C | SEG_D | SEG_E | SEG_F;
    case 'H': return SEG_B | SEG_C | SEG_E | SEG_F | SEG_G;
    case 'I': return SEG_B | SEG_C;
    case 'J': return SEG_B | SEG_C | SEG_D | SEG_E;
    case 'K': return SEG_A | SEG_C | SEG_E | SEG_F | SEG_G;
    case 'L': return SEG_D | SEG_E | SEG_F;
    case 'M': return SEG_A | SEG_B | SEG_C | SEG_E | SEG_F;
    case 'N': return SEG_C | SEG_E | SEG_G;
    case 'O': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
    case 'P': return SEG_A | SEG_B | SEG_E | SEG_F | SEG_G;
    case 'Q': return SEG_A | SEG_B | SEG_C | SEG_F | SEG_G;
    case 'R': return SEG_E | SEG_G;
    case 'S': return SEG_A | SEG_C | SEG_D | SEG_F | SEG_G;
    case 'T': return SEG_D | SEG_E | SEG_F | SEG_G;
    case 'U': return SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
    case 'V': return SEG_C | SEG_D | SEG_E;
    case 'W': return SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
    case 'X': return SEG_B | SEG_C | SEG_E | SEG_F | SEG_G;
    case 'Y': return SEG_B | SEG_C | SEG_D | SEG_F | SEG_G;
    case 'Z': return SEG_A | SEG_B | SEG_D | SEG_E | SEG_G;

    // --- SIMBOLOS E OUTROS ---
    case '-': return SEG_G;
    case '_': return SEG_D;
    case ' ': return 0x00;

    default: return SEG_G;
  }
}

void atualizarDisplay(String texto, bool ligarPontos) {
  uint8_t data[4] = {0, 0, 0, 0};
  int tam = texto.length();

  for (int i = 0; i < 4; i++) {
    if (i < tam) data[i] = charParaSegmento(texto[i]);
    else data[i] = 0x00;
  }

  // Ativa os dois pontos centrais no 2º dígito (data[1])
  if (ligarPontos) {
    data[1] |= 0x80; // Caso não acenda no seu módulo, mude 0x80 para 0x40
  }

  display.setSegments(data);
}

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleSetDisplay() {
  if (server.hasArg("texto")) {
    textoAtual = server.arg("texto");
    pontosAcesos = server.hasArg("pontos"); // Se a caixa estiver marcada, retorna true
    
    atualizarDisplay(textoAtual, pontosAcesos);
    
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Parametro incorreto.");
  }
}

void setup() {
  Serial.begin(115200);
  display.setBrightness(0x0a);
  display.clear();

  WiFi.hostname(hostName);

  if (!WiFi.config(ip, gateway, subnet, dns)) {
    Serial.println("Falha ao configurar IP Fixo!");
  }

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado com Sucesso!");

  if (MDNS.begin(hostName)) {
    Serial.print("Hostname: http://");
    Serial.print(hostName);
    Serial.println(".local");
  }

  Serial.print("IP Fixo: http://");
  Serial.println(WiFi.localIP());

  atualizarDisplay("ON  ", false);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/set-display", HTTP_POST, handleSetDisplay);

  server.begin();
  Serial.println("Servidor Web rodando!");
}

void loop() {
  MDNS.update();
  server.handleClient();
}