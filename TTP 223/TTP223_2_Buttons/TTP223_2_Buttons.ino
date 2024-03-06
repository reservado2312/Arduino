
/* 
 * Autor: Jonathan Rafael(Reservado2312)
 * Date:  03/03/2024
 * 
 * Connections
 * 
 * TTP 223        Arduino
 * 5v         =>  5v       
 * I\O(Sinal) =>  DigitalPort
 * GND        =>  GND
 * 
*/
#define Button1Pin 2
#define Button2Pin 3

int stBtn1;     
int stBtn2;     





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
     Serial.println("Button 1 Press") ;
   }
   if(stBtn2 == HIGH){
     Serial.println("Button 2 Press") ;
   }
  delay(500);
}
