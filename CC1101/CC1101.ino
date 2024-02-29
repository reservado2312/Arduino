#include <ELECHOUSE_CC1101.h>
byte buffer[100] = {0};

void setup()
{
  Serial.begin(9600);
  Serial.println("Rx");
  ELECHOUSE_cc1101.Init(F_277);
  ELECHOUSE_cc1101.SetReceive();
}

void loop()
{
  if (ELECHOUSE_cc1101.CheckReceiveFlag()) {
    ELECHOUSE_cc1101.ReceiveData(buffer);
    String str((char*) buffer);
    Serial.println(str);
    ELECHOUSE_cc1101.SetReceive();
  }
}
