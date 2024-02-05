#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;

#define TS_MINX 189
#define TS_MINY 919
#define TS_MAXX 925
#define TS_MAXY 209

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 80
#define BUTTON_MARGIN 20

#define BLACK   0x0000
#define WHITE   0xFFFF
#define CYAN    0x07FF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F

bool buttonExecuted[6] = {false, false, false, false, false, false}; // Inicializa todas as variáveis de controle como falsas


const int XP = 8, XM = A2, YP = A3, YM = 9; // Ajuste conforme a sua configuração

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button buttons[6];

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  tft.begin(ID);
  tft.setRotation(3);
  tft.fillScreen(BLACK);

  int screenWidth = tft.width();
  int screenHeight = tft.height();

  // Configuração dos botões
  for (int i = 0; i < 6; i++) {
    int buttonX = (screenWidth - 2 * BUTTON_WIDTH - 2 * BUTTON_MARGIN) / 2 + (i % 3) * (BUTTON_WIDTH + BUTTON_MARGIN);
    int buttonY = 80 + (i / 3) * (BUTTON_HEIGHT + BUTTON_MARGIN);
    buttons[i].initButton(&tft, buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, WHITE, CYAN, BLACK, (char*)("Icon " + String(i + 1)).c_str(), 2);
    buttons[i].drawButton(false);
  }
}

void loop() {
  TSPoint p = ts.getPoint();

  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);

  bool touched = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

  if (touched) {
    int16_t screen_x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    int16_t screen_y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    for (int i = 0; i < 6; i++) {
      if (buttons[i].contains(screen_x, screen_y)) {
        buttons[i].press(true);
      } else {
        buttons[i].press(false);
      }
    }
  }

  for (int i = 0; i < 6; i++) 
  {
    if (buttons[i].justReleased()) 
    {
      buttons[i].drawButton();
      if (!buttonExecuted[i]) 
      {
        handleButtonClick(i);
        buttonExecuted[i] = true; // Define a variável de controle para true após a execução da função
      }
    }
    if (buttons[i].justPressed()) 
    {
      buttons[i].drawButton(true);     
    }
  }
}

void handleButtonClick(int buttonIndex) {
  // Adicione lógica específica para cada botão aqui
  Serial.print("Icon ");
  Serial.print(buttonIndex + 1);
  Serial.println(" clicado!");
  // Execute ação correspondente ao ícone clicado
  switch (buttonIndex) {
    case 0:
      // Ação para o ícone 1
      // Exemplo: abrir uma página da web, iniciar uma função, etc.
      break;
    case 1:
      // Ação para o ícone 2
      break;
    // Continue para os outros ícones
  }
}
