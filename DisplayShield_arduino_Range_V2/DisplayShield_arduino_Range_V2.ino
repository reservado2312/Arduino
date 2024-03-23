
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

// Segundo Display

//*** COPY-PASTE from Serial Terminal:
//const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
//const int TS_LEFT=276,TS_RT=826,TS_TOP=909,TS_BOT=167;

//PORTRAIT  CALIBRATION     240 x 320
//x = map(p.x, LEFT=276, RT=826, 0, 240)
//y = map(p.y, TOP=909, BOT=167, 0, 320)

//Randge Slider Setting
const int X_RandgeSlider = 10, Y_RandgeSlider = 140, W_RandgeSlider = 220, H_RandgeSlider = 10;

//Texts Settings

//Time
const int X_Time = 45, Y_Time = 10, Size_Time = 5;
  
//Date
const int X_Date = 30, Y_Date = 55, Size_Date = 3;
  
//Temp
const int X_Temp = 40, Y_Temp = 85, Size_Temp = 3;
  
//Humidity
const int X_Humi = 45, Y_Humi = 120, Size_Humi = 2;
  
  
  


  //          X   Y   S
  //TIME
  //showmsgXY(45, 10, 5, "00:00");
  //DATE
  //showmsgXY(30, 55, 3, "00/00/0000");
  //TEMP
  //showmsgXY(40, 85, 3, "Temp 22 C");
  //Humidity
  //showmsgXY(50, 120, 2, "Humidity 00%");

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button btn_1, btn_2, btn_3, btn_4, Range_btn;

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
  //TIME
  showmsgXY(X_Time, Y_Time, Size_Time, "00:00");
  //DATE
  showmsgXY(X_Date, Y_Date, Size_Date, "00/00/0000");
  //TEMP
  showmsgXY(X_Temp, Y_Temp, Size_Temp, "Temp 22 C");
  //Humidity
  showmsgXY(X_Humi, Y_Humi, Size_Humi, "Humidity 00%");
//y = altura x = largura no modo portatil no retrado é ao contrario
  tft.fillRect(X_RandgeSlider, Y_RandgeSlider, W_RandgeSlider, H_RandgeSlider, RED);
  //falta adcionar os textos e integrar com o codigo Comando Serial para receber e enviar os dados pela seriaal
  //e controlar dispositivos/ler valores de variaveis
  Range_btn.initButton(&tft, 120, 180, 220, 30, WHITE, CYAN, BLACK, "LED", 2);

  btn_1.initButton(&tft,  60, 230, 110, 40, WHITE, CYAN, BLACK, "RELE 1", 2);
  btn_2.initButton(&tft, 180, 230, 110, 40, WHITE, CYAN, BLACK, "RELE 2", 2);  
  btn_3.initButton(&tft,  60, 280, 110, 40, WHITE, CYAN, BLACK, "RELE 3", 2);
  btn_4.initButton(&tft, 180, 280, 110, 40, WHITE, CYAN, BLACK, "RELE 4", 2);


  btn_1.drawButton(false);
  btn_2.drawButton(false);
  btn_3.drawButton(false);
  btn_4.drawButton(false);
  Range_btn.drawButton(false);

}


void ChamadaComando()
{
    String comando = Serial.readStringUntil(' '); // Lê o comando enviado pela porta serial
    String acao; //Recebe acao do comando que Liga ou Desliga algo
    //Exemplo led: LED ON ou LED OFF
    //Exemplo rele: RELE1 ON ou RELE1 OFF
    //Exemplo msg: MSG Sua Menssagem!!

    // Debug: Imprime o comando recebido
    Serial.print("Comando recebido: ");
    Serial.println(comando);

    if (comando == "TIME" || comando == "DATE" || comando == "TEMP" || comando == "RANGE_BAR") {

      acao = Serial.readStringUntil('\n'); // Lê a ação (ON ou OFF)
      
      // Debug: Imprime a ação recebida
      //Serial.print("Ação recebida: ");
      //Serial.println(acao);

      // Determina qual LED deve ser controlado
      if (comando == "TIME") 
      {
        //De 5 espaços para centralizar a hora depois da data
        //TIME 23:40
        tft.fillRect(0, Y_Time, 240, 40, BLACK); //Apaga o que estiver escrito nessa cordenada
        showmsgXY(X_Time, Y_Time, Size_Time, acao.c_str());
        //showmsgXY(50, 10, 5, acao.c_str());
      }
      else if (comando == "DATE") 
      {
        //De 5 espaços para centralizar a hora depois da data
        //DATE 21/03/2024
        tft.fillRect(0, Y_Date, 240, 20, BLACK); //Apaga o que estiver escrito nessa cordenada
        showmsgXY(X_Date, Y_Date, Size_Date, acao.c_str());
        //showmsgXY(50, 60, 3, acao.c_str());
      } 
      else if (comando == "TEMP") 
      {
        tft.fillRect(0, Y_Temp, 240, 20, BLACK);
        char txTemp[10]; // Array to store Text, assuming numbers won't exceed 9 digits      
        strcpy(txTemp, acao.c_str());
        char msgTemp[10]; // Assuming the message won't exceed 50 characters
        strcpy(msgTemp, "Temp ");
        strcpy(txTemp, acao.c_str());
        strcat(msgTemp, txTemp);
        strcat(msgTemp, " C");
        showmsgXY(X_Temp, Y_Temp, Size_Temp, msgTemp);
        //showmsgXY(40, 80, 3, msgTemp);
      } 
      else if (comando == "RANGE_BAR") 
      {
        tft.fillRect(X_RandgeSlider, Y_RandgeSlider, W_RandgeSlider, H_RandgeSlider, RED); //  Reset RandgeSlider
        int meuInt = acao.toInt();
        //Serial.print("Valor: ");
        //Serial.println(meuInt);
        //There's a small margin that you have to calibrate according to the size of your button
        //The minimum and maximum size of the slide bar
        int larguraRetangulo = map(meuInt, 0, 100, 0, W_RandgeSlider); // Mapeia o novo valor para a largura do retângulo
        tft.fillRect(X_RandgeSlider, Y_RandgeSlider, larguraRetangulo, H_RandgeSlider, WHITE); // Preenche o retângulo com a nova largura        


        char txHumi[10]; // Array to store Text, assuming numbers won't exceed 9 digits      
        strcpy(txHumi, acao.c_str());
        char msgHumi[10]; // Assuming the message won't exceed 50 characters
        strcpy(msgHumi, "Humidity ");
        strcpy(txHumi, acao.c_str());
        strcat(msgHumi, txHumi);
        strcat(msgHumi, " %");

        tft.fillRect(0, Y_Humi, 240, 20, BLACK);
        showmsgXY(X_Humi, Y_Humi, Size_Humi, msgHumi);
      }  
    }
    else
    {
      //Debug que Trata comando inexistente
      Serial.println("Comando: ");
      Serial.println(comando);
      Serial.println("Inesistente!");
    }
}


void showmsgXY(int x, int y, int sz, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;    
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(10);
}

void loop()
{
  bool down = Touch_getXY();
  btn_1.press(down && btn_1.contains(pixel_x, pixel_y));
  btn_2.press(down && btn_2.contains(pixel_x, pixel_y));
  btn_3.press(down && btn_3.contains(pixel_x, pixel_y));
  btn_4.press(down && btn_4.contains(pixel_x, pixel_y));
  Range_btn.press(down && Range_btn.contains(pixel_x, pixel_y));
  if (Serial.available() > 0) 
  { // Verifica se há dados disponíveis para leitura
    ChamadaComando();
  }

  if (btn_1.justReleased()) btn_1.drawButton();
  if (btn_2.justReleased()) btn_2.drawButton();
  if (btn_3.justReleased()) btn_3.drawButton();
  if (btn_4.justReleased()) btn_4.drawButton();
  if (Range_btn.justReleased()) Range_btn.drawButton();                  

  if (btn_1.justPressed()) 
  {    
    btn_1.drawButton(true);
    //tft.fillRect(0, 10, 240, 20, BLACK); //Apaga o que estiver escrito nessa cordenada
    //showmsgXY(8, 10, 2 , "Botao 1 Pressionado");
    Serial.println("BTN 1");
  }
  if (btn_2.justPressed()) 
  {
    btn_2.drawButton(true);
    //tft.fillRect(0, 10, 240, 20, BLACK); //Apaga o que estiver escrito nessa cordenada
    //showmsgXY(8, 10, 2 , "Botao 2 Pressionado");
    Serial.println("BTN 2");
  }
  
  if (btn_3.justPressed()) 
  {
    btn_3.drawButton(true);
    //tft.fillRect(0, 10, 240, 20, BLACK); //Apaga o que estiver escrito nessa cordenada
    //showmsgXY(8, 10, 2 , "Botao 3 Pressionado");
    Serial.println("BTN 3");
  }
  if (btn_4.justPressed()) 
  {
    btn_4.drawButton(true);
    //tft.fillRect(0, 10, 240, 20, BLACK); //Apaga o que estiver escrito nessa cordenada
    //showmsgXY(8, 10, 2 , "Botao 4 Pressionado");
    Serial.println("BTN 4");
  }
  if (Range_btn.justPressed()) 
  {
    //A RangeOp1 e RangeOp2 foram geradas pelo Chat GPT para solucionar um problema mas não saiu do jeito que 
    //eu queria criei então a RangeOp3 que simplifica a solução porem tira a questão do efeito visual
    //quem sabe eu não crie a RangeOp4 que provavelmente sera gerando dois objetos um para o campo vazio 
    //e outro para o que esta preenchendo a barra
    RangeOp1();
  }
}

void RangeOp1()
{   
    //debug
    //Serial.print("X ");
    //Serial.println(pixel_x);
    //Serial.print("y ");
    //Serial.println(pixel_y);
    

    tft.fillRect(X_RandgeSlider, Y_RandgeSlider, W_RandgeSlider, H_RandgeSlider, RED); //  Reset RandgeSlider
    Range_btn.drawButton(true); // Redraw the button you press
    int novoValor = map(pixel_x, 10, 229, 0, 179); // Map X for novo valor
    //There's a small margin that you have to calibrate according to the size of your button
    //The minimum and maximum size of the slide bar
    int larguraRetangulo = map(novoValor, 0, 179, 1, W_RandgeSlider); // Mapeia o novo valor para a largura do retângulo
    tft.fillRect(X_RandgeSlider, Y_RandgeSlider, larguraRetangulo, H_RandgeSlider, WHITE); // Preenche o retângulo com a nova largura
    int novoTexto = map(pixel_x, 10, 229, 0, 100); // Mapeia o valor da posição X para o novo texto
    char texto[4]; // Array to store Text
    sprintf(texto, "%d", novoTexto); // Convert Number to String
    char message[50]; // Assuming the message won't exceed 50 characters
    strcpy(message, "Led ");
    strcat(message, texto);
    strcat(message, "%");

    //Send Data for Serial By MySelf Protocol
    Serial.print("RANG_BTN ");
    Serial.println(novoTexto);    
    Range_btn.initButton(&tft, 120, 180, 220, 30, WHITE, CYAN, BLACK, message, 2); // Inicializa o botão com o novo texto
}
