#include "HID-Project.h"
#include <HID-Settings.h>

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>


MCUFRIEND_kbv tft;
#include <TouchScreen.h>
//#include <Keyboard.h>

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
char btn1Label[] = "Chrome";
char btn2Label[] = "Note";
char btn3Label[] = "Calc";
char btn4Label[] = "Lock";
char btn5Label[] = "Unlock";
char btn6Label[] = "YTube";
//Your PassWord
char yourPwd[] = "2312";
char btn7Label[] = "Gmail";
char btn8Label[] = "ChGPT";
char btn9Label[] = "GDrive";
char btn10Label[] = "Github";
//char btn11Label[] = "Vol +";
//char btn12Label[] = "Vol -";
void setup(void)
{
  Serial.begin(9600);
  Keyboard.begin();
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3)
    ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0); //PORTRAIT
  tft.fillScreen(BLACK);

  btn1.initButton(&tft, 60, 160, 120, 40, WHITE, CYAN, BLACK, btn1Label, 3);
  btn2.initButton(&tft, 180, 160, 120, 40, WHITE, CYAN, BLACK, btn2Label, 3);
  btn4.initButton(&tft, 60, 40, 120, 40, WHITE, CYAN, BLACK, btn4Label, 3);
  btn5.initButton(&tft, 180, 40, 120, 40, WHITE, CYAN, BLACK, btn5Label, 3);
  btn3.initButton(&tft, 60, 100, 120, 40, WHITE, CYAN, BLACK, btn3Label, 3);
  btn6.initButton(&tft, 180, 100, 120, 40, WHITE, CYAN, BLACK, btn6Label, 3);
  btn7.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, btn7Label, 3);
  btn8.initButton(&tft, 180, 220, 120, 40, WHITE, CYAN, BLACK, btn8Label, 3);
  btn9.initButton(&tft, 60, 280, 120, 40, WHITE, CYAN, BLACK, btn9Label, 3);
  btn10.initButton(&tft, 180, 280, 120, 40, WHITE, CYAN, BLACK, btn10Label, 3);
  //btn11.initButton(&tft, 60, 380, 120, 40, WHITE, CYAN, BLACK, btn11Label, 3);
  //btn12.initButton(&tft, 180, 380, 120, 40, WHITE, CYAN, BLACK, btn12Label, 3);

  // Draw all Buttons
  btn1.drawButton(false);
  btn2.drawButton(false);
  btn3.drawButton(false);
  btn4.drawButton(false);
  btn5.drawButton(false);
  btn6.drawButton(false);
  btn7.drawButton(false);
  btn8.drawButton(false);
  btn9.drawButton(false);
  btn10.drawButton(false);
  //btn11.drawButton(false);
  //btn12.drawButton(false);
}

void loop(void)
{
  bool down = Touch_getXY();

  // Refresh all buttons with Your Touch Coordinates
  btn1.press(down && btn1.contains(pixel_x, pixel_y));
  btn2.press(down && btn2.contains(pixel_x, pixel_y));
  btn3.press(down && btn3.contains(pixel_x, pixel_y));
  btn4.press(down && btn4.contains(pixel_x, pixel_y));
  btn5.press(down && btn5.contains(pixel_x, pixel_y));
  btn6.press(down && btn6.contains(pixel_x, pixel_y));
  btn7.press(down && btn7.contains(pixel_x, pixel_y));
  btn8.press(down && btn8.contains(pixel_x, pixel_y));
  btn9.press(down && btn9.contains(pixel_x, pixel_y));
  btn10.press(down && btn10.contains(pixel_x, pixel_y));
 // btn11.press(down && btn5.contains(pixel_x, pixel_y));
  //btn12.press(down && btn6.contains(pixel_x, pixel_y));


  // Lead with the buttons individually
  if (btn1.justReleased())
    btn1.drawButton();
  if (btn2.justReleased())
    btn2.drawButton();
  if (btn3.justReleased())
    btn3.drawButton();
  if (btn4.justReleased())
    btn4.drawButton();
  if (btn5.justReleased())
    btn5.drawButton();
  if (btn6.justReleased())
    btn6.drawButton();
  if (btn7.justReleased())
    btn7.drawButton();
  if (btn8.justReleased())
    btn8.drawButton();
  if (btn9.justReleased())
    btn9.drawButton();
  if (btn10.justReleased())
    btn10.drawButton();
/*
  if (btn11.justReleased())
    btn11.drawButton();
  if (btn12.justReleased())
    btn12.drawButton();
*/    
  
  // Handle press events
  if (btn1.justPressed())
  {
    btn1.drawButton(true);
    Serial.println(btn1Label);
    //Google Chrome
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("chrome");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
  if (btn2.justPressed())
  {
    btn2.drawButton(true);
    Serial.println(btn2Label);
    //Notepad
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("notepad");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
  if (btn3.justPressed())
  {
    btn3.drawButton(true);
    Serial.println(btn3Label);
    //Calculator
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("CALC");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
  if (btn4.justPressed())
  {
    btn4.drawButton(true);
    Serial.println(btn4Label);
    // Press and realese The Shortcut(Win + L)
    Keyboard.press(KEY_LEFT_GUI); // Tecla Win (Windows)
    Keyboard.press('l'); // Tecla 'l'
    delay(100);
    Keyboard.releaseAll();
  }
  if (btn5.justPressed())
  {
    btn5.drawButton(true);
    Serial.println(btn5Label);
    //yourPwd
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.releaseAll();
    Keyboard.print(yourPwd);
  }
  if (btn6.justPressed())
  {
    btn6.drawButton(true);
    Serial.println(btn6Label);
    //Youtube Chrome 
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("chrome youtube.com");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
    // Handle press events for new buttons
  if (btn7.justPressed())
  {
    btn7.drawButton(true);
    Serial.println(btn7Label);
    //Gmail
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("chrome gmail.com");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
  if (btn8.justPressed())
  {
    btn8.drawButton(true);
    Serial.println(btn8Label);
    // Chat GPT
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("chrome chat.openai.com");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
  if (btn9.justPressed())
  {
    btn9.drawButton(true);
    Serial.println(btn9Label);
    // Google Drive
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(100);
    Keyboard.print("chrome drive.google.com");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
    
  }
  if (btn10.justPressed())
  {
    btn10.drawButton(true);
    Serial.println(btn10Label);
    //GitHub
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(200);
    Keyboard.print("chrome github.com");
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
  }
  /*
  if (btn11.justPressed())
  {
    btn11.drawButton(true);
    Serial.println(btn11Label);
    // Adicione sua lógica aqui
  }
  if (btn12.justPressed())
  {
    btn12.drawButton(true);
    Serial.println(btn12Label);
    // Adicione sua lógica aqui
  }
  */
}
