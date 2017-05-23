
bool lastLoop=false;
bool isAuto = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( 6, OUTPUT);
  pinMode( 3, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  bool reading = digitalRead(3);
  if( reading != lastLoop && reading==0 )
  {
    isAuto = !isAuto;
    digitalWrite(6,isAuto);
  }
  
  int input = digitalRead(6);
  Serial.print(reading);   Serial.print(","); Serial.print(input); 
  Serial.print(","); Serial.println(isAuto);


   lastLoop = reading;

}
