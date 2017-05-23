#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Serial.h"
#include "cinder/params/Params.h"

#include "CameraHandler.hpp"
#include "ArduinoHandler.hpp"
#include "Player.hpp"
#include "ParticleHandler.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class Game_PCApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseMove( MouseEvent event ) override;
    void keyDown( KeyEvent event ) override;
	void update() override;
	void draw() override;
    void setupParams();
    void updateRobotParams();
    CameraHandler cameraHandler;
    ArduinoHandler arduinoHandlerInput;
     ArduinoHandler arduinoHandlerOutput;
    PlayerRef player1;
    PlayerRef player2;
    
    ParticleHandler particleHandler;

    params::InterfaceGlRef	mParams;
    
    float mCircleOffX =695 ;
    float robotSize =50;
   
 
    bool debugView =false;
    bool useCameraPositioning =true;
    vec2 mousePos;
    
    float   mFrameRate;
};

void Game_PCApp::setup()
{
    //projector size = 1280 720
    //camera size =640*480;
    
    setWindowSize(1280, 720);
    
    
    for( const auto &dev : Serial::getDevices() )
        console() << "Device: " << dev.getName() << endl;
    
    
    cameraHandler.setup("tty.usbserial-A403JIFZ");
    arduinoHandlerOutput.setup("tty.usbmodem14111");
    arduinoHandlerInput.setup("tty.usbmodem14141");
    setupParams();
    
   
    
    player1 =Player::create();
    player2 =Player::create();

    arduinoHandlerInput.player1 = player1;
    arduinoHandlerInput.player2 = player2;
   
    particleHandler.setup(720/2, vec2(mCircleOffX,720/2));
    
    updateRobotParams();
    
    
   
}
void Game_PCApp::keyDown( KeyEvent event )
{

    if(event.getCode() == KeyEvent::KEY_f)
    {
        setFullScreen(!isFullScreen());
    
    }
    if(event.getCode() == KeyEvent::KEY_d)
    {
        debugView =!debugView;
        
    }
    if(event.getCode() == KeyEvent::KEY_r)
    {
        particleHandler.reset();
        
    }

};
void Game_PCApp::mouseMove( MouseEvent event )
{
    
   mousePos =  event.getPos();
    
    
}
void Game_PCApp::mouseDown( MouseEvent event )
{
}

void Game_PCApp::update()
{
   
      
    cameraHandler.update();
    arduinoHandlerInput.update();
    if(useCameraPositioning )
    {
        if(cameraHandler.newPos){
        cameraHandler.newPos =false;
            player1->setPosition(cameraHandler.currentPosition,cameraHandler.currentDirection);
        }
    }
    else
    {
        player1->setPosition(vec4(mousePos.x,mousePos.y,0,1),vec4(1,0,0,1));
    }
    
    player1->update();
    if(player1->hasNewCommand)
    {
        player1->hasNewCommand =false;
        arduinoHandlerOutput.sendCommand(player1->command);
    }
    
    particleHandler.update(player1);
    mFrameRate = getAverageFps();
}

void Game_PCApp::draw()
{
	gl::clear( Color( 0,0,0 ) );
    
    
    if(!debugView){
    
        gl::color(54.f/556.f,65.f/556.f,1.f/556.f);
    
        /*gl::drawSolidCircle(vec2(mCircleOffX,720/2), 720/2 );
        gl::drawSolidCircle(vec2(mCircleOffX-(720/2),720/2), robotSize );
        gl::drawSolidCircle(vec2(mCircleOffX+(720/2),720/2), robotSize );
    */
        gl::color(1,1,1);
       
 
    
    
   
        particleHandler.draw();
        player1->draw();
        
    }
    else{
        gl::color(1,1,1);
        
        gl::drawStrokedCircle(vec2(mCircleOffX,720/2), 720/2 );
        gl::drawStrokedCircle(vec2(mCircleOffX-(720/2),720/2), robotSize );
        gl::drawStrokedCircle(vec2(mCircleOffX+(720/2),720/2), robotSize );
        
        gl::drawLine(vec2(1280/2,0),vec2(1280/2,720));
        gl::drawLine(vec2(mCircleOffX,0),vec2(mCircleOffX,720));
        gl::drawLine(vec2(0,720/2),vec2(1280,720/2));

        
        player1->drawDebug();
        for(auto p: cameraHandler.pointsTransform)
        {
            gl::drawLine(vec2(p.x-10,p.y),vec2(p.x+10,p.y));
            gl::drawLine(vec2(p.x,p.y-10),vec2(p.x,p.y+10));
             gl::drawStrokedCircle(vec2(p.x,p.y),5);
    
        }
        mParams->draw();
    }
  //console()<<getAverageFps()<<endl;
   
}

void Game_PCApp::updateRobotParams()
{

    player1->robotSize = robotSize;
    player2->robotSize = robotSize;
}
void Game_PCApp::setupParams()
{
    mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( ivec2( 200, 300 ) ) );
    mParams->addSeparator();
    mParams->addParam( "Framerate", &mFrameRate, "", true );
    mParams->addSeparator();
    mParams->addText("game position");
    mParams->addSeparator();
    mParams->addParam( "circleOffX", &mCircleOffX ).min( 1280/2 -200 ).max( 1280/2 +200).precision( 1 ).step( 0.2f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    
    
    mParams->addSeparator();
    mParams->addText("camera calibration");
    mParams->addSeparator();
    mParams->addParam( "offX", &cameraHandler.mOffX ).min( 1280/2 -200 ).max( 1280/2 +200).precision( 1 ).step( 0.2f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "offY", &cameraHandler.mOffY ).min( 720/2  -200 ).max(720/2  +200).precision( 1 ).step( 0.2f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "scaleX", &cameraHandler.mScaleX).min( 1 ).max( 3).precision( 2 ).step( 0.01f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "scaleY", &cameraHandler.mScaleY ).min( 1 ).max( 3).precision( 2 ).step( 0.01f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "mRot", &cameraHandler.mRot ).min( -0.5 ).max( 0.5).precision( 2).step( 0.01f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    
    
    mParams->addSeparator();
    mParams->addText("robot");
    mParams->addParam( "robotsize", & robotSize  ).min( 20 ).max( 70).precision( 1 ).step( 0.2f ).updateFn( [this] {updateRobotParams();  });



}

//CINDER_APP( Game_PCApp,RendererGl(  ) )
CINDER_APP( Game_PCApp,RendererGl( RendererGl::Options().msaa( 16 ) ) )
