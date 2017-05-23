void setup() {
  // put your setu  p code here, to run once:
  pinMode( 2, INPUT_PULLUP);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if( digitalRead( 2)==0) digitalWrite(13,1);
  else digitalWrite(13,0);
}
