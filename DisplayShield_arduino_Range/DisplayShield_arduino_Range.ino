
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
//y = altura x = largura no modo portatil no retrado é ao contrario
  tft.fillRect(40, 40, 160, 80, RED);
  //falta adcionar os textos e integrar com o codigo Comando Serial para receber e enviar os dados pela seriaal
  //e controlar dispositivos/ler valores de variaveis
  Range_btn.initButton(&tft, 120, 180, 220, 30, WHITE, CYAN, BLACK, "LED", 2);

  green_btn.initButton(&tft,  60, 230, 100, 40, WHITE, CYAN, BLACK, "LED ON", 2);
  red_btn.initButton(&tft, 180, 230, 100, 40, WHITE, CYAN, BLACK, "LED OFF", 2);

  blue_btn.initButton(&tft,  60, 280, 100, 40, WHITE, CYAN, BLACK, "RELE 1", 2);
  white_btn.initButton(&tft, 180, 280, 100, 40, WHITE, CYAN, BLACK, "RELE 2", 2);


  green_btn.drawButton(false);
  red_btn.drawButton(false);
  blue_btn.drawButton(false);
  white_btn.drawButton(false);
  Range_btn.drawButton(false);

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
//  if (Range_btn.justPressed()) 
//{
//  Range_btn.drawButton(true);
//  //tft.fillRect(40, 40, 160, 80, WHITE);
//  Serial.println("Range Pressed");
//  Serial.print("X: ");
//  Serial.println(pixel_x);
//    Serial.print(", Y: ");
//    Serial.println(pixel_y);
//  Serial.println(map(pixel_x, 20, 220, 0, 179));
//  Range_btn.setText(map(pixel_x, 20, 220, 0, 179));

  if (Range_btn.justPressed()) 
  {
    //A RangeOp1 e RangeOp2 foram geradas pelo Chat GPT para solucionar um problema mas não saiu do jeito que 
    //eu queria criei então a RangeOp3 que simplifica a solução porem tira a questão do efeito visual
    //quem sabe eu não crie a RangeOp4 que provavelmente sera gerando dois objetos um para o campo vazio 
    //e outro para o que esta preenchendo a barra
    //RangeOp1();
    //RangeOp2();
    RangeOp3();
  
  }
}


void RangeOp1()
{
  Range_btn.drawButton(true); // Redesenha o botão pressionado
  delay(5); // Pequeno atraso para efeito visual
  tft.fillRect(40, 40, 160, 80, RED); //
  Serial.print("X: ");
  Serial.println(pixel_x);
  int novoTexto = map(pixel_x, 10, 229, 0, 179); // Mapeia o valor da posição X para o novo texto
  char texto[4]; // Array para armazenar o texto
  sprintf(texto, "%d", novoTexto); // Converte o número para uma string
  Range_btn.initButton(&tft, 120, 180, 220, 30, WHITE, CYAN, BLACK, texto, 2); // Inicializa o botão com o novo texto
  Range_btn.drawButton(); // Desenha o novo botão com o novo texto    
  int novoValor = map(pixel_x, 10, 229, 0, 179); // Mapeia a coordenada X para o novo valor
  int larguraRetangulo = map(novoValor, 0, 179, 0, 160); // Mapeia o novo valor para a largura do retângulo

  int larguraAtual = 160; // Largura atual do retângulo

  if (larguraRetangulo < larguraAtual) // Se a nova largura for maior que a largura anterior
  {
    // Preenche a área removida com a cor vermelha de frente para trás
    for (int i = larguraAtual - 1; i >= larguraRetangulo; i--) 
    {
      delay(5); // Pequeno atraso para efeito visual
      tft.fillRect(40 + i, 40, 5, 80, BLUE);
      delay(5); // Pequeno atraso para efeito visual
    }
  }
  else if (larguraRetangulo > larguraAtual) // Se a nova largura for menor que a largura anterior
  {
    // Preenche a área adicional com a cor branca de trás para frente
    for (int i = larguraAtual; i < larguraRetangulo; i++) 
    {
      delay(5); // Pequeno atraso para efeito visual
      tft.fillRect(40 + i, 40, 5, 80, RED);
      delay(5); // Pequeno atraso para efeito visual
    }
  }  
}

void RangeOp2()
{
  Range_btn.drawButton(true); // Redesenha o botão pressionado
  delay(5); // Pequeno atraso para efeito visual
  tft.fillRect(40, 40, 160, 80, WHITE); //
  Serial.print("X: ");
  Serial.println(pixel_x);
  int novoTexto = map(pixel_x, 10, 229, 0, 179); // Mapeia o valor da posição X para o novo texto
  char texto[4]; // Array para armazenar o texto
  sprintf(texto, "%d", novoTexto); // Converte o número para uma string
  Range_btn.initButton(&tft, 120, 180, 220, 30, WHITE, CYAN, BLACK, texto, 2); // Inicializa o botão com o novo texto
  Range_btn.drawButton(); // Desenha o novo botão com o novo texto 
  int novoValor = map(pixel_x, 20, 220, 0, 179); // Mapeia a coordenada X para o novo valor
  int larguraRetangulo = map(novoValor, 0, 179, 0, 160); // Mapeia o novo valor para a largura do retângulo

  int larguraAtual = 160; // Largura atual do retângulo
  
  if (larguraRetangulo > larguraAtual) // Se a nova largura for maior que a largura anterior
  {
    // Preenche a área adicional com a cor branca de frente para trás
    for (int i = larguraRetangulo - 1; i >= larguraAtual; i--) 
    {
      tft.fillRect(40 + i, 40, 1, 80, WHITE);
      delay(5); // Pequeno atraso para efeito visual
    }
  }
  else if (larguraRetangulo < larguraAtual) // Se a nova largura for menor que a largura anterior
  {
    // Preenche a área removida com a cor vermelha de trás para frente
    for (int i = larguraAtual - 1; i >= larguraRetangulo; i--) 
    {
      tft.fillRect(40 + i, 40, 1, 80, RED);
      delay(5); // Pequeno atraso para efeito visual
    }
  }
}

void RangeOp3()
{ 
    tft.fillRect(40, 40, 165, 80, RED); //
    delay(5); // Pequeno atraso para efeito visual
    Range_btn.drawButton(true); // Redesenha o botão pressionado
    int novoValor = map(pixel_x, 20, 220, 0, 179); // Mapeia a coordenada X para o novo valor
    //tem uma pequena margem que vc tem qeu calibrar com o map do novoValor para dar cempre 
    //o tamnho minimo e maximo da barra deslizante
    int larguraRetangulo = map(novoValor, 0, 179, 1, 158); // Mapeia o novo valor para a largura do retângulo
    tft.fillRect(40, 40, larguraRetangulo, 80, WHITE); // Preenche o retângulo com a nova largura
}
