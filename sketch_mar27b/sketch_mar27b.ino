#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>

double input=0.0;
ros::NodeHandle nh;
int pwmPin= 11;
char text[10];

void getTwist(  const geometry_msgs::Twist& twist_msg)
{
  input = twist_msg.linear.x;
  
}

ros::Subscriber<geometry_msgs::Twist> sub( "cmd_vel",getTwist);

int calculatePwm(double input)
{
  return (int)( 130.0 * input);
}

void setup() {
  // put your setup code here, to run once:
  pinMode( pwmPin,    OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(9,1);
    nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  int pwm = calculatePwm(input);
  sprintf(text,"%d\n", pwm);
    nh.loginfo(text);

  analogWrite(pwmPin,pwm);
}
