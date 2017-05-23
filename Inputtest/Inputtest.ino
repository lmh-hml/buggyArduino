int button = 3;
int brake = 8;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(brake, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = digitalRead(button);
  int j = digitalRead(brake);
  Serial.print(i);
  Serial.println(j);
}
