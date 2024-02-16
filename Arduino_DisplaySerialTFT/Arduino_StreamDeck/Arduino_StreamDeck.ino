#include <TouchScreen.h>
#include <string.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>


MCUFRIEND_kbv tft;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
const int XP = 8, XM = A2, YP = A3, YM = 9; // 240x320 ID=0x9341
const int TS_LEFT = 189, TS_RT = 919, TS_TOP = 925, TS_BOT = 209;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn10; //, btn11, btn12; // Add More Buttons

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


//Labels of Buttons
char btn1Label[] = "Rele 1";
char btn2Label[] = "Rele 2";
char btn3Label[] = "Rele 3";
char btn4Label[] = "Rele 4";

bool btn1Pressed = false;
bool btn2Pressed = false;
bool btn3Pressed = false;
bool btn4Pressed = false;

// Variável para armazenar o valor recebido pela porta serial
char serialData[20]; // Tamanho arbitrário, ajuste conforme necessário

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

  btn1.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, btn1Label, 3);
  btn2.initButton(&tft, 180, 220, 120, 40, WHITE, CYAN, BLACK, btn2Label, 3);
  btn3.initButton(&tft, 60, 280, 120, 40, WHITE, CYAN, BLACK, btn3Label, 3);
  btn4.initButton(&tft, 180, 280, 120, 40, WHITE, CYAN, BLACK, btn4Label, 3);
  // Draw all Buttons
  btn1.drawButton(false);
  btn2.drawButton(false);
  btn3.drawButton(false);
  btn4.drawButton(false);
  //tft.setTextColor(WHITE, BLACK);
  //tft.setTextSize(3); 
  //tft.setCursor(55,80);
  //tft.println("00:00:00");
  
}

void loop(void)
{
  showbtn();
  showbtnPressed();
  if (Serial.available() > 0) 
    {
    // Se houver dados disponíveis na porta serial
    // Leia os dados para a variável serialData
    Serial.readBytesUntil('\n', serialData, sizeof(serialData));

    tft.fillRect(55, 80, 240, 50, BLACK);
    // Exiba o valor recebido no display TFT
    tft.setTextColor(WHITE);
    tft.setTextSize(3); 
    tft.setCursor(55,80);
    tft.print(serialData);    
  }
}




void showbtn()
{
  bool down = Touch_getXY();

  // Refresh all buttons with Your Touch Coordinates
  btn1.press(down && btn1.contains(pixel_x, pixel_y));
  btn2.press(down && btn2.contains(pixel_x, pixel_y));
  btn3.press(down && btn3.contains(pixel_x, pixel_y));
  btn4.press(down && btn4.contains(pixel_x, pixel_y));
  
  // Lead with the buttons individually
  if (btn1.justReleased())
    btn1.drawButton();
  if (btn2.justReleased())
    btn2.drawButton();
  if (btn3.justReleased())
    btn3.drawButton();
  if (btn4.justReleased())
    btn4.drawButton();
  // Handle press events
  if (btn1.justPressed())
  {
    btn1Pressed = true;
    btn1.drawButton(true);
    Serial.println(btn1Label);  
  }
  if (btn2.justPressed())
  {
    btn1Pressed = false;
    btn2.drawButton(true);
    Serial.println(btn2Label);   
  }
  if (btn3.justPressed())
  {
    btn3.drawButton(true);
    Serial.println(btn3Label);   
  }
  if (btn4.justPressed())
  {
    btn4.drawButton(true);
    Serial.println(btn4Label);   
  }
}

void showbtnPressed()
{
  if(btn1Pressed)
  {
    showmsgXY(60, 110, 2, strcat(btn1Label, "  Press"));
    btn1Pressed = false;
  }
}

void showmsgXY(int x, int y, int sz, const char *msg)
{    
    tft.fillRect(0, y, 240, 50, BLACK);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(300);
    //preenche os espaço com 
    tft.fillRect(0, y, 240, 50, BLACK);
    
}
