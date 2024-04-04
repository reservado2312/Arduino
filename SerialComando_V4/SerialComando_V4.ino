//Arduino Boards
//#include <Servo.h>

//ESP32
#include <ESP32Servo.h>

Servo servoMotor;

const int RELE_1 = 25;
const int RELE_2 = 26;
const int SERVO_PIN =215;

// Conector servoMotor
// D  VCC  GND 
// ^   +    -


void setup() {
  Serial.begin(9600);
  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(50);
}

void acionarRele(int pin, String acao) {
  if (acao == "ON") {
    digitalWrite(pin, HIGH);
  } else if (acao == "OFF") {
    digitalWrite(pin, LOW);
  } else {
    Serial.print("Erro: Ação ");
    Serial.print(acao);
    Serial.println(" inválida.");
  }
}

void controlarServo(String posicao) {
  int angulo = posicao.toInt();
  if (angulo >= 0 && angulo <= 180) {
    servoMotor.write(angulo);
  } else {
    Serial.println("Erro: Valor de posição inválido. Deve estar entre 0 e 180.");
  }
}

void RecebeComando() {
  String comando = Serial.readStringUntil(' ');

  if (comando == "RELE1") {
    acionarRele(RELE_1, Serial.readStringUntil('\n'));
  } else if (comando == "RELE2") {
    acionarRele(RELE_2, Serial.readStringUntil('\n'));
  } else if (comando == "SERVO") {
    controlarServo(Serial.readStringUntil('\n'));
  } else if (comando == "MSG") {
    String mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem: ");
    Serial.println(mensagem);
  } else {
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
