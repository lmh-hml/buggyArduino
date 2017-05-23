#include <Servo.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <buggy.h>

typedef unsigned long uint32;

bool lastButtonState=false;
bool brakeCmd=false, receivedCmd_vel=false;
bool isAuto = false;
volatile double input=0;
char text[200];
volatile uint32 highTime=0, lowTime=0,fall_time=0, rise_time=0;
uint32 brakeTime=0, pwmTime=0;
int  pwm = 0;


ros::NodeHandle nh;
Servo brake_motor;


void getTwist(  const geometry_msgs::Twist& twist_msg)
{
  input = (twist_msg.linear.x<0.0) ? 0.0:twist_msg.linear.x;
  brakeCmd = twist_msg.linear.z;
  receivedCmd_vel=true;
}

ros::Subscriber<geometry_msgs::Twist> sub( "cmd_vel",getTwist);

int calculatePwm(double input)
{
  return (int)( 150.0 * input);
}

void setup() {
 // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( powerCoil, OUTPUT);
  pinMode( pwmPin,    OUTPUT);
  pinMode( lampCoil,OUTPUT);
  pinMode( button,INPUT_PULLUP);
  pinMode(speedPin,INPUT);
  attachInterrupt( digitalPinToInterrupt(speedPin), falling, FALLING);
  pinMode( brake, INPUT_PULLUP);
  brake_motor.attach(brakeMotor);
  brake_motor.write(BRAKE_STOP);
  pinMode(13,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
  delay(500);
  
  while( digitalRead(brake)==0)
  {
    brake_motor.write(80);
  }
  analogWrite(pwmPin,0);
  pwmTime = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  nh.spinOnce(); 
  int  brakeUp       = digitalRead(brake);
  int  brake_output  = brake_motor.read();
  bool buttonState   = digitalRead(button);

       
  if( buttonState != lastButtonState && buttonState==0 )
  {
    isAuto = !isAuto;
    digitalWrite(lampCoil,isAuto);
    digitalWrite(powerCoil,isAuto);   
    digitalWrite(13,isAuto);
  }
  

  if( receivedCmd_vel)
  { 
     pwm = (isAuto) ? calculatePwm(input) : 0;
      
     if(brakeCmd==false && !brakeUp)
     {
       brake_motor.write( BRAKE_UP);
     }
     else if( brakeCmd == true && brakeUp)
     {
        brake_motor.write( BRAKE_DOWN);
        pwm = 0;
     }
     else
     {
       brake_motor.write( BRAKE_STOP);
     }
     brakeTime = millis();
     receivedCmd_vel=false;
  }

  
  uint32 elapsed = millis()-brakeTime;
  if( ( brake_output< BRAKE_STOP && ( brakeUp==1 ||elapsed>=250)) ||  (brake_output>BRAKE_STOP && elapsed>=250)  )
  {
    brake_motor.write(BRAKE_STOP);
  }

  //check the width of the duty cycle every period; if its too high, there is no pwm coming from the speed signal
  if( (micros()-pwmTime) >= PWM_PERIOD && highTime!= NO_VALID_PWM_PERIOD)
  {
    //if( highTime > PWM_PERIOD ) highTime = NO_VALID_PWM_PERIOD;
    pwmTime= micros();
  }
  
  if( !nh.connected() )
  {
     pwm = 0;
  }

  sprintf(text,"PWM:%d, Speed:%lu, BMotor:%d,brake:%d,isAuto:%d\n", pwm, highTime,brake_output,brakeUp,isAuto);
  nh.loginfo(text);
  analogWrite(pwmPin,pwm);
  lastButtonState = buttonState;
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




