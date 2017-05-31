
int pushButton1 = 48;
int pushButton2 = 50;

int buttonState1 =0 ;
int x1 =0;
int y1_ =0; //WTF y1 already defined???

int buttonState1prev =0 ;
int x1prev =0;
int y1prev =0;

int buttonState2 =0 ;
int x2 =0;
int y2 =0;

int buttonState2prev =0 ;
int x2prev =0;
int y2prev =0;


void setup() {
  Serial.begin(115200);
  pinMode(pushButton1, INPUT);
  pinMode(pushButton2, INPUT);
}




void loop() {

  
  delay(20);
  
   buttonState1 = digitalRead(pushButton1);
   x1 = analogRead(A0);
   y1_ = analogRead(A1);


  if(buttonState1 != buttonState1prev)
  {
    updateControler1();
  }
  else if (x1prev+5<x1 || x1prev-5 > x1)
  {
    updateControler1();
  }
  else if (y1prev+5<y1_ || y1prev-5 > y1_)
  {
    updateControler1();
  }


  delay(20);
  buttonState2 = digitalRead(pushButton2);
  x2 = analogRead(A2);
  y2 = analogRead(A3);

 
  if(buttonState2 != buttonState2prev)
  {
   
    updateControler2();
  }
  else if (x2prev+5<x2 || x2prev-5 > x2)
  {
    
    updateControler2();
  }
  else if (y2prev+5<y2 || y2prev-5 > y2)
  {
   
    updateControler2();
  }
  
  
} // Loop

void updateControler1()
{
  x1prev =x1;
  y1prev =y1_;
  buttonState1prev = buttonState1;

  Serial.print("0 ");
  Serial.print(x1);
  Serial.print(" ");
   Serial.print(y1_);
  Serial.print(" ");
   Serial.println(buttonState1);
  
  
}
void updateControler2()
{
  x2prev =x2;
  y2prev =y2;
  buttonState2prev = buttonState2;

  Serial.print("1 ");
  Serial.print(x2);
  Serial.print(" ");
  Serial.print(y2);
  Serial.print(" ");
  Serial.println(buttonState2);
  
  
}
