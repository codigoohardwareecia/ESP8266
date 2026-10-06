# Primeiros Passos com ESP8266 na Arduino IDE

Este guia rápido faz parte do canal **Código Hardware e Cia** e resume o passo a passo técnico para configurar o ambiente e rodar o projeto básico de "Pisca LED" no ESP8266 (NodeMCU).

---

## 1. Configuração da Arduino IDE

1. Abra a **Arduino IDE**.
2. Vá em **File > Preferences** (Arquivo > Preferências).
3. No campo **Additional boards manager URLs**, cole a seguinte URL:
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Clique em **OK** e aguarde o download inicial.

---

## 2. Instalando o Pacote do ESP8266

1. Vá em **Tools > Board > Boards Manager** (Ferramentas > Placa > Gerenciador de Placas).
2. Na barra de pesquisa, digite **ESP8266**.
3. Selecione o pacote **esp8266 by ESP8266 Community** e clique em **Install**.

---

## 3. Conexão e Seleção de Placa

1. Conecte o ESP8266 à porta USB do computador.
2. Configure a placa em **Tools > Board > esp8266 > NodeMCU 1.0 (ESP-12E Module)**.
3. Selecione a porta correspondente em **Tools > Port**.

---

## 4. Exemplo do Código do Projeto Blink (Pisca LED)

Copie o código abaixo e cole na sua Arduino IDE:

```cpp
void setup() {
  // Configura o pino do LED interno como saída
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Liga o LED 
  // Nota: No ESP8266, o LED interno geralmente é "Active Low", 
  // o que significa que LOW liga e HIGH desliga.
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000);                       // Aguarda 1 segundo
  
  // Desliga o LED
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(1000);                       // Aguarda 1 segundo
}
```

---

## 5. Compilação e Upload

1. Clique no ícone de **Visto (Verify)** para validar o código.
2. Clique no ícone de **Seta para a direita (Upload)** para enviar o código para a placa.
3. *(Opcional)* Altere o valor do `delay` para `500` se quiser que o LED pisque mais rápido.

---

## 📄 Licença

Este projeto está sob a licença MIT. Sinta-se à vontade para usar, modificar e compartilhar os códigos!
