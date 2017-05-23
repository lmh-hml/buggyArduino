void setup() {
  // put your setup code here, to run once:
  pinMode( 13,OUTPUT);
  pinMode( brake, INPUT_PULLUP);
  digitalWrite(13,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if( digitalRead(brake)==0)
  {
    digitalWrite(13,1);
  }
  else
  {
    digitalWrite(13,0);
  }
}
