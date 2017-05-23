#include <buggy.h>

volatile bool lastLoop=false;
volatile bool isAuto;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
    pinMode( lampCoil,OUTPUT);

  attachInterrupt( digitalPinToInterrupt(button), change, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  int r = digitalRead(button);
  Serial.println(r);
}

void change()
{
  int reading = digitalRead(button);
  if( reading != lastLoop && reading==0)
  {
    isAuto = !isAuto;
    digitalWrite(lampCoil,isAuto);
  }
   lastLoop = reading;

}

