
#include <Adafruit_CharacterOLED.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <string.h>

Adafruit_INA219 ina219;

Adafruit_CharacterOLED lcd(OLED_V2, 31, 33, 35, 37, 39, 41, 43);
char test[20];
void setup() 
{
  delay(100);
  
  lcd.begin(16, 4);

  delay(100);
 ina219.begin();
}

void loop() 
{
  String a ="";
  float current_mA = abs(ina219.getCurrent_mA())/1000;
  if(current_mA>0.01)
  {
    
    a+= String( current_mA/1000);
    a+=" Amp";
  //oled doesn't seam to work for me, use the buffer overflow... 
    for(int i =a.length(); i<128/2 ;i++)
    {
      a+=" "; 
    }
    float sensorValue = analogRead(A0);
    a+= String( sensorValue/27.777);
    a+=" Volt";

    for(int i =a.length(); i<128 ;i++)
    {
      a+=" "; 
    }
   
   }else
   {
    a= "waiting";
     for(int i =a.length(); i<128 ;i++)
    {
      a+=" "; 
    }
    
   }
    lcd.print(a);
  //th is a fake comment

   delay(100);
  
  
  



}
