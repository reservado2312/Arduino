#include <XPT2046_Bitbang.h>
#include <TFT_eSPI.h>

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN  25
#define CS_PIN   33

XPT2046_Bitbang touchscreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);
TFT_eSPI tft = TFT_eSPI();

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
#define BUTTON1_X 10
#define BUTTON1_Y 155
#define BUTTON2_X 130
#define BUTTON2_Y 155

#define LedRed 16
#define LedGrenn 4
#define LedBlue 17

void setup() {
  Serial.begin(115200);
  pinMode(LedRed, OUTPUT);
  pinMode(LedGrenn, OUTPUT);
  pinMode(LedBlue, OUTPUT);

  digitalWrite(LedRed, LOW);
  digitalWrite(LedGrenn, LOW);
  digitalWrite(LedBlue, LOW);

  // Initialize the touchscreen
  touchscreen.begin();
  
  // Initialize the TFT display
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.invertDisplay(true);

  // Draw buttons on the screen
  tft.fillRect(BUTTON1_X, BUTTON1_Y, BUTTON_WIDTH, BUTTON_HEIGHT, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Power On Pc", BUTTON1_X + 15, BUTTON1_Y + 20);

  tft.fillRect(BUTTON2_X, BUTTON2_Y, BUTTON_WIDTH, BUTTON_HEIGHT, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Power Off Pc", BUTTON2_X + 15, BUTTON2_Y + 20);
}

void loop() {
  TouchPoint touch = touchscreen.getTouch();

  // Check if the touch has a pressure value (Z)
  if (touch.zRaw != 0) {
    // Map touchscreen coordinates to display coordinates
    int16_t x = map(touch.y, 23, 290, 0, tft.width());
    int16_t y = map(touch.x, 20, 217, 0, tft.height());
    // Retrato 1
    //int16_t x = map(touch.x, 23, 290, 0, tft.width());
    //int16_t y = map(touch.y, 20, 217, 0, tft.height());

    // Check if Button 1 is pressed
    if (x > BUTTON1_X && x < BUTTON1_X + BUTTON_WIDTH &&
        y > BUTTON1_Y && y < BUTTON1_Y + BUTTON_HEIGHT) {
      Serial.println("Botao 1 pressionado");
      
      digitalWrite(LedRed, HIGH);
      digitalWrite(LedGrenn, HIGH);
      digitalWrite(LedBlue, HIGH);
    }

    // Check if Button 2 is pressed
    if (x > BUTTON2_X && x < BUTTON2_X + BUTTON_WIDTH &&
        y > BUTTON2_Y && y < BUTTON2_Y + BUTTON_HEIGHT) {
      Serial.println("Botao 2 pressionado");
      digitalWrite(LedRed, LOW);
      digitalWrite(LedGrenn, LOW);
      digitalWrite(LedBlue, LOW);
    }
  }

  delay(100);  // Adjust delay as needed for responsiveness vs. serial output frequency
}
