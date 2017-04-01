#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Serial.h"
#include "cinder/params/Params.h"




//http://www.mtahlers.de/index.php/robotik/omnivehicle

using namespace ci;
using namespace ci::app;
using namespace std;


#define BUFSIZE 380


class ominbotcontrolerApp : public App {
  public:
    
    
    
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseUp( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    
    void keyDown(KeyEvent)override;
    void keyUp(KeyEvent)override;
    
	void update() override;
	void draw() override;
    
    void updateVectors();
    void updatePID();
    void updateRotation();
    void drawVector(vec2 center, vec2 target);
    
    
    params::InterfaceGlRef	mParams;
    
    vec2 centerScreen;
    vec2 moveVector;
    bool mouseIsDown =false;
    
    vec2 motorVectorA;
    vec2 motorVectorB;
    vec2 motorVectorC;
    
    SerialRef	mSerial;
    
    string data ="w";
    int currentKey =-1;
    
    float kp =30.f;
    float ki=200.f;
    float kd=0.f;
    
    float kpR=0.01f;
    float kiR=0.f;
    float kdR=0.f;
    
    float rotation =0;
};

void ominbotcontrolerApp::setup()
{
    setWindowSize(1200, 600);
    
    mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( ivec2( 200, 300 ) ) );
    mParams->addParam( "rot", &rotation).min( 0.f ).max(359.f ).updateFn( [this] { updateRotation(); } );
    
    mParams->addSeparator();
    mParams->addParam( "p move", &kp).min( 0.f ).max( 500.f ).updateFn( [this] { updatePID(); } );
    mParams->addParam( "i move", &ki).min( 0.f ).max( 500.f ).updateFn( [this] { updatePID(); } );
    mParams->addParam( "d move", &kd).min( 0.f ).max( 500.f ).updateFn( [this] { updatePID(); } );
    mParams->addSeparator();
    mParams->addParam( "p rot", &kpR).min( 0.f ).max( 0.5f ).step(0.002f).updateFn( [this] { updatePID(); } );
    mParams->addParam( "i rot", &kiR).min( 0.f ).max( 0.5f ).step(0.002f).updateFn( [this] { updatePID(); } );
    mParams->addParam( "d rot", &kdR).min( 0.f ).max(0.5f ).step(0.002f).updateFn( [this] { updatePID(); } );
    
    
    
    centerScreen.x= getWindowWidth()/2;
     centerScreen.y= getWindowHeight()/2;
    
    
    for( const auto &dev : Serial::getDevices() )
        console() << "Device: " << dev.getName() << endl;
    
    try {
        Serial::Device dev = Serial::findDeviceByNameContains( "tty.usbmodem" );
        mSerial = Serial::create( dev, 115200);
     //   mSerial->readStringUntil( '\n', BUFSIZE );
    }
    catch( SerialExc &exc ) {
        console()<<"failed to connect to serial"<<endl;
    }

  
}
void ominbotcontrolerApp::keyUp( KeyEvent event )
{
    int keyCode =  event.getCode();
    if(keyCode ==264 || keyCode ==258 ||keyCode ==260 ||keyCode ==262 ||keyCode ==265 ||keyCode ==263 ||keyCode ==259 ||keyCode ==257 )
    {
        if(currentKey == keyCode){
            currentKey =-1;
            moveVector.x =0;
            moveVector.y =0;
            updateVectors();
        }

    
    }
}
void ominbotcontrolerApp::keyDown( KeyEvent event )
{
    
    int keyCode =  event.getCode();
    if(keyCode ==264 || keyCode ==258 ||keyCode ==260 ||keyCode ==262 ||keyCode ==265 ||keyCode ==263 ||keyCode ==259 ||keyCode ==257 )
    {
        if(currentKey != keyCode){
            currentKey =keyCode;
            moveVector.x =0;
            moveVector.y =0;
            if(currentKey== 264  || currentKey== 265  ||currentKey== 263  )
            {
                moveVector.y =-1;
            
            }
            else if(currentKey== 258 || currentKey== 259  ||currentKey== 257  )
            {
            
                moveVector.y =1;
            }
            
            if(currentKey== 262  || currentKey== 265  ||currentKey== 259  )
            {
                moveVector.x =1;
                
            }
            else if(currentKey== 260 || currentKey== 263  ||currentKey== 257  )
            {
                
                moveVector.x =-1;
            }
            moveVector  = glm::normalize(moveVector);
            moveVector *=100;
            updateVectors();
        }
    
    }
    //up 264
    //down 258
    //left  260
    //right 262
    
    //topR 265
    //topL 263
    //bottomR 259
    //bottomL 257
    
    
    
}
void ominbotcontrolerApp::mouseDown( MouseEvent event )
{
    mouseIsDown =true;
   moveVector = -centerScreen;
    moveVector.x +=event.getX();
    moveVector.y +=event.getY();
    updateVectors();

}
void ominbotcontrolerApp::mouseUp( MouseEvent event )
{
    mouseIsDown =false;
    moveVector.x =0;
    moveVector.y =0;
    updateVectors();

}
void ominbotcontrolerApp::mouseDrag( MouseEvent event )
{
    
    moveVector= -centerScreen;
    moveVector.x +=event.getX();
    moveVector.y +=event.getY();
    updateVectors();
}

void ominbotcontrolerApp::updatePID()
{

    if(mSerial)
    {
        mSerial->writeString("2:"+to_string(kp )+":"+to_string(ki  )+":"+to_string(kd )+"\n");
        mSerial->writeString("3:"+to_string(kpR )+":"+to_string(kiR  )+":"+to_string(kdR )+"\n");
    }


}
void ominbotcontrolerApp::updateRotation()
{
    if(mSerial)
    {
        console()<<"WWWWWWWWW";
        mSerial->writeString("1:"+to_string(rotation )+":"+to_string(0  )+":"+to_string(0)+"\n");
        
    }

}
void ominbotcontrolerApp::updateVectors()
{
    moveVector.x*=-1;
    motorVectorA =moveVector;
    
    float rad120 = 2*3.1415/3;
    motorVectorB.x=moveVector.x* cos(-rad120)-moveVector.y* sin(-rad120);
    motorVectorB.y=moveVector.x* sin(-rad120) +moveVector.y*cos(-rad120);
    
    
    motorVectorC.x = moveVector.x *cos(rad120)-moveVector.y*sin(rad120);
    motorVectorC.y = moveVector.x *sin(rad120)+moveVector.y*cos(rad120);
    if(mSerial)
    {
        mSerial->writeString("0:"+to_string(motorVectorA.x/100 )+":"+to_string(motorVectorB.x/100  )+":"+to_string(motorVectorC.x/100 )+"\n");
    
    }

}

void ominbotcontrolerApp::update()
{
    if(mSerial){
        if(mSerial->getNumBytesAvailable() >1)
        {
             data =     mSerial->readStringUntil( '\n', BUFSIZE );
            console()<<data<<endl;
        }
        
    }
    
    

}

void ominbotcontrolerApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    gl::pushMatrices();
    gl::translate(centerScreen);
    gl::enableAlphaBlending();
   
    
        
        float radius = 200;
        float rad120 = 2*3.1415/3;
        
        gl::color(1,1,0);
        drawVector(vec2(0,0),  moveVector);
        gl::color(0.4,0,0);
        drawVector(vec2(0,0),vec2( moveVector.x,0));
        gl::color(0,0.4,0);
       drawVector(vec2(0,0), vec2(0, moveVector.y));
        
        
    
        
        //draw A
        
        gl::pushMatrices();
        gl::translate(0,-radius);
        gl::color(0.2,0.2,0);
        drawVector(vec2(0,0), motorVectorA);
        gl::color(1,0,0);
        drawVector(vec2(0,0),vec2( motorVectorA.x,0));
       
        
        
        gl::color(0,0.2,0);
        drawVector(vec2(0,0), vec2(0,motorVectorA.y));
         gl::color(0.5,0.5,0.5);
        gl::drawStrokedRect(Rectf(-70,-12,70,12));
        
        gl::popMatrices();
        
        
        
          //draw B
        float  angle =(-90.f+120.f) /180*3.1415;

                gl::pushMatrices();
        gl::translate(vec2(cos(angle)*radius,sin(angle)*radius));
        gl::rotate(rad120,vec3(0,0,1));
        gl::color(0.2,0.2,0);
        drawVector(vec2(0,0), motorVectorB);
        gl::color(1,0,0);
        drawVector(vec2(0,0),vec2( motorVectorB.x,0));
        

        gl::color(0,0.2,0);
        drawVector(vec2(0,0), vec2(0,motorVectorB.y));
        
        gl::color(0.5,0.5,0.5);
        gl::drawStrokedRect(Rectf(-70,-12,70,12));
        gl::popMatrices();

        
        
        
        //drawC
        angle =(-90.f+120.f+120.f) /180*3.1415;
        gl::pushMatrices();
        gl::translate(vec2(cos(angle)*radius,sin(angle)*radius));
        gl::rotate(-rad120,vec3(0,0,1));
        gl::color(0.2,0.2,0);
        drawVector(vec2(0,0), motorVectorC);
        gl::color(1,0,0);
        drawVector(vec2(0,0),vec2( motorVectorC.x,0));
       
        gl::color(0,0.2,0);
        drawVector(vec2(0,0), vec2(0,motorVectorC.y));
        
        gl::color(0.5,0.5,0.5);
        gl::drawStrokedRect(Rectf(-70,-12,70,12));
        gl::popMatrices();
        
        
        
       
        
        
    
    
    gl::popMatrices();
    
    
    mParams->draw();
    
}
void ominbotcontrolerApp::drawVector(vec2 center, vec2 target)
{
   
    gl::drawLine(center, target);
    
    vec2 dir =glm::normalize(center-target);
    vec2 dirP ;
    dirP.x = -dir.y;
    dirP.y = dir.x;
    dirP*=5;
    
    dir*=8;
    
    
    vec2 p1 =target+dir-dirP ;
    vec2 p2 =target+dir+dirP ;
   
    gl::drawLine(target, p1);
    gl::drawLine(target, p2);
}
CINDER_APP( ominbotcontrolerApp, RendererGl() )
