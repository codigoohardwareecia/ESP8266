# Modulo Display LCD no ESP8266

Os links a seguir são apenas links de referência para ajudar

Módulo Displya LCD
https://www.mercadolivre.com.br/display-lcd-16x2-modulo-i2c-soldado-verde-hd44780-arduino/p/MLB36736922?pdp_filters=item_id:MLB4670064634#is_advertising=true&searchVariation=MLB36736922&backend_model=search-backend;fallback_productos-promocionados_supple_view_view&be_origin=backend&position=1&search_layout=grid&type=pad&tracking_id=c699f4ea-464b-483e-b0f4-91e1ae35dbf3&ad_domain=VQCATCORE_LST&ad_position=1&ad_click_id=OTBlMzk4MjAtZTdmMy00NmFkLTg1MzYtNjI1ZDFhYjliNjlj

Módulo I2C
https://www.mercadolivre.com.br/4x-modulo-conversor-i2c-para-display-lcd-16x02-20x04/p/MLB65062367?pdp_filters=item_id:MLB6209961514#is_advertising=true&backend_model=search-backend&be_origin=backend&position=2&search_layout=grid&type=pad&tracking_id=c699f4ea-464b-483e-b0f4-91e1ae35dbf3&ad_domain=VQCATCORE_LST&ad_position=2&ad_click_id=N2Q2YjdjM2EtODFhMi00YTcyLWIyOGEtNGQ1NTJiODJmZDdj

### PASSO 1: Pré Requisitos

1. Módulo ESP8266 NODE MCU
2. Módulo Display LCD 16x2 16 pinos HD44780
3. Módulo I2C para LCD 16x2 16 pinos PCF8574
4. Cabo USB C
5. Arduino IDE
6. Fios com conectoresa femeas dos dois lados
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

1. Vamos precisar instalar a biblioteca "LiquidCrystal I2C", clique em Tools > Library Manager
2. Procure por "LiquidCrystal I2C" selecione do autor by Frank de Brabander e clique em "Install"

### PASSO 3 : Conexão do Display LCD

1. O módulo IDC deve ser solado nos pinos do Display LCD
2. Conectar os 4 pinos do móduolo I2C aos pinos do ESP8266
3. Pino GND do I2C no pino GND do ESP8266
4. Pino VCC do I2C no pino VV do ESP8266
5. Pino SDA do I2C no pino D2
6. Pino SCL do I2C no pino D1

### PASSO 4 : Detectando o endereço do módulo I2C

1. Copie e cole o código abaixo no Arduino IDE

```C
#include <Wire.h>

void setup() {
  Wire.begin(D2, D1); // SDA = D2, SCL = D1
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- Escaneando Barramento I2C ---");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado no endereco 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
  }

  if (nDevices == 0) Serial.println("Nenhum dispositivo I2C encontrado. Verifique as conexoes (SDA/SCL)!");
  else Serial.println("Fim do escaneamento.\n");

  delay(5000);
}
```

2. Faça Upload do código
3. Clique no icone que está no lado direito do Arduino IDE chamado Monitor Serial
4. Reinicie o ESP8266 (tire o cabo e conecte de novo)
5. Se os pinos foram conectados corretamente ele vai retorna o endereco
6. Anote o endereço vamos usar no código de teste a seguir

### PASSO 5 : Testando o Display de LCD

1. Copie e cole o código a seguir:
2. Na linha "LiquidCrystal_I2C lcd(0x27, 16, 2);" onde aparece 0x27 verifique se é o mesmo endereço que apareceu na execução do código anterior. Em alguns casos o Display LCD pode ter o endreço diferente.

```C
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
```
3. Faça o Upload do código e quando o ESP8266 reiniciar verique se apareceu a mensagem "FUNCIONOU".

4. Se não apareceu atrás do módulo I2C tem um potenciometro, talvez vc vai precisar regular ele para que ele exiba corretamente a luminozidade dos caracteres, use uma chave de fenda pequena e mova o cursor do potenciometro

### PASSO 6 : Código final

1.  O código a seguir  usa a conexão Wifi para  disponiblizar um servidor web que possui uma pagina com conteudo html, verifique a senha do Wifi no código e logo abaixo o endereçameno de IP Fixo e coloque o IP da sua rede. O código também pode receber uma ou mais redes Wifi, altere o nome da rede e a senha caso seja senha diferente para cada roteados.

2. Ele vai tentar se conectar no primeiro, caso não consiga vai para o segundo.
```
	wifiMulti.addAP("Rede1", password_comum);
	wifiMulti.addAP("Rede2", password_comum);
```
3. Copie e cole o código no editor do Arduino IDE e faça o Upload
```C
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
const char* password_comum = "SUA SENHA"; // Sua senha do Wi-Fi

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
```

4. Para testar abra o navegador e cole o endereço "http://painel-lcd.local"
5. Será aberto um formulário com dois campos
6. Digite qualquer coisa nesses campos e clique no botão Enviar
7. O LCD deverá exibir a mensagem digitada

### PASSO 7 : Automatizando as mensagens

1. Abra o Visual Studio 
2. Clique em Create new Project
3. Selecione Windows Forms App
4. Informe o nome do projeto, selecione o caminho e o nome da Solution
5. Clique em Next
6. Selecione .NET 8
7. Ao abrir o formulário arraste um controle do tipo Timer da Toolbox
8. Configura as propriedades Interval do timer para 1000 e Enable para true
9. Copie e cole o código a seguir
10. Clique em Play ou pressione F5
11. Os dados do Display irão ser atualizados com a data e hora correntes a cada um segundo

```CSharp
namespace ESP8266_DIsplay_LCD_Desktop
{
    public partial class Form1 : Form
    {
        private static readonly HttpClient httpClient = new HttpClient();

        public Form1()
        {
            InitializeComponent();
        }

        private async Task SendDataToDevice(string l1, string l2)
        {
            var formData = new Dictionary<string, string>
            {
                { "linha1", l1 },
                { "linha2", l2 }
            };

            var content = new FormUrlEncodedContent(formData);

            HttpResponseMessage response = await httpClient.PostAsync("http://painel-lcd.local/print", content);
        }

        private async void timer1_Tick(object sender, EventArgs e)
        {
            await SendDataToDevice(DateTime.Now.Date.ToShortDateString(), DateTime.Now.ToLongTimeString());
        }
    }
}
```
