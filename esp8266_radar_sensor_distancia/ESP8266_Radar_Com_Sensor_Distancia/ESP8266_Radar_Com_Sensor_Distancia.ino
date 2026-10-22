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