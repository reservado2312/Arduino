#define LED_PIN 2 // Pino do Primeiro LED
#define RELE_1 26 // Pino do Primeito Rele
#define RELE_2 25 // Pino do Segundo Rele

void setup() {
  pinMode(LED_PIN, OUTPUT); // Configura o Pino do LED como saída
  pinMode(RELE_1, OUTPUT); // Configura o Pino do Primeito Rele como saída
  pinMode(RELE_2, OUTPUT); // Configura o Pino do Segundo Rele como saída
  Serial.begin(9600); // Inicia a comunicação serial
}

void loop() {
  if (Serial.available() > 0) 
  { // Verifica se há dados disponíveis para leitura
    ChamadaComando();
  }
}

void ChamadaComando()
{
    String comando = Serial.readStringUntil(' '); // Lê o comando enviado pela porta serial
    String acao; //Recebe acao do comando que Liga ou Desliga algo
    //Exemplo led: LED ON ou LED OFF
    //Exemplo rele: RELE1 ON ou RELE1 OFF
    //Exemplo msg: MSG Sua Menssagem!!
    int pin;

    // Debug: Imprime o comando recebido
    Serial.print("Comando recebido: ");
    Serial.println(comando);

    if (comando == "LED" || comando == "RELE1" || comando == "RELE2") {
      acao = Serial.readStringUntil('\n'); // Lê a ação (ON ou OFF)
      
      // Debug: Imprime a ação recebida
      Serial.print("Ação recebida: ");
      Serial.println(acao);

      // Determina qual LED deve ser controlado
      if (comando == "LED") {
        pin = LED_PIN;
      } else if (comando == "RELE1") {
        pin = RELE_1;
      } else if (comando == "RELE2") {
        pin = RELE_2;
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
        //Debug Caso não execute nada ou a ação não exista!
        Serial.println("Erro ao receber ação: ");
        Serial.println(acao);
        Serial.println("No Pino: ");
        Serial.println(pin);
      }      
    }
    else if (comando == "MSG") 
    {
      String mensagem = Serial.readStringUntil('\n'); // Lê a mensagem

      // Imprime a mensagem
      Serial.print("Mensagem: ");
      Serial.print(mensagem);
    }
    else
    {
      //Debug que Trata comando inexistente
      Serial.println("Comando: ");
      Serial.println(comando);
      Serial.println("Inexistente!");
    }
}
