
#include <PID_v1.h>
#include <Wire.h>
#include <LSM303.h>

#include <SPI.h>
#include "RF24.h"

byte addresses[][6] = {"1Node","2Node"};
bool radioNumber = 1;
RF24 radio(52,53);
struct RadioData{
  int type;
  float value1;
  float value2;
  float value3;
}radioData;

LSM303 compass;


int dirPin1 =3;
int pmwPin1 =2;
int dirPin2=5;
int pmwPin2=4;
int dirPin3=7;
int pmwPin3=6;


volatile int motor1Count = 0;
int encoder1A =22;
int encoder1B =23;

volatile int motor2Count = 0;
int encoder2A =24;
int encoder2B =25;

volatile int motor3Count = 0;
int encoder3A =26;
int encoder3B =27;

double Kp=30.00, Ki=200.00, Kd=00.00;

double SetPointRotate;
double targetRotate;
double Setpoint1, Input1, Output1,SetpointMove1;
PID pid1(&Input1, &Output1, &Setpoint1, Kp, Ki, Kd, DIRECT);
double Setpoint2, Input2, Output2,SetpointMove2;
PID pid2(&Input2, &Output2, &Setpoint2, Kp, Ki, Kd, DIRECT);
double Setpoint3, Input3, Output3,SetpointMove3;
PID pid3(&Input3, &Output3, &Setpoint3, Kp, Ki, Kd, DIRECT);

double SetpointR, InputR, OutputR;
double KpR=0.01, KiR=0.00, KdR=00.00;
PID pidR(&InputR, &OutputR, &SetpointR, KpR, KiR, KdR, DIRECT);

unsigned long previousMicros;
float motorUpdateTime =0;

int count =0;


 
void setup() {

  Serial.begin(115200);

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
   radio.startListening();


  //Wire.begin();
  //compass.init();
  //compass.enableDefault();
 
  //compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  //compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};



  
  pinMode(encoder1A, INPUT);
  pinMode(encoder1B, INPUT);
  pinMode(encoder2A, INPUT);
  pinMode(encoder2B, INPUT);
  pinMode(encoder3A, INPUT);
  pinMode(encoder3B, INPUT);

  pinMode(dirPin1, OUTPUT);
  pinMode(pmwPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(pmwPin2, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(pmwPin3, OUTPUT);
  digitalWrite(dirPin1, 0);
  digitalWrite(dirPin2, 0);
  digitalWrite(dirPin3, 0);
  analogWrite(pmwPin1,0);
  analogWrite(pmwPin2,0);
  analogWrite(pmwPin3,0);
   
  attachInterrupt(encoder1A, motor1interuptA, CHANGE);
  attachInterrupt(encoder1B, motor1interuptB, CHANGE);
  attachInterrupt(encoder2A, motor2interuptA, CHANGE);
  attachInterrupt(encoder2B, motor2interuptB, CHANGE);
  attachInterrupt(encoder3A, motor3interuptA, CHANGE);
  attachInterrupt(encoder3B, motor3interuptB, CHANGE);
  
  previousMicros =0;
  targetRotate = 0;
  

  pid1.SetMode(AUTOMATIC);
  pid1.SetOutputLimits(-255,255);
  Setpoint1 = 0;
  SetpointMove1 = 0;
  
  
  pid2.SetMode(AUTOMATIC);
  pid2.SetOutputLimits(-255,255);
  Setpoint2 = 0;
  SetpointMove2 = 0;

 
  pid3.SetMode(AUTOMATIC);
  pid3.SetOutputLimits(-255,255);
  Setpoint3 = 0;
  SetpointMove3 = 0;

  pidR.SetMode(AUTOMATIC);
  pidR.SetOutputLimits(-1.5,1.5);
  
  analogWrite(pmwPin1, 0);
  analogWrite(pmwPin2, 0);
  analogWrite(pmwPin3, 0);
  Serial.println("READY");
  //radio.printDetails();
  
}

void loop() {

  unsigned long currentMicros = micros();
  float timeElapsed = currentMicros- previousMicros;
  previousMicros = currentMicros;

  motorUpdateTime+= timeElapsed;

  if(motorUpdateTime>10000)
  {
   // radio.stopListening();
    unsigned long error =123;
    //radio.write( &error, sizeof( unsigned long) );  
   // radio.startListening();
  
      count++;
      float secEllpased = motorUpdateTime/1000000.f;
      float speedFactor = (secEllpased*  64.f*43.7f);
      motorUpdateTime -=10000;

    /* compass.read();
      float heading = compass.heading();
      float headingInv = heading- 360;
      if(abs(heading-targetRotate) >abs(headingInv-targetRotate))
      {
        heading  =headingInv;
      }


      SetpointR += (heading-SetpointR)/2.f  ;
      InputR = targetRotate;
      pidR.Compute();
*/
       OutputR =0;
     // Serial.print(  targetRotate);
      //Serial.print(  " ");
      //Serial.println( SetpointR);
     
      //Serial.println(OutputR);
      //
      // update motor1
      //

      Setpoint1 = SetpointMove1+OutputR ;
      
      Input1  = (float)motor1Count/   speedFactor ;
      motor1Count =0;
      
      pid1.Compute();
      int currentPwm1 = Output1;
      
      if(currentPwm1<0)
      {
           digitalWrite(dirPin1, 0);
           // Serial.println(-currentPwm1);
           analogWrite(pmwPin1,-currentPwm1 );
      }else
      {
          digitalWrite(dirPin1, 1);
            //Serial.println(currentPwm1);
          analogWrite(pmwPin1,currentPwm1 );
      }

      //
      // update motor2
      //
      Setpoint2 = SetpointMove2+OutputR ;
      
      Input2  = (float)motor2Count/   speedFactor ;
      motor2Count =0;
      
      pid2.Compute();
      int currentPwm2 = Output2;
      
      if(currentPwm2<0)
      {
           digitalWrite(dirPin2, 0);
           // Serial.println(-currentPwm2);
           analogWrite(pmwPin2,-currentPwm2 );
      }else
      {
          digitalWrite(dirPin2, 1);
           // Serial.println(currentPwm2);
          analogWrite(pmwPin2,currentPwm2 );
      }


      //
      // update motor3
      //
      Setpoint3 = SetpointMove3+OutputR ;
      
      Input3  = (float)motor3Count/   speedFactor ;
      motor3Count =0;
      
      pid3.Compute();
      int currentPwm3 = Output3;
      
      if(currentPwm3<0)
      {
           digitalWrite(dirPin3, 0);
          // Serial.println(-currentPwm3);
           analogWrite(pmwPin3,-currentPwm3 );
      }else
      {
          digitalWrite(dirPin3, 1);
           //Serial.println(currentPwm3);
          analogWrite(pmwPin3,currentPwm3 );
      }

     
      
       
   }

    handleSerialInput();



  
  

}

void handleSerialInput()
{
   //Serial.println("radi");
    if( radio.available())
    {                                                    
      while (radio.available()) {      // While there is data ready

        Serial.println("DATA");
         //radio.read( &radioData, sizeof(radioData) );
       //  unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        //radio.read( &got_time, sizeof(unsigned long) );
        //return;
        radio.read( &radioData, sizeof(radioData) );             // Get the payload
      }
      if(radioData.type==0)
      {
        
          SetpointMove1 =radioData.value1;
          SetpointMove2 =radioData.value2;
          SetpointMove3 =radioData.value3;
       }
       else if(radioData.type==1)
       {
          targetRotate =radioData.value1;
       }
        else if(radioData.type==2)
       {
          Kp =radioData.value1;
          Ki =radioData.value2;
          Kd =radioData.value3;
          pid1.SetTunings(Kp, Ki, Kd);
          pid2.SetTunings(Kp, Ki, Kd);
          pid3.SetTunings(Kp, Ki, Kd);
       }
        else if(radioData.type==3)
       {
          KpR =radioData.value1;
          KiR =radioData.value2;
          KdR =radioData.value3;
          pidR.SetTunings(KpR, KiR, KdR);
       }
     radio.stopListening();                               // First, stop listening so we can talk  
      //int error =0;                      
      //radio.write( &error, sizeof(int) );              // Send the final one back.      
      radio.startListening();    
    }
   if (Serial.available() > 0) {
    
    
       String commandS = Serial.readStringUntil(':');
      String val1S = Serial.readStringUntil(':');
      String val2S = Serial.readStringUntil(':');
      String val3S = Serial.readStringUntil('\n');
     
      radioData.type =commandS.toInt();
      radioData.value1 =val1S.toFloat();
      radioData.value2 =val2S.toFloat();
      radioData.value3 =val3S.toFloat();

      if(radioData.type==0)
      {
          SetpointMove1 =radioData.value1;
          SetpointMove2 =radioData.value2;
          SetpointMove3 =radioData.value3;
       }
       else if(radioData.type==1)
       {
          targetRotate =radioData.value1;
       }
        else if(radioData.type==2)
       {
          Kp =radioData.value1;
          Ki =radioData.value2;
          Kd =radioData.value3;
          pid1.SetTunings(Kp, Ki, Kd);
          pid2.SetTunings(Kp, Ki, Kd);
          pid3.SetTunings(Kp, Ki, Kd);
       }
        else if(radioData.type==3)
       {
          KpR =radioData.value1;
          KiR =radioData.value2;
          KdR =radioData.value3;
          pidR.SetTunings(KpR, KiR, KdR);
       }
   
    }
  
 }


 
////
////interupts
//// 
void motor1interuptA()
{
    if (digitalRead(encoder1A) != digitalRead(encoder1B))
    {
      motor1Count--;
    }
    else  
    {
      motor1Count++;
    }
 }
 void motor1interuptB()
{
    if (digitalRead(encoder1A) != digitalRead(encoder1B))
    {
      motor1Count++;
    }
    else  
    {
       motor1Count--;
    }
 }

 void motor2interuptA()
{
    if (digitalRead(encoder2A) != digitalRead(encoder2B))
    {
      motor2Count--;
    }
    else  
    {
      motor2Count++;
    }
 }
 void motor2interuptB()
{
    if (digitalRead(encoder2A) != digitalRead(encoder2B))
    {
      motor2Count++;
    }
    else  
    {
       motor2Count--;
    }
 }
void motor3interuptA()
{
    if (digitalRead(encoder3A) != digitalRead(encoder3B))
    {
      motor3Count--;
    }
    else  
    {
      motor3Count++;
    }
 }
 void motor3interuptB()
{
    if (digitalRead(encoder3A) != digitalRead(encoder3B))
    {
      motor3Count++;
    }
    else  
    {
       motor3Count--;
    }
 }

