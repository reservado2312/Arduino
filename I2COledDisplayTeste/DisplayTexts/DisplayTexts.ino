
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




void setup() {
  Serial.begin(9600);




  // put your setup code here, to run once:
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
  myTestOledText();
}


void myTestOledText()
{
  display.clearDisplay();
// distancia minima para segunda parte do display I2C
  display.setCursor(5, 20);
  display.println(F("Temperatura Atual:"));
  display.display();
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.println(F("32 C"));  
  display.display();


}

void loop() {
  
}
