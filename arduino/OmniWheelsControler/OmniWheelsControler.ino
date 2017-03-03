
/*
* Getting Started example sketch for nRF24L01+ radios
* This is an example of how to send data from one node to another using data structures
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

byte addresses[][6] = {"1Node","2Node"};


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(52,53);
/**********************************************************/



/**
* Create a data structure for transmitting and receiving data
* This allows many variables to be easily sent and received in a single transmission
* See http://www.cplusplus.com/doc/tutorial/structures/
*/
struct RadioData{
  int type;
  float value1;
  float value2;
  float value3;
}radioData;

void setup() {

  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted_HandlingData"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
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
  
  radioData.type = 0;
   radioData.value1 = 0;
    radioData.value2 = 0;
     radioData.value3 = 0;
  // Start the radio listening for data
  radio.startListening();
}




void loop() {
  
 if (Serial.available() > 0) {
    
    
      String commandS = Serial.readStringUntil(':');
      String val1S = Serial.readStringUntil(':');
      String val2S = Serial.readStringUntil(':');
      String val3S = Serial.readStringUntil('\n');
     
      radioData.type =commandS.toInt();
      radioData.value1 =val1S.toFloat();
      radioData.value2 =val2S.toFloat();
      radioData.value3 =val3S.toFloat();
    
      radio.stopListening();                                    // First, stop listening so we can talk.
    Serial.println("s");
     if (!radio.write( & radioData, sizeof( radioData) )){
       Serial.println(F("failed"));
     }   
    radio.startListening();                                    // Now, continue listening
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
             int a ;                                                  // Grab the response, compare, and send to debugging spew
        radio.read( &a, sizeof(int) );
        unsigned long time = micros();
        
        // Spew it
       
        Serial.println(a);
     
      
    }

 }
  





} // Loop
