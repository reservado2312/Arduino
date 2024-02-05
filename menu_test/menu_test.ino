#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 10


#define XP 8
#define YP A3
#define XM A2
#define YM 9

#define TS_MINX 178
#define TS_MAXX 904
#define TS_MINY 937
#define TS_MAXY 214

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);

  tft.setRotation(1); // Ajuste a rotação conforme necessário

  tft.fillScreen(TFT_WHITE);
  
  drawButton(10, 10, " 1");
  drawButton(100, 10, " 2");
  drawButton(190, 10, " 3");
  drawButton(10, 60, " 4");
  drawButton(100, 60, " 5");
  drawButton(190, 60, " 6");
  drawButton(10, 110, " 7");
  drawButton(100, 110, " 8");
  drawButton(190, 110, " 9");
}

void loop() {
  TSPoint touch = ts.getPoint();

  if (touch.z > 0) {
    int touchX = map(touch.x, TS_MINX, TS_MAXX, 0, tft.width());
    int touchY = map(touch.y, TS_MINY, TS_MAXY, 0, tft.height());

    checkButtonPress(touchX, touchY);
    delay(50); // Debounce
  }
}

void checkButtonPress(int touchX, int touchY) {
  if (checkButton(touchX, touchY, 10, 10, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 1
    Serial.println("Botão 1 pressionado");
    
  } else if (checkButton(touchX, touchY, 100, 10, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 2
    Serial.println("Botão 2 pressionado");
  } else if (checkButton(touchX, touchY, 190, 10, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 3
    Serial.println("Botão 3 pressionado");
  } else if (checkButton(touchX, touchY, 10, 60, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 4
    Serial.println("Botão 4 pressionado");
  } else if (checkButton(touchX, touchY, 100, 60, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 5
    Serial.println("Botão 5 pressionado");
  } else if (checkButton(touchX, touchY, 190, 60, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 6
    Serial.println("Botão 6 pressionado");
  } else if (checkButton(touchX, touchY, 10, 110, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 7
    Serial.println("Botão 7 pressionado");
  } else if (checkButton(touchX, touchY, 100, 110, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 8
    Serial.println("Botão 8 pressionado");
  } else if (checkButton(touchX, touchY, 190, 110, BUTTON_WIDTH, BUTTON_HEIGHT)) {
    // Lógica para o botão 9
    Serial.println("Botão 9 pressionado");
  }
}

bool checkButton(int touchX, int touchY, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
  return (touchX >= buttonX && touchX <= buttonX + buttonWidth && touchY >= buttonY && touchY <= buttonY + buttonHeight);
}

void drawButton(int x, int y, const char* label) {
  tft.fillRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(x + 10, y + 10);
  tft.print(label);
}
