#include <Servo.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <buggy.h>

typedef unsigned long uint32;


int buttonCount=0;
bool lastLoop=false;
bool brakes=false, receivedCmd_vel=false;
volatile bool isAuto = false;
volatile double input=0, limit=0;
char text[200];
volatile uint32 highTime=0, lowTime=0,fall_time=0, rise_time=0;
uint32 brakeTime=0;

ros::NodeHandle nh;
Servo brake_motor;


void getTwist(  const geometry_msgs::Twist& twist_msg)
{
  input = twist_msg.linear.x;
  isAuto = twist_msg.linear.y; 
  brakes = twist_msg.linear.z;
  receivedCmd_vel=true;
  
}

ros::Subscriber<geometry_msgs::Twist> sub( "cmd_vel",getTwist);

int calculatePwm(double input)
{
  return (int)( 130.0 * input);
}

void setup() {
 // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( powerCoil, OUTPUT);
  pinMode( pwmPin,    OUTPUT);
  pinMode( lampCoil,OUTPUT);
  pinMode( button,INPUT_PULLUP);
  pinMode(speedPin,INPUT);
  attachInterrupt( digitalPinToInterrupt(speedPin), rising, RISING);
  pinMode( brake, INPUT_PULLUP);
  brake_motor.attach(brakeMotor);
  brake_motor.write(93);
  pinMode(13,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
 delay(500);
  while( digitalRead(brake)==0)
  {
    brake_motor.write(80);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  bool reading = digitalRead(button);
  int  b       = digitalRead(brake);
  int  val     = brake_motor.read();
  int pwm = 0;

/*  if( reading != lastLoop && reading==0)
  {
    isAuto = !isAuto;
    digitalWrite(lampCoil,isAuto);
  }*/

  digitalWrite(lampCoil,isAuto);

  if( isAuto )
  {
      digitalWrite(powerCoil,1);   
      digitalWrite(13,1);
      pwm = calculatePwm(input);
      if(!b) 
      {
        pwm =0;
      }
      analogWrite( pwmPin, pwm );
  }
  else
  {
    digitalWrite(pwmPin,0);
    digitalWrite(13,0);
    digitalWrite(powerCoil,0);
  }

  if( receivedCmd_vel)
  {
     if(brakes==0.0 && b==0)
     {
       brake_motor.write( 70);
     }
     else if( brakes == 1.0 && b==1)
     {
        brake_motor.write( 120);
     }
     else
     {
       brake_motor.write( 93);
     }
     brakeTime = millis();
     receivedCmd_vel=false;
  }
  
  uint32 elapsed = millis()-brakeTime;
  if( ( val<93 && (b==1 ||elapsed>=250)) ||  (val>93 && elapsed>=250)  )
  {
    brake_motor.write(93);
  }

  uint32 period = highTime+lowTime;
  uint32 hz = (period == 0) ? 0: 1000000/period;
  sprintf(text,"PWM:%d, Speed:%d, BMotor:%d,brake:%d,isAuto:%d\n",pwm, hz,val,digitalRead(brake),reading);
  nh.loginfo(text);
 //lastLoop = reading;
}

void rising() 
{
  rise_time = micros();
  lowTime = rise_time-fall_time;
  attachInterrupt(digitalPinToInterrupt(speedPin), falling, FALLING);

}

void falling() {
  fall_time = micros();
  highTime = fall_time-rise_time;
  attachInterrupt(digitalPinToInterrupt(speedPin), rising, RISING);
}

int serialInput()
{
        if(Serial.available())
      {
        input = Serial.parseInt();
        if( input> 0 )
        {

          return input;
        }
        else
        {
          return 0;
        }
      }  
}


