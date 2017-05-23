typedef unsigned long uint64 ;
volatile unsigned long rise_time, fall_time,r_interval=0,f_interval=0;
volatile unsigned long rise=0, fall=0;
 unsigned long  hz =0,waiting=0;
 
 
 void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, INPUT);
  attachInterrupt( digitalPinToInterrupt(3), rising, RISING); 
  Serial.println(digitalRead(3));
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(1000000/interval);
  waiting = micros();
  if(waiting - rise_time > 100000 && waiting - fall_time >100000)
  {
    rise_time = 0; fall_time=0;
  }
    hz = (r_interval>0&&f_interval>0)? 1000000/(r_interval+f_interval):0;
  Serial.println(f_interval);
 /*
 int val = pulseIn(2,LOW);
 Serial. print(val);Serial.println(" ms");*/

}

void rising()
{
  rise_time = micros();
  r_interval = rise_time-fall_time;
  attachInterrupt(digitalPinToInterrupt(3), falling, RISING);

}

void falling()
{
  fall_time = micros();
  f_interval = fall_time - rise_time;
  attachInterrupt(digitalPinToInterrupt(3), rising, FALLING);
}

