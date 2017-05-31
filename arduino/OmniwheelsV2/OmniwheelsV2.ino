
#include <PID_v1.h>
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include "RF24Network.h"


bool isRobot1 =true;


RF24 radio(44,45);
RF24Network network(radio);



struct RadioData{
  int type;
  float value1;
  float value2;
  float value3;
}radioData;


int batteryPin =49;
int onPin =51;
int dataPin =43;


int motorEnablePin =8;

int pmwPin1 =11;
int dirPin1B =10;
int dirPin1A =9;

int pmwPin2=7;
int dirPin2B=6;
int dirPin2A=5;

int pmwPin3=2;
int dirPin3B=4;
int dirPin3A=3;


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
int motorUpdateTime =0;
int batteryUpdateTime =0;
int count =0;


const uint16_t base_node = 00;        // Address of our node in Octal format
const uint16_t robot1 = 01;       // Address of the other node in Octal format
const uint16_t robot2 = 05;
 
void setup() {

  Serial.begin(115200);

  SPI.begin();
  radio.begin();
  if(isRobot1)
  {
      network.begin(/*channel*/ 90, /*node address*/robot1);
    
  }else
  {
    
      network.begin(/*channel*/ 90, /*node address*/ robot2);
  }
 

 
  

  
  
  pinMode(batteryPin, OUTPUT);
  pinMode(onPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(onPin, HIGH);
  digitalWrite(batteryPin, HIGH);
  digitalWrite(dataPin, LOW);
  
  pinMode(encoder1A, INPUT);
  pinMode(encoder1B, INPUT);
  pinMode(encoder2A, INPUT);
  pinMode(encoder2B, INPUT);
  pinMode(encoder3A, INPUT);
  pinMode(encoder3B, INPUT);

  pinMode(dirPin1A, OUTPUT);
  pinMode(dirPin1B, OUTPUT);
  pinMode(pmwPin1, OUTPUT);
  pinMode(dirPin2A, OUTPUT);
  pinMode(dirPin2B, OUTPUT);
  pinMode(pmwPin2, OUTPUT);
  pinMode(dirPin3A, OUTPUT);
  pinMode(dirPin3B, OUTPUT);
  pinMode(pmwPin3, OUTPUT);
  
  digitalWrite(dirPin1A, 0);
  digitalWrite(dirPin2A, 0);
  digitalWrite(dirPin3A, 0);
  digitalWrite(dirPin1B, 0);
  digitalWrite(dirPin2B, 0);
  digitalWrite(dirPin3B, 0);
  analogWrite(pmwPin1,0);
  analogWrite(pmwPin2,0);
  analogWrite(pmwPin3,0);

  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorEnablePin, HIGH);
  
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
  batteryUpdateTime+= timeElapsed;

  if(motorUpdateTime>10000)
  {
  
      unsigned long error =123;

  
      count++;
      float secEllpased = motorUpdateTime/1000000.f;
      float speedFactor = (secEllpased*  64.f*43.7f);
      motorUpdateTime -=10000;

  
       OutputR =0;
     
      
     
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
           digitalWrite(dirPin1A, 0);
           digitalWrite(dirPin1B, 1);
           analogWrite(pmwPin1,-currentPwm1 );
      }else
      {
          digitalWrite(dirPin1A, 1);
          digitalWrite(dirPin1B, 0);  
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
           digitalWrite(dirPin2A, 0);
           digitalWrite(dirPin2B, 1);
           analogWrite(pmwPin2,-currentPwm2 );
      }else
      {
          digitalWrite(dirPin2A, 1);
          digitalWrite(dirPin2B, 0);      
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
           digitalWrite(dirPin3A, 0);
           digitalWrite(dirPin3B, 1);
           analogWrite(pmwPin3,-currentPwm3 );
           
      }else
      {
           digitalWrite(dirPin3A, 1);
           digitalWrite(dirPin3B, 0);
          analogWrite(pmwPin3,currentPwm3 );
          
      }

     
      if(currentPwm3==0 && currentPwm2==0 && currentPwm1==0 )
      {
        
        digitalWrite(motorEnablePin, 0);
      }else
      {
        
         digitalWrite(motorEnablePin, 1);
      }
       
   }
   else if ( batteryUpdateTime>10000000)
   {
      batteryUpdateTime =0;
      float voltage = analogRead(A0)/28.2;


      if(voltage<10.5)
      {
        digitalWrite(batteryPin, HIGH);
      }else 
      {
        digitalWrite(batteryPin, LOW);
        
       }
     
   }
   
    digitalWrite(dataPin,LOW );
    handleSerialInput();



  
  

}

void handleSerialInput()
{
    network.update(); 
    while ( network.available() ) 
    {     // Is there anything ready for us?
    
      RF24NetworkHeader header;        // If so, grab it and print it out
     
      network.read(header,&radioData, sizeof(radioData));
     
     
       digitalWrite(dataPin, HIGH );
       
      
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
         Serial.println( "setMotor");
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

