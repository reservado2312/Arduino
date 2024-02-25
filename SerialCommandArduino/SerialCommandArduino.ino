#define LED_PIN 12 // Define o pino do LED

void setup() {
  pinMode(LED_PIN, OUTPUT); // Configura o pino do LED como saída
  Serial.begin(9600); // Inicia a comunicação serial
}

void loop() {
  if (Serial.available() > 0) { // Verifica se há dados disponíveis para leitura
    String comando = Serial.readStringUntil(' '); // Lê o comando enviado pela porta serial

    if (comando == "LED1") {
      String acao = Serial.readStringUntil('\n'); // Lê a ação (ON ou OFF)

      if (acao == "ON") {
        digitalWrite(LED_PIN, HIGH); // Liga o LED
        Serial.println("LED1 lIGADO");
      } else if (acao == "OFF") {
        digitalWrite(LED_PIN, LOW); // Desliga o LED
        Serial.println("LED1 Desligado");
      }
    }
  }
}
