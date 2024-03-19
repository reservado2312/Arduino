
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=178,TS_RT=904,TS_TOP=937,TS_BOT=214;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button green_btn, red_btn, blue_btn, white_btn, Range_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY()
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup()
{
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0);            //PORTRAIT
  tft.fillScreen(BLACK);

  green_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "1", 2);
  red_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "2", 2);

  blue_btn.initButton(&tft,  60, 250, 100, 40, WHITE, CYAN, BLACK, "3", 2);
  white_btn.initButton(&tft, 180, 250, 100, 40, WHITE, CYAN, BLACK, "4", 2);

  Range_btn.initButton(&tft, 120, 10, 200, 20, WHITE, CYAN, BLACK, "Range Pin", 2);

  green_btn.drawButton(false);
  red_btn.drawButton(false);
  blue_btn.drawButton(false);
  white_btn.drawButton(false);
  Range_btn.drawButton(false);

  tft.fillRect(40, 40, 160, 80, RED);
}

/* two buttons are quite simple
 */
void loop()
{
  bool down = Touch_getXY();
  green_btn.press(down && green_btn.contains(pixel_x, pixel_y));
  red_btn.press(down && red_btn.contains(pixel_x, pixel_y));
  blue_btn.press(down && blue_btn.contains(pixel_x, pixel_y));
  white_btn.press(down && white_btn.contains(pixel_x, pixel_y));
  Range_btn.press(down && Range_btn.contains(pixel_x, pixel_y));

  if (green_btn.justReleased())
      green_btn.drawButton();
  if (red_btn.justReleased())
      red_btn.drawButton();
  if (blue_btn.justReleased())
      blue_btn.drawButton();
  if (white_btn.justReleased())
      white_btn.drawButton();
  if (Range_btn.justReleased())
      Range_btn.drawButton();                  

  if (green_btn.justPressed()) 
  {
    green_btn.drawButton(true);
    tft.fillRect(40, 40, 160, 80, GREEN);
  }
  if (red_btn.justPressed()) 
  {
    red_btn.drawButton(true);
    tft.fillRect(40, 40, 160, 80, RED);
  }
  
  if (blue_btn.justPressed()) 
  {
    blue_btn.drawButton(true);
    tft.fillRect(40, 40, 160, 80, BLUE);
  }
  if (white_btn.justPressed()) 
  {
    white_btn.drawButton(true);
    tft.fillRect(40, 40, 160, 80, WHITE);
  }
    if (Range_btn.justPressed()) 
  {
    Range_btn.drawButton(true);
    //tft.fillRect(40, 40, 160, 80, WHITE);
    Serial.println("Range Pressed");
    Serial.print("X: ");
    Serial.print(pixel_x);
    Serial.print(", Y: ");
    Serial.println(pixel_y);
  }
}



