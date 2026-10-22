
## ESP8266 - Radar com sensor de distância
## 🛠️ Como configurar o ambiente no Arduino IDE

### 1. Adicionar o suporte ao ESP8266

1. Abra o **Arduino IDE**.
    
2. Vá em **Arquivo** > **Preferências** (no macOS: **Arduino IDE** > **Settings**).
    
3. No campo **URLs Adicionais para o Gerenciador de Placas**, cole o seguinte link:
    
    Plaintext
    
    ```
    https://arduino.esp8266.com/stable/package_esp8266com_index.json
    ```
    
    _(Caso já existam outras URLs no campo, separe-as utilizando vírgula)._
    
4. Clique em **OK** para salvar.
    

### 2. Instalar a placa ESP8266

1. Acesse **Ferramentas** > **Placa** > **Gerenciador de Placas...** (ou clique no ícone de placas na barra lateral).
    
2. Busque por `esp8266` e instale o pacote oficial mantido pela **ESP8266 Community**.
    

### 3. Selecionar a placa e a porta

1. Vá em **Ferramentas** > **Placa** > **esp8266** e selecione o modelo **NodeMCU 1.0 (ESP-12E Module)**.
    
2. Conecte sua placa ao computador via cabo USB.
    
3. Vá em **Ferramentas** > **Porta** e selecione a porta COM correspondente à sua placa.
    

> **Dica de validação:** Para testar a conexão e garantir que o upload está funcionando, carregue um sketch básico com os métodos `setup()` e `loop()` vazios.

### 4. Instalar a biblioteca necessária

1. Vá em **Ferramentas** > **Gerenciador de Bibliotecas...** (ou pressione `Ctrl + Shift + I` / `Cmd + Shift + I`).
    
2. Digite `ArduinoJson` na barra de pesquisa.
    
3. Localize a biblioteca desenvolvida por **Benoit Blanchon** e clique em **Instalar**.

   
### Implmentação

```C++
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Usuário e senha do seu Wifi
const char* ssid = "Archenar";
const char* password = "PASS1234567890000";

// Pinos utilizados para conectar a placa do sensor ao ESP8266
const int TRIG_PIN = D2;
const int ECHO_PIN = D3;

// Instância do servidor web
ESP8266WebServer server(80);

// Configuração de rede fixa
IPAddress local_IP(192, 168, 15, 92);
IPAddress gateway(192, 168, 15, 1);
IPAddress subnet(255, 255, 255, 0);

String jsonOutput;

// Variáveis para calcular dados do sensor
long lastDistance = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);

  // configura os pinos com input e outiput
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configura e inicializa Wifi
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  
  // Aguarda conexão Wifi
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
  }
  Serial.println("\nConectado!");

  // Cria a rota Data no servidor web para retornar o Json
  server.on("/data", []() {
    serializeJson(getSensorJson(), jsonOutput);
    server.send(200, "application/json", jsonOutput);
  });

  server.begin();
}

DynamicJsonDocument getSensorJson() {
  DynamicJsonDocument doc(256);
  
  // Executa o disparo do trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Timeout de 30ms (equivalente a ~5 metros, dentro do limite do HC-SR04)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  long currentDist = (duration > 0) ? (duration * 0.034 / 2) : 0;

  // BLINDAGEM: Verifica se a leitura é fisicamente possível (ex: 2cm a 400cm)
  bool isDataValid = (currentDist >= 2 && currentDist <= 400);

  // Calcula valores de telemetria apenas se o dado for válido
  unsigned long currentTime = millis();
  float velocity = 0;
  
  if (isDataValid) {
    if (lastTime > 0) {
      velocity = (float)(currentDist - lastDistance) / (currentTime - lastTime);
    }
    lastDistance = currentDist;
    lastTime = currentTime;
  } else {
    // Valores default em caso de erro
    currentDist = 0;
    velocity = 0;
  }

  // Monta o JSON
  doc["distancia"] = currentDist;
  doc["velocidade"] = velocity;
  doc["status"] = isDataValid ? "Success" : "Sensor_Error";
  doc["timestamp"] = currentTime;

  return doc;
}

void loop() {
  server.handleClient();
}
```

## Visualizando Dados

Para visualizar os dados abra o navegador e digite o endereço IP fixo que foi informado no codigo 
da seguinte forma:

http://192.168.15.92/data

C# .NET

```CSharp
using System.Text.Json;

namespace RadarEsp8266
{
    public partial class Form1 : Form
    {
        private float angle = 0;
        private HttpClient client = new HttpClient();
        private Bitmap radarBmp;
        private System.Windows.Forms.Timer timer;

        public Form1()
        {
            InitializeComponent();
            this.Text = "Radar Pro - Jarvis";

            radarBmp = new Bitmap(this.Width, this.Height);
            timer = new System.Windows.Forms.Timer { Interval = 10 };
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private async void Timer_Tick(object sender, EventArgs e)
        {
            try
            {
                string json = await client.GetStringAsync("http://192.168.15.92/data");
                var data = JsonSerializer.Deserialize<RadarData>(json);
                DrawRadar(data.distancia);
            }
            catch {  }
        }
8
        private void DrawRadar(float dist)
        {
            using (Graphics g = Graphics.FromImage(radarBmp))
            {
                g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias; // Esfumaçado
                g.FillRectangle(new SolidBrush(Color.FromArgb(20, 0, 0, 0)), 0, 0, 500, 500); // Rastro
                g.DrawEllipse(new Pen(Color.FromArgb(50, Color.LimeGreen)), 50, 50, 400, 400);   // Desenhar grade

                float rad = angle * (float)Math.PI / 180;
                float x = 250 + (dist * 1.2f) * (float)Math.Cos(rad);
                float y = 250 + (dist * 1.2f) * (float)Math.Sin(rad);

                g.FillEllipse(Brushes.LimeGreen, x - 3, y - 3, 6, 6);
                g.DrawLine(new Pen(Color.LimeGreen, 2), 250, 250, 250 + 200 * (float)Math.Cos(rad), 250 + 200 * (float)Math.Sin(rad));  // Linha de varredura
            }

            angle = (angle + 5) % 360;

            pictureBox1.Image = radarBmp;
        }

        public class RadarData
        {
            public float distancia { get; set; }
            public string status { get; set; }
        }
    }
}

```

## Links

```
Link do Mercado Livre

Módulo Sensor Ultrassônico Impermeável Jsn-sr04t/aj-sr04m
```
https://www.mercadolivre.com.br/modulo-sensor-ultrassnico-impermeavel-jsn-sr04taj-sr04m/p/MLB42966766?pdp_filters=item_id:MLB3960236289
```

