#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
const int XP = 8, XM = A2, YP = A3, YM = 9; // 240x320 ID=0x9341
const int TS_LEFT = 189, TS_RT = 919, TS_TOP = 925, TS_BOT = 209;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn, btn3, btn4, btn5, btn6; // Adicione mais botões conforme necessário

int pixel_x, pixel_y; //Touch_getXY() updates global vars

bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed)
  {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void setup(void)
{
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3)
    ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0); //PORTRAIT
  tft.fillScreen(BLACK);

  char btn1Label[] = "Chrome";
  char btn2Label[] = "Notepad";
  char btn3Label[] = "Calc";
  char btn4Label[] = "Lock PC";
  char btn5Label[] = "Unlock";
  char btn6Label[] = "Shutdow";

  on_btn.initButton(&tft, 60, 200, 120, 40, WHITE, CYAN, BLACK, btn1Label, 2);
  off_btn.initButton(&tft, 180, 200, 120, 40, WHITE, CYAN, BLACK, btn2Label, 2);
  btn3.initButton(&tft, 60, 80, 120, 40, WHITE, CYAN, BLACK, btn3Label, 2);
  btn4.initButton(&tft, 180, 80, 120, 40, WHITE, CYAN, BLACK, btn4Label, 2);
  btn5.initButton(&tft, 60, 140, 120, 40, WHITE, CYAN, BLACK, btn5Label, 2);
  btn6.initButton(&tft, 180, 140, 120, 40, WHITE, CYAN, BLACK, btn6Label, 2);

  // Desenhe todos os botões
  on_btn.drawButton(false);
  off_btn.drawButton(false);
  btn3.drawButton(false);
  btn4.drawButton(false);
  btn5.drawButton(false);
  btn6.drawButton(false);
}

void loop(void)
{
  bool down = Touch_getXY();

  // Atualize todos os botões com as coordenadas do toque
  on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
  off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
  btn3.press(down && btn3.contains(pixel_x, pixel_y));
  btn4.press(down && btn4.contains(pixel_x, pixel_y));
  btn5.press(down && btn5.contains(pixel_x, pixel_y));
  btn6.press(down && btn6.contains(pixel_x, pixel_y));

  // Lidere com os botões individualmente
  if (on_btn.justReleased())
    on_btn.drawButton();
  if (off_btn.justReleased())
    off_btn.drawButton();
  if (btn3.justReleased())
    btn3.drawButton();
  if (btn4.justReleased())
    btn4.drawButton();
  if (btn5.justReleased())
    btn5.drawButton();
  if (btn6.justReleased())
    btn6.drawButton();

  // Trate os eventos de pressionar
  if (on_btn.justPressed())
  {
    on_btn.drawButton(true);
    Serial.println("Botão Liga 1 pressionado");
  }
  if (off_btn.justPressed())
  {
    off_btn.drawButton(true);
    Serial.println("Botão Desliga 2 pressionado");
  }
  if (btn3.justPressed())
  {
    btn3.drawButton(true);
    Serial.println("Botão 3 pressionado");
  }
  if (btn4.justPressed())
  {
    btn4.drawButton(true);
    Serial.println("Botão 4 pressionado");
  }
  if (btn5.justPressed())
  {
    btn5.drawButton(true);
    Serial.println("Botão 5 pressionado");
  }
  if (btn6.justPressed())
  {
    btn6.drawButton(true);
    Serial.println("Botão 6 pressionado");
  }
}
