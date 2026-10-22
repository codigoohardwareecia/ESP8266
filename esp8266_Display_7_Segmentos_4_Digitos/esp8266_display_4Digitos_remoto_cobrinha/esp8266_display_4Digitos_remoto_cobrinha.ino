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