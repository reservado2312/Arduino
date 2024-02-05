#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 940
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000
const int XP = 8, XM = A2, YP = A3, YM = 9; // Ajuste conforme a sua configuração

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define XP 8
#define XM A2
#define YP A3
#define YM 9

#define BLACK   0x0000
#define WHITE   0xFFFF
#define CYAN    0x07FF

#define BUTTON_WIDTH 90
#define BUTTON_HEIGHT 60
#define BUTTON_RADIUS 7
#define BUTTON_MARGIN 20

bool buttonState[6] = {false, false, false, false, false, false}; // Estado inicial: todos desligados

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

  // Configuração dos botões na parte superior
  for (int i = 0; i < 3; i++) {
    int buttonX = (screenWidth - 3 * BUTTON_WIDTH - 2 * BUTTON_MARGIN) / 2 + i * (BUTTON_WIDTH + BUTTON_MARGIN);
    int buttonY = 20;
    buttons[i].initButton(&tft, buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, WHITE, CYAN, BLACK, ("Button " + String(i + 1)).c_str(), 2);
    buttons[i].drawButton(false);
  }

  // Configuração dos botões na parte inferior
  for (int i = 3; i < 6; i++) {
    int buttonX = (screenWidth - 3 * BUTTON_WIDTH - 2 * BUTTON_MARGIN) / 2 + (i - 3) * (BUTTON_WIDTH + BUTTON_MARGIN);
    int buttonY = screenHeight - 100;
    buttons[i].initButton(&tft, buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, WHITE, CYAN, BLACK, ("Button " + String(i + 1)).c_str(), 2);
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
    int16_t screen_x = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
    int16_t screen_y = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());

    for (int i = 0; i < 6; i++) {
      if (buttons[i].contains(screen_x, screen_y)) {
        buttons[i].press(true);
      } else {
        buttons[i].press(false);
      }
    }
  }

  for (int i = 0; i < 6; i++) {
    if (buttons[i].justReleased()) {
      buttons[i].drawButton();
    }
    if (buttons[i].justPressed()) {
      buttons[i].drawButton(true);
      buttonState[i] = !buttonState[i]; // Inverte o estado ao pressionar
      handleButtonPress(i);
    }
  }
}

void handleButtonPress(int buttonIndex) {
  // Adicione lógica específica para cada botão aqui
  Serial.print("Button ");
  Serial.print(buttonIndex + 1);
  Serial.print(" switched ");
  Serial.println(buttonState[buttonIndex] ? "ON" : "OFF");
}
