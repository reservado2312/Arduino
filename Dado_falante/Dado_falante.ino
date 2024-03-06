#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JQ6500_Serial.h>
#include <SPI.h>

// Create the mp3 module object, 
//   Arduino Pin 8 is connected to TX of the JQ6500
//   Arduino Pin 6 is connected to one end of a  1k resistor, 
//   the other end of the 1k resistor is connected to RX of the JQ6500
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor
SoftwareSerial mySerial(8, 6);
JQ6500_Serial mp3(mySerial);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  mp3.reset();
  mp3.setVolume(100);
  mp3.setLoopMode(MP3_LOOP_NONE);
}

void loop() {
    int randomNumber = random(1, 11);  // Gera um número aleatório de 1 a 6
    Serial.println(randomNumber);     // Imprime o número aleatório no monitor serial
    delay(100);
  
  if(mp3.getStatus() != MP3_STATUS_PLAYING)
  {
    mp3.playFileByIndexNumber(randomNumber);  
  } 

  delay(2000);
}
