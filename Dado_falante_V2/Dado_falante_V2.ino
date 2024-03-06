/*
 * Dados/Roleta Falante
 * Usando 3 Botões e um Modulo MP3 JQ 6500 16Mb   
   Autor: Jonathan Rafel(reservado2312)  Data: 5 de Março de 2024  
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JQ6500_Serial.h>

#define Button1Pin 2 // Button Number 1 to 10
#define Button2Pin 3 // Button Number 1 to 6
#define Button3Pin 4 // Button Number 1 to 12


int stBtn1;     
int stBtn2;     
int stBtn3;  
int randonNumber;


//   Create the mp3 module object, 
//   Arduino Pin 8 is connected to TX of the JQ6500
//   Arduino Pin 6 is connected to one end of a  1k resistor, 
//   the other end of the 1k resistor is connected to RX of the JQ6500
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor

SoftwareSerial mySerial(8, 6);
JQ6500_Serial mp3(mySerial);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(Button1Pin, INPUT);
  pinMode(Button2Pin, INPUT);
  mp3.reset();
  mp3.setVolume(100);
  mp3.setLoopMode(MP3_LOOP_NONE);
}

void loop() {
  stBtn1 = digitalRead(Button1Pin);
  stBtn2 = digitalRead(Button2Pin);

  if(stBtn1 == HIGH)
  {
   Serial.println("Option 1 - Generating random number from 1 to 10") ;
   randonNumber = random(1, 11);
   Serial.println(randonNumber);
    if(mp3.getStatus() != MP3_STATUS_PLAYING)
    {
      mp3.playFileByIndexNumber(14);  
      delay(4900);
      mp3.playFileByIndexNumber(randonNumber);       
    }   
  }
  if(stBtn2 == HIGH){
   Serial.println("Option 2 - Generating random number from 1 to 6") ;
   randonNumber = random(1, 7);
   Serial.println(randonNumber);
    if(mp3.getStatus() != MP3_STATUS_PLAYING)
    {
      mp3.playFileByIndexNumber(13);  
      delay(2000);
      mp3.playFileByIndexNumber(randonNumber);  
    } 
  }
  if(stBtn3 == HIGH){
   Serial.println("Option 3 - Generating random number from 1 to 12") ;
   randonNumber = random(1, 7);
   Serial.println(randonNumber);
    if(mp3.getStatus() != MP3_STATUS_PLAYING)
    {
      mp3.playFileByIndexNumber(13);  
      delay(2000);
      mp3.playFileByIndexNumber(randonNumber);  
    } 
  }
}
