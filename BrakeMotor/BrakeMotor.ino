#include <Servo.h>
#include <buggy.h>
#include <ros.h>

ros::NodeHandle nh;
Servo myservo;
unsigned long ms;
int r=93;
bool startUp;
char text[200];



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(brake,INPUT_PULLUP);
  startUp=true;
  myservo.attach(brakeMotor);
  myservo.write(93);
  delay(500);
  while( digitalRead(brake)==0)
  {
    myservo.write(80);
  }
  //  nh.initNode();

}

void loop() {
  // put your main code here, to run repeatedly:
  //  nh.spinOnce();

  int b = digitalRead(brake);

  if( Serial.available())
  {
    r = Serial.parseInt();
    ms = millis();
      if( r>0 )
      {
          myservo.write(r);
      }
      else
      {
         myservo.write(93);
      }
  }
  
  unsigned long elapsed = millis()-ms;
  int val = myservo.read();
  if( ( val<93 && b==1) ||  (val>93 && elapsed>=250) )
  {
    myservo.write(93);
  }
 // sprintf(text,"brake: %d,\n",b);
 // nh.loginfo(text);
  

  Serial.print(val); Serial.print(",");
  Serial.print(b); Serial.print(",");
  Serial.println(elapsed);
}
