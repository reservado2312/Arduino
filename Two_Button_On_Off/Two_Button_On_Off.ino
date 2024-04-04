// Definição dos pinos
const int buttonPin1 = 18; // Pino do botão 1
const int buttonPin2 = 19; // Pino do botão 2

const int ledPin1 = 25;   // Pino do LED 1
const int ledPin2 = 26;   // Pino do LED 2

// Variáveis de estado
bool ledState1 = false;   // Estado inicial do LED 1
bool lastButtonState1 = HIGH; // Estado anterior do botão 1
bool buttonState1;        // Estado atual do botão 1

bool ledState2 = false;   // Estado inicial do LED 2
bool lastButtonState2 = HIGH; // Estado anterior do botão 2
bool buttonState2;        // Estado atual do botão 2

void setup() {
  Serial.begin(9600);
  // Configura o pino do botão 1 como entrada
  pinMode(buttonPin1, INPUT);
  // Configura o pino do LED 1 como saída
  pinMode(ledPin1, OUTPUT);
  // Configura o pino do botão 2 como entrada
  pinMode(buttonPin2, INPUT);
  // Configura o pino do LED 2 como saída
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // Lê o estado atual do botão
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  // Verifica se o botão 1 foi pressionado
  if (buttonState1 == LOW && lastButtonState1 == HIGH) {
    // Inverte o estado do LED 1
    ledState1 = !ledState1;
    // Atualiza o estado do LED 1 caso ledState seja verdadeio ou falso usando o
    // operador condicional ternário (variavel/função que retorne true ou false ? HIGH : FALSE)  para determinar o valor a ser escrito no pino do LED
    digitalWrite(ledPin1, ledState1 ? HIGH : LOW);
    ledState1 ? Funcao1Butao1() : Funcao2Butao1();
  }
  // Salva o estado atual do botão 1 para a próxima iteração
  lastButtonState1 = buttonState1;

  // Verifica se o botão 2 foi pressionado
  if (buttonState2 == LOW && lastButtonState2 == HIGH) {
    // Inverte o estado do LED 2
    ledState2 = !ledState2;
    // Atualiza o estado do LED 2 caso ledState seja verdadeio ou falso usando o
    // operador condicional ternário (variavel/função que retorne true ou false ? HIGH : FALSE)  para determinar o valor a ser escrito no pino do LED
    digitalWrite(ledPin2, ledState2 ? HIGH : LOW);
    ledState2 ? Funcao1Butao2() : Funcao2Butao2();
  }
  // Salva o estado atual do botão 2 para a próxima iteração
  lastButtonState2 = buttonState2;
}

void Funcao1Butao1()
{
  //Codigo que executa a função 1 do botão 1
  Serial.println("Led 1 Ligado");
}

void Funcao2Butao1()
{
  //Codigo que executa a função 2 do botão 1
  Serial.println("Led 1 Desligado");
}
void Funcao1Butao2()
{
  //Codigo que executa a função 1 do botão 2
  Serial.println("Led 2 Ligado");
}

void Funcao2Butao2()
{
  //Codigo que executa a função 2 do botão 2
  Serial.println("Led 2 Desligado");
}
