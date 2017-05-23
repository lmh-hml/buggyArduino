#include <Servo.h>

#include <buggy.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(brake,INPUT_PULLUP);
  digitalWrite( 13,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  int r = digitalRead(brake);
  if( r==0)
  {
    digitalWrite( 13,1);
  }
  else
  {
    digitalWrite( 13,0);
  }
  Serial.println(r);
}
