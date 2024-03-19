void setup() {
  pinMode(2, OUTPUT); // Inicializa o pino D4 (GPIO 2)  como uma saída
}
 
void loop() {
  digitalWrite(2, HIGH); // Coloca o pino D4 (GPIO 2) em nível alto
  delay(1000); // Aguarda 1 segundo
  digitalWrite(2, LOW); // Coloca o pino D4 (GPIO 2) em nível baixo
  delay(1000); // Aguarda 1 segundo
}
