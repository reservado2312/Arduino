// Definição dos pinos
const int buttonPin = 2; // Pino do botão
const int ledPin = 13;   // Pino do LED

// Variáveis de estado
bool ledState = false;   // Estado inicial do LED
bool lastButtonState = HIGH; // Estado anterior do botão
bool buttonState;        // Estado atual do botão

void setup() {
  Serial.begin(9600);
  // Configura o pino do botão como entrada
  pinMode(buttonPin, INPUT);
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Lê o estado atual do botão
  buttonState = digitalRead(buttonPin);

  // Verifica se o botão foi pressionado
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Inverte o estado do LED
    ledState = !ledState;
    // Atualiza o estado do LED caso ledState seja verdadeio ou falso usando o
    // operador condicional ternário (variavel/função que retorne true ou false ? HIGH : FALSE)  para determinar o valor a ser escrito no pino do LED
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    ledState ? Funcao1() : Funcao2();
  }

  // Salva o estado atual do botão para a próxima iteração
  lastButtonState = buttonState;
}

void Funcao1()
{
  //Codigo que executa a função 1
  Serial.println("Led Ligado");
}

void Funcao2()
{
  //Codigo que executa a função 2
  Serial.println("Led Desligado");
}
