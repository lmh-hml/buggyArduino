#include <buggy.h>

int nums[] = { 4,6,8,10};

void setup() {
  // put your setup code here, to run once:

    pinMode( 5,OUTPUT);
    pinMode( 11,OUTPUT);
    pinMode(9,OUTPUT);
    for(int i=0;i<4;++i)
    {
    pinMode(nums[i],OUTPUT);
    }
    Serial.begin(9600);
    digitalWrite(9,1);
}

void loop() {
  // put your main code here, to run repeatedly:
       if(Serial.available())
      {
        int input = Serial.parseInt();
        if( input>=0 )
        {
            for(int i=0;i<4;++i)
            {
              analogWrite(nums[i],nums[i]*10);
               Serial.println( (nums[i]*10*100) / 255);

            }
           analogWrite( 11, input);
        }
        else
        {
            for(int i=0;i<4;++i)
            {
              analogWrite(nums[i],0);
            }
          analogWrite( 5, 0);
          analogWrite( 11, 0);

        }
      }  
}
