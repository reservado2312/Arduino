
/* 
 * Autor: Jonathan Rafael(Reservado2312)
 * Date:  03/03/2024
 * 
 * Connections
 * 
 * TTP 223        Arduino/Esp Boards
 * 5v         =>  5v  Or 3.3v     
 * I\O(Sinal) =>  DigitalPort
 * GND        =>  GND
 * 
*/
#define Button1Pin 2
#define Button2Pin 3

int stBtn1;     
int stBtn2;     
int randonNumber;

void setup() {
  Serial.begin(9600);
  pinMode(Button1Pin, INPUT);
  pinMode(Button2Pin, INPUT);
}

void loop() {
  stBtn1 = digitalRead(Button1Pin);
  stBtn2 = digitalRead(Button2Pin);
   if(stBtn1 == HIGH)
   {
     Serial.println("Option 1 - Generating random number from 1 to 10") ;
     randonNumber = random(1, 11);
     Serial.println(randonNumber);
   }
   if(stBtn2 == HIGH){
     Serial.println("Option 2 - Generating random number from 1 to 6") ;
     randonNumber = random(1, 7);
     Serial.println(randonNumber);
   }
  delay(500);
}
