#define LED_PIN_1 13 // Pino do primeiro LED
#define LED_PIN_2 12 // Pino do segundo LED
#define LED_PIN_3 11 // Pino do terceiro LED

void setup() {
  pinMode(LED_PIN_1, OUTPUT); // Configura o pino do primeiro LED como saída
  pinMode(LED_PIN_2, OUTPUT); // Configura o pino do segundo LED como saída
  pinMode(LED_PIN_3, OUTPUT); // Configura o pino do terceiro LED como saída
  Serial.begin(9600); // Inicia a comunicação serial
}

void loop() {
  if (Serial.available() > 0) { // Verifica se há dados disponíveis para leitura
    String comando = Serial.readStringUntil(' '); // Lê o comando enviado pela porta serial

    String acao;
    int pin;

    // Debug: Imprime o comando recebido
    Serial.print("Comando recebido: ");
    Serial.println(comando);

    if (comando == "LED1" || comando == "LED2" || comando == "LED3") {
      acao = Serial.readStringUntil('\n'); // Lê a ação (ON ou OFF)
      //Exemplo LED1 ON ou LED1 OFF
      // Debug: Imprime a ação recebida
      Serial.print("Ação recebida: ");
      Serial.println(acao);

      // Determina qual LED deve ser controlado
      if (comando == "LED1") {
        pin = LED_PIN_1;
      } else if (comando == "LED2") {
        pin = LED_PIN_2;
      } else if (comando == "LED3") {
        pin = LED_PIN_3;
      }
      if (acao == "ON")
      {
        digitalWrite(pin, HIGH); // Liga o LED
      }
      else if(acao == "OFF")
      {
        digitalWrite(pin, LOW); // Desliga o LED
      }
      else
      {
        //Debug Caso não execute nada
        Serial.println("Erro ao receber ação: ");
        Serial.println(acao);
        Serial.println("No Pino: ");
        Serial.println(pin);
      }      
    }
  }
}
