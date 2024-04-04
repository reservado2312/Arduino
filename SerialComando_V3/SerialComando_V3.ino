
// Reles  --------------------------------------------------------------
// Use constantes para determinar o numero que indica os Pinos dos Reles 
// pois é um valor que não sera alterado durante a execução do código 
// garantindo assim a integridade do valor da variavel
const int RELE_1 = 25;
const int RELE_2 = 26;

void setup() {
  Serial.begin(9600);
  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
}

void acionarRele(int pin, bool ligar) {
  digitalWrite(pin, ligar ? HIGH : LOW);
}

void RecebeComando() {
  //Recebe da serial determinado comando
  //ex: RELE1
  String comando = Serial.readStringUntil(' ');

  if (comando == "RELE1") 
  {
    //chama a função acionarRele passando o Rele e Se seraligado ou na 
    acionarRele(RELE_1, Serial.readStringUntil('\n') == "ON");

  } 
  else if (comando == "RELE2") 
  {
    acionarRele(RELE_2, Serial.readStringUntil('\n') == "ON");
  } 
  else if (comando == "MSG") 
  {
    String mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem: ");
    Serial.println(mensagem);
  } 
  else 
  {
    Serial.print("Comando ");
    Serial.print(comando);
    Serial.println(" inexistente!");
  }
}

void loop() {
  if (Serial.available()) {
    RecebeComando();
  }
}
