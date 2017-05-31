

#include <SPI.h>
#include "RF24.h"
#include "RF24Network.h"





RF24 radio(52,53);
RF24Network network(radio);

const uint16_t this_node = 00;        // Address of our node in Octal format
const uint16_t robot1 = 01;       // Address of the other node in Octal format
const uint16_t robot2 = 05;

struct RadioData{
  int type;
  float value1;
  float value2;
  float value3;
}radioData;

int valX=0;
int valY=0;
int valBtn =0;

unsigned long previousTime=0;
int testControlesTime =0;

void setup() {

  Serial.begin(115200);
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

}




void loop() {

  
    unsigned long currentTime = micros();  
    unsigned long timeEllapsed =  currentTime-previousTime;
    previousTime =currentTime;
    network.update(); 
    
    while ( network.available() ) 
    {     // Is there anything ready for us?
    
      RF24NetworkHeader header;        // If so, grab it and print it out
      int payload;
      network.read(header,&payload,sizeof(int));
     
      Serial.println(payload);
    }
  

  
    if (Serial.available() > 6) 
    {
    
        String target = Serial.readStringUntil(':');
        String commandS = Serial.readStringUntil(':');
        String val1S = Serial.readStringUntil(':');
        String val2S = Serial.readStringUntil(':');
        String val3S = Serial.readStringUntil('\n');
     
        radioData.type =commandS.toInt();
        radioData.value1 =val1S.toFloat();
        radioData.value2 =val2S.toFloat();
        radioData.value3 =val3S.toFloat();


        if(target=="1")
        {
          RF24NetworkHeader header(/*to node*/ robot1);
          bool ok = network.write(header,&radioData,sizeof(radioData));
          if (ok)
            Serial.println("11");
          else
            Serial.println("10");
        }
        else 
        {
          RF24NetworkHeader header(/*to node*/ robot2);
          bool ok = network.write(header,&radioData,sizeof(radioData));
          if (ok)
            Serial.println("21");
          else
            Serial.println("20");
      
         }
  
    }
  
} // Loop
