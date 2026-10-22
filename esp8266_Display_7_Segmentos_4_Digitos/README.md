# ESP8266 com Display de 7 segmentos e 4 digitos
## Links de Referencia

Módulo Tm1637 4 digitos
https://www.mercadolivre.com.br/modulo-tm1637-display-7-segmentos-4-digitos-arduino-diy-led/p/MLB36539817#polycard_client=search-desktop&float_highlight=last_units&be_origin=backend&overlay_label=not_apply&search_layout=grid&position=5&type=product&tracking_id=33dceac3-4eee-4c36-9893-8546e97a384b&wid=MLB4654781350&sid=search
### PASSO 1: Pré Requisitos

1. Módulo ESP8266 NODE MCU
2. Módulo Display de 4 Segmentos Módulo Tm1637
3. Cabo USB C
4. Arduino IDE
5. Fios com conectores femeas dos dois lados
### PASSO 1: Configurando a board

1. Abra o Arduino IDE
2. Clique em File > Preferences
3. Em additionla boards manager URLs cole a url abaixo e clique em Ok
http://arduino.esp8266.com/stable/package_esp8266com_index.json
4. Clique em Tools > Board Manager > e digite ESP8266
5. Procure "esp8266 by ESP8266 Community e clique "Install" 
6. Clique em Tools > Board Manager > ESP8266  e selecione NODE MCU 1.0 (ESP 12E Module)
7. Conecte seu ESP8266 na porta USB
8. Clique em Tools > Ports e procure a porta que seu ESP8266 esta conectado
9. Para tester use o modelo de código vazio e clique  em Upload, se tudo ocorrer bem com a comunicação o firmware será gravado com sucesso.

### PASSO 2 : Instalando as bibliotecas

1. Vamos precisar instalar a biblioteca "TM1637 by Avishay Orpaz", clique em Tools > Library Manager
2. Procure por "TM1637" selecione do autor by Avishay Orpaz e clique em "Install"

### PASSO 3 : Conexão do Display 

1. Conectar os 4 pinos do módulo Display TM1637 aos pinos do ESP8266
2. Pino GND do Display no pino G/GND do ESP8266
3. Pino VCC do Display no pino 3V do ESP8266
4. Pino CLK do Display no pino D1
5. Pino DIO do Display no pino D2

#### Exemplo de Contador Sequencial

```C
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
```

Valores para o brilho
```
display.setBrightness(0x00); //   0% (Desligado)
display.setBrightness(0x19); //  10%
display.setBrightness(0x33); //  20%
display.setBrightness(0x4C); //  30%
display.setBrightness(0x66); //  40%
display.setBrightness(0x7F); //  50% (Valor atual)
display.setBrightness(0x99); //  60%
display.setBrightness(0xB2); //  70%
display.setBrightness(0xCC); //  80%
display.setBrightness(0xE5); //  90%
display.setBrightness(0xFF); // 100% (Brilho máximo)
```
#### Exemplo de Relógio Wifi

```C
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TM1637Display.h>

// Configurações do Wi-Fi
const char* ssid     = "Sua Wifi";
const char* password = "Senha Wifi";

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
  display.setBrightness(0x0f); // Brilho médio/alto (0x00 a 0x0f)

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
```

### Exemplo de Caracters Hoje e Erro

Segmentos
```
       -- a --
      |       |
      f       b
      |       |
       -- g --
      |       |
      e       c
      |       |
       -- d -- 
```
```C
#include <TM1637Display.h>

#define CLK D1
#define DIO D2

TM1637Display display(CLK, DIO);

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
```

### Exemplo de Abcedário
```C
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
```

### Exemplo de Cobrinha
```C
#include <TM1637Display.h>

#define CLK D1
#define DIO D2

TM1637Display display(CLK, DIO);

struct Passo {
  uint8_t digito;   // Qual dígito (0 a 3)
  uint8_t segmento; // Qual segmento acender
};

// Sequência 100% contínua que faz um "8" sem sobressaltos
const Passo CAMINHO[] = {
  // 1. Topo (Esquerda -> Direita)
  {0, SEG_A}, {1, SEG_A}, {2, SEG_A}, {3, SEG_A},
  
  // 2. Transição no canto superior direito
  {3, SEG_B},
  
  // 3. Meio (Direita -> Esquerda)
  {3, SEG_G}, {2, SEG_G}, {1, SEG_G}, {0, SEG_G},
  
  // 4. Transição no canto inferior esquerdo
  {0, SEG_E},
  
  // 5. Fundo (Esquerda -> Direita)
  {0, SEG_D}, {1, SEG_D}, {2, SEG_D}, {3, SEG_D},
  
  // 6. Transição no canto inferior direito
  {3, SEG_C},
  
  // 7. Meio novamente (Direita -> Esquerda)
  {3, SEG_G}, {2, SEG_G}, {1, SEG_G}, {0, SEG_G},
  
  // 8. Transição no canto superior esquerdo (liga de volta no Topo!)
  {0, SEG_F}
};

const int TOTAL_PASSOS = sizeof(CAMINHO) / sizeof(CAMINHO[0]);

void setup() {
  display.setBrightness(0x0a);
  display.clear();
}

void loop() {
  int tamanhoCobra = 3; // Quantidade de segmentos no corpo da cobra

  for (int i = 0; i < TOTAL_PASSOS; i++) {
    uint8_t tela[4] = {0, 0, 0, 0};

    // Monta o corpo da cobra de forma suave
    for (int j = 0; j < tamanhoCobra; j++) {
      int indice = (i - j + TOTAL_PASSOS) % TOTAL_PASSOS;
      tela[CAMINHO[indice].digito] |= CAMINHO[indice].segmento;
    }

    display.setSegments(tela, 4, 0);
    delay(90); // Ajuste a velocidade do movimento aqui
  }
}
```

#### Exemplo Painel Remoto 
```C
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
```

#### Exemplo Windows Forms
1. Abra o Visual Studio 
2. Clique em Create new Project
3. Selecione Windows Forms App
4. Informe o nome do projeto, selecione o caminho e o nome da Solution
5. Clique em Next
6. Selecione .NET 8

##### Forms Design
7.  Abra a classe Form1.Designer.cs e cole os codigos substituindo tudo:

```CSharp
namespace ESP8266_Display_4_Segmentos
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            timer1 = new System.Windows.Forms.Timer(components);
            btnClock = new Button();
            lblContent = new Label();
            btnCrono = new Button();
            btnChronoStart = new Button();
            btnChronoStop = new Button();
            btnCountDow = new Button();
            btnCountdownPlay = new Button();
            btnCountdownStop = new Button();
            txtStartCountDown = new TextBox();
            btnText = new Button();
            txtDisplaytext = new TextBox();
            btnSetText = new Button();
            btnStartMarque = new Button();
            txtMarquee = new TextBox();
            btnMarque = new Button();
            SuspendLayout();
            // 
            // timer1
            // 
            timer1.Enabled = true;
            timer1.Interval = 1000;
            timer1.Tick += timer1_Tick;
            // 
            // btnClock
            // 
            btnClock.Location = new Point(245, 20);
            btnClock.Name = "btnClock";
            btnClock.Size = new Size(83, 23);
            btnClock.TabIndex = 0;
            btnClock.Text = "Relógio ";
            btnClock.UseVisualStyleBackColor = true;
            btnClock.Click += btnClock_Click;
            // 
            // lblContent
            // 
            lblContent.BackColor = Color.OliveDrab;
            lblContent.Font = new Font("Segoe UI", 36F, FontStyle.Regular, GraphicsUnit.Point, 0);
            lblContent.ForeColor = Color.White;
            lblContent.Location = new Point(12, 9);
            lblContent.Name = "lblContent";
            lblContent.Size = new Size(189, 74);
            lblContent.TabIndex = 1;
            lblContent.Text = "...";
            lblContent.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // btnCrono
            // 
            btnCrono.Location = new Point(245, 60);
            btnCrono.Name = "btnCrono";
            btnCrono.Size = new Size(83, 23);
            btnCrono.TabIndex = 2;
            btnCrono.Text = "Cronometro";
            btnCrono.UseVisualStyleBackColor = true;
            btnCrono.Click += btnCrono_Click;
            // 
            // btnChronoStart
            // 
            btnChronoStart.Location = new Point(337, 61);
            btnChronoStart.Name = "btnChronoStart";
            btnChronoStart.Size = new Size(36, 23);
            btnChronoStart.TabIndex = 3;
            btnChronoStart.Text = ">";
            btnChronoStart.UseVisualStyleBackColor = true;
            btnChronoStart.Click += btnChronoStart_Click;
            // 
            // btnChronoStop
            // 
            btnChronoStop.Location = new Point(379, 61);
            btnChronoStop.Name = "btnChronoStop";
            btnChronoStop.Size = new Size(36, 23);
            btnChronoStop.TabIndex = 4;
            btnChronoStop.Text = "[   ]";
            btnChronoStop.UseVisualStyleBackColor = true;
            btnChronoStop.Click += btnChronoStop_Click;
            // 
            // btnCountDow
            // 
            btnCountDow.Location = new Point(245, 98);
            btnCountDow.Name = "btnCountDow";
            btnCountDow.Size = new Size(83, 23);
            btnCountDow.TabIndex = 5;
            btnCountDow.Text = "Countdown";
            btnCountDow.UseVisualStyleBackColor = true;
            btnCountDow.Click += btnCountDow_Click;
            // 
            // btnCountdownPlay
            // 
            btnCountdownPlay.Location = new Point(337, 98);
            btnCountdownPlay.Name = "btnCountdownPlay";
            btnCountdownPlay.Size = new Size(36, 23);
            btnCountdownPlay.TabIndex = 6;
            btnCountdownPlay.Text = ">";
            btnCountdownPlay.UseVisualStyleBackColor = true;
            btnCountdownPlay.Click += btnCountdownPlay_Click;
            // 
            // btnCountdownStop
            // 
            btnCountdownStop.Location = new Point(379, 98);
            btnCountdownStop.Name = "btnCountdownStop";
            btnCountdownStop.Size = new Size(36, 23);
            btnCountdownStop.TabIndex = 7;
            btnCountdownStop.Text = "[   ]";
            btnCountdownStop.UseVisualStyleBackColor = true;
            btnCountdownStop.Click += btnCountdownStop_Click;
            // 
            // txtStartCountDown
            // 
            txtStartCountDown.Location = new Point(421, 98);
            txtStartCountDown.Name = "txtStartCountDown";
            txtStartCountDown.Size = new Size(42, 23);
            txtStartCountDown.TabIndex = 8;
            // 
            // btnText
            // 
            btnText.Location = new Point(245, 136);
            btnText.Name = "btnText";
            btnText.Size = new Size(83, 23);
            btnText.TabIndex = 9;
            btnText.Text = "Text";
            btnText.UseVisualStyleBackColor = true;
            btnText.Click += btnText_Click;
            // 
            // txtDisplaytext
            // 
            txtDisplaytext.Location = new Point(337, 136);
            txtDisplaytext.MaxLength = 4;
            txtDisplaytext.Name = "txtDisplaytext";
            txtDisplaytext.Size = new Size(84, 23);
            txtDisplaytext.TabIndex = 10;
            // 
            // btnSetText
            // 
            btnSetText.Location = new Point(425, 135);
            btnSetText.Name = "btnSetText";
            btnSetText.Size = new Size(36, 23);
            btnSetText.TabIndex = 11;
            btnSetText.Text = ">";
            btnSetText.UseVisualStyleBackColor = true;
            btnSetText.Click += btnSetText_Click;
            // 
            // btnStartMarque
            // 
            btnStartMarque.Location = new Point(425, 164);
            btnStartMarque.Name = "btnStartMarque";
            btnStartMarque.Size = new Size(36, 23);
            btnStartMarque.TabIndex = 14;
            btnStartMarque.Text = ">";
            btnStartMarque.UseVisualStyleBackColor = true;
            btnStartMarque.Click += btnStartMarque_Click;
            // 
            // txtMarquee
            // 
            txtMarquee.Location = new Point(337, 165);
            txtMarquee.MaxLength = 50;
            txtMarquee.Name = "txtMarquee";
            txtMarquee.Size = new Size(84, 23);
            txtMarquee.TabIndex = 13;
            // 
            // btnMarque
            // 
            btnMarque.Location = new Point(245, 165);
            btnMarque.Name = "btnMarque";
            btnMarque.Size = new Size(83, 23);
            btnMarque.TabIndex = 12;
            btnMarque.Text = "Marquee";
            btnMarque.UseVisualStyleBackColor = true;
            btnMarque.Click += btnMarque_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(473, 215);
            Controls.Add(btnStartMarque);
            Controls.Add(txtMarquee);
            Controls.Add(btnMarque);
            Controls.Add(btnSetText);
            Controls.Add(txtDisplaytext);
            Controls.Add(btnText);
            Controls.Add(txtStartCountDown);
            Controls.Add(btnCountdownStop);
            Controls.Add(btnCountdownPlay);
            Controls.Add(btnCountDow);
            Controls.Add(btnChronoStop);
            Controls.Add(btnChronoStart);
            Controls.Add(btnCrono);
            Controls.Add(lblContent);
            Controls.Add(btnClock);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private Button btnClock;
        private Label lblContent;
        private Button btnCrono;
        private Button btnChronoStart;
        private Button btnChronoStop;
        private Button btnCountDow;
        private Button btnCountdownPlay;
        private Button btnCountdownStop;
        private TextBox txtStartCountDown;
        private Button btnText;
        private TextBox txtDisplaytext;
        private Button btnSetText;
        private Button btnStartMarque;
        private TextBox txtMarquee;
        private Button btnMarque;
    }
}
```
##### Forms Codigo

7.  Clique com o botão direito em Form1 e selecione ViewCode, será aberto Form1.cs e cole os codigos abaixo substituindo tudo:
```CSharp
namespace ESP8266_Display_4_Segmentos
{
    public partial class Form1 : Form
    {
        DisplayService _displayService;

        public Form1()
        {
            InitializeComponent();
            _displayService = new DisplayService("http://192.168.15.150/set-display");
            _displayService.SetContext(Context.None);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void btnClock_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Clock);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lblContent.Text = _displayService.getContentESP();
        }

        private void btnCrono_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Cronometer);
        }

        private void btnChronoStart_Click(object sender, EventArgs e)
        {
            _displayService.StartChronometer();
        }

        private void btnChronoStop_Click(object sender, EventArgs e)
        {
            _displayService.StopChronometer();
        }

        private void btnCountDow_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Countdown);
        }

        private void btnCountdownPlay_Click(object sender, EventArgs e)
        {
            _displayService.SetCountdown(txtStartCountDown.Text);
            _displayService.StartCountDown();
        }

        private void btnCountdownStop_Click(object sender, EventArgs e)
        {
            _displayService.StopCountdown();
        }

        private void btnSetText_Click(object sender, EventArgs e)
        {
            _displayService.SetText(txtDisplaytext.Text);
        }

        private void btnText_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Text);

        }

        private void btnMarque_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Marquee);
        }

        private void btnStartMarque_Click(object sender, EventArgs e)
        {
            _displayService.SetMarquee(txtMarquee.Text);
        }
    }
}
```

##### Display Service

Clique com o botão direito sobre o nome do projeto e selecione Add > Class
Informe o nome de  DisplayService.cs
Substitua o codigo dessa classe pelo codigo abaixo:

```CSharp
using System.Diagnostics;
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.AxHost;

namespace ESP8266_Display_4_Segmentos
{
    public class DisplayService
    {
        private static readonly HttpClient _client = new HttpClient();
        private string _url;
        private Context _context = Context.None;
        private readonly System.Threading.Timer _senderData;
        private readonly System.Threading.Timer _updaterContext;
        private readonly System.Threading.Timer _clockTimer;
        private readonly System.Threading.Timer _cronometroTimer;
        private readonly System.Threading.Timer _countDownTimer;
        private readonly Stopwatch _stopwatchChronometer = new Stopwatch();
        private readonly Stopwatch _stopwatchCountdown = new Stopwatch();
        private string _rawContent = string.Empty;
        private string _espContent = string.Empty;
        private Dictionary<string, string> _postData;
        private bool _blinkPoints = false;
        private bool _isConnected = false;
        private TimeSpan _elapsetTimer;
        private TimeSpan _startTime;

        public DisplayService(string url)
        {
            _isConnected = false;
            _url = url;
            _rawContent = string.Empty;
            _espContent = string.Empty;
            _postData = new Dictionary<string, string>();
            _senderData = new System.Threading.Timer(SendToESP, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
            _clockTimer = new System.Threading.Timer(UpdateClockTimer, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
            _cronometroTimer = new System.Threading.Timer(UpdateChronometer, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
            _countDownTimer = new System.Threading.Timer(UpdateCountdown, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
        }

        public void SetContext(Context context)
        {
            _context = context;
            _rawContent = string.Empty;
            _espContent = string.Empty;

            if (_context == Context.None)
            {
                _senderData?.Change(TimeSpan.FromSeconds(1), Timeout.InfiniteTimeSpan);
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                return;
            }

            if (_context == Context.Clock)
            {
                _senderData?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _clockTimer?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = true;
                return;
            }

            if (_context == Context.Cronometer)
            {
                _senderData?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = true;
                return;
            }


            if (_context == Context.Countdown)
            {
                _senderData?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _blinkPoints = true;
                return;
            }

            if(_context == Context.Text)
            {
                _senderData?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = false;
                return;
            }


            if (_context == Context.Marquee)
            {
                _senderData?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = false;
                return;
            }
        }

        public string getContentESP()
        {
            return _espContent;
        }

        public string getRawContent()
        {
            return _rawContent;
        }

        public void StartChronometer()
        {
            if (_context == Context.Cronometer)
                _stopwatchChronometer.Start();
        }

        public void StopChronometer()
        {
            if (_context == Context.Cronometer)
                _stopwatchChronometer.Stop();
        }

        public void StartCountDown()
        {
            if (_context == Context.Countdown)
                _stopwatchCountdown.Start();
        }

        public void SetCountdown(string time)
        {
            if (_context == Context.Countdown)
            {
                if (int.TryParse(time, out int valorDigitado))
                {
                    _startTime = TimeSpan.FromSeconds(valorDigitado); ;
                }
            }
        }

        public void StopCountdown()
        {
            if (_context == Context.Countdown)
                _stopwatchCountdown.Stop();
        }

        private async void UpdateClockTimer(object? state)
        {
            _espContent = DateTime.Now.ToString("HHmm");
            _rawContent = DateTime.Now.ToString("HH:mm:ss");
            _blinkPoints = (DateTime.Now.Second % 2 == 0);
        }


        private async void UpdateChronometer(object? state)
        {
            _elapsetTimer = _stopwatchChronometer.Elapsed;
            _rawContent = _elapsetTimer.ToString(@"hh\:mm\:ss");
            _espContent = _elapsetTimer.ToString(@"mmss");
        }

        private async void UpdateCountdown(object? state)
        {
            if (_stopwatchCountdown.IsRunning)
            {
                TimeSpan tempoRestante = _startTime - _stopwatchCountdown.Elapsed;

                if (tempoRestante <= TimeSpan.Zero)
                {
                    _stopwatchCountdown.Reset();
                    _stopwatchCountdown.Stop();
                }

                _rawContent = tempoRestante.ToString(@"hh\:mm\:ss");
                _espContent = tempoRestante.ToString(@"mmss");
            }

        }

        public void SetText(string text)
        {
            if (_context == Context.Text)
            {
                _espContent = text;
                SendToESP(null);
            }
  
        }

        public async void SetMarquee(string text)
        {
            if (_context == Context.Marquee)
            {
                await DisplayMarqueeAsync(text);
            }

        }

        public async Task DisplayMarqueeAsync(string frase, int velocidadeMs = 400)
        {
            if (string.IsNullOrWhiteSpace(frase))
                return;

            // Adiciona 4 espaços no começo e no fim para a frase entrar e sair "lisinha" do display
            string fraseFormatada = "    " + frase + "    ";

            // Calcula até onde o índice pode ir para sempre conseguir pegar 4 caracteres
            int limite = fraseFormatada.Length - 4;

            for (int i = 0; i <= limite; i++)
            {
                // 1. Recorta 4 caracteres a partir da posição atual e atribui à sua variável
                _espContent = fraseFormatada.Substring(i, 4);

                // 2. Chame o envio diretamente (passando null por conta da assinatura do Timer)
                SendToESP(null);

                // 3. Aguarda o tempo necessário antes de dar o próximo "passo" no texto
                await Task.Delay(velocidadeMs);
            }
        }

        private async void SendToESP(object? state)
        {
            try
            {
                _postData.Clear();
                _postData.Add("texto", _espContent);

                if(_blinkPoints)
                    _postData.Add("pontos", "1");

                HttpResponseMessage response = await _client.PostAsync(_url, new FormUrlEncodedContent(_postData));

                if (response.IsSuccessStatusCode)
                    _isConnected = true;
                else
                    _isConnected = false ;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("Erro: " + ex.Message);
            }
            finally
            {
                // Se estiver usando o padrão para evitar requisições sobrepostas:
                _senderData?.Change(TimeSpan.FromSeconds(1), Timeout.InfiniteTimeSpan);
            }
        }
        public void Dispose()
        {
            _senderData?.Dispose();
        }

    }

    public enum Context
    {
        None = 0,
        Clock = 1,      
        Cronometer = 2, 
        Countdown = 3,
        Text = 4,
        Marquee = 5
    }
}

```

Salve e execute o projeto
