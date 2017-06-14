#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Serial.h"
#include "cinder/params/Params.h"

#include "CameraHandler2.hpp"

#include "ArduinoHandler.hpp"
#include "Player.hpp"
#include "ParticleHandler.hpp"
#include "Calibrator.hpp"
#include "GameRenderer.hpp"



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
    
    void drawDebug();
    
    GameRenderer renderer;
    
    CameraHandler2 cameraHandler;
    ArduinoHandler arduinoHandlerInput;
    ArduinoHandler arduinoHandlerOutput;
   
    PlayerRef player1;
    PlayerRef player2;
    
    ParticleHandler particleHandler;

    params::InterfaceGlRef	mParams;
    
    float mCircleOffX =689 ;
    float robotSize =52;
   
 
    bool debugView =false;
    bool useCameraPositioning =false;
    vec2 mousePos;
   
    float   mFrameRate;
    
   
    bool useCameraCalibration =true;
    bool showCameraCalibration =false;
    bool useFloorCalibration =true;
    bool showFloorCalibration =false;
    Calibrator calibratorCam;
    Calibrator calibratorFloor;
    
    double previousTime;
    double previousCameraTime;
};

void Game_PCApp::setup()
{
    //projector size = 1280 720
    //camera size =1280 720;
    
    setWindowSize(1280, 720);
    
    
    for( const auto &dev : Serial::getDevices() )
        console() << "Device: " << dev.getName() << endl;
    
    if(useCameraPositioning){
        cameraHandler.setup(true);
    }
    arduinoHandlerOutput.setup("tty.usbmodem14111");
    arduinoHandlerInput.setup("tty.usbmodem14141");
    //arduinoHandlerInput.setup("tty.usbmodem14211");
    
   
    
    player1 =Player::create();
    player1->setup();
    player1->name ="1:";
    player1->setUseCamera(useCameraPositioning);
    
    player2 =Player::create();
    player2->setup();
    player2->name ="2:";
    player2->setUseCamera(useCameraPositioning);
    
    arduinoHandlerInput.player1 = player1;
    arduinoHandlerInput.player2 = player2;
   
    particleHandler.setup(720/2, vec2(mCircleOffX,720/2));
    
    updateRobotParams();
    
    calibratorCam.setup();
    calibratorFloor.setup("floor");
    
    previousTime  =getElapsedSeconds();
    previousCameraTime = previousTime ;
    
    renderer.setup();
    
    setupParams();
    
    
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
    double currentTime  =getElapsedSeconds();
    double elapsed = (currentTime-previousTime)*1000;
    previousTime =currentTime;
    
    if(!calibratorCam.loaded)calibratorCam.loadCalibration();
    if(!calibratorFloor.loaded)calibratorFloor.loadCalibration();
    if (useCameraPositioning)cameraHandler.update();
    
    
    arduinoHandlerInput.update();
    arduinoHandlerOutput.update();
    
    if(useCameraPositioning )
    {
        if(cameraHandler.newPos){
            
            cameraHandler.newPos =false;
            double elapsedCamera = (currentTime-previousCameraTime)*1000;
            previousCameraTime =currentTime;
            
            
            vec4 pos1 = cameraHandler.position1.currentPosition;
            ivec2 input1 =ivec2 (pos1.x,pos1.y);
            vec2 offset1 =  calibratorCam.getOffsetForPoint(input1);
            pos1.x+=offset1.x;
            pos1.y+=offset1.y;
            player1->setPosition(pos1,cameraHandler.position1.currentDirection,elapsedCamera);
            
            
            vec4 pos2 = cameraHandler.position2.currentPosition;
            ivec2 input2 =ivec2 (pos2.x,pos2.y);
            vec2 offset2 =  calibratorCam.getOffsetForPoint(input2);
            pos2.x+=offset2.x;
            pos2.y+=offset2.y;
            player2->setPosition(pos2,cameraHandler.position2.currentDirection,elapsedCamera);
            
        }
    }
    else
    {
        //console()<<mousePos<<endl;
        //player1->setPosition(vec4(mousePos.x,mousePos.y,0,1),vec4(1,0,0,1),elapsed);
        //player2->setPosition(vec4(mousePos.x+200,mousePos.y,0,1),vec4(1,0,0,1),elapsed);
    }
    
    ///////////////////////////////////
    ///////////////////////////////////
    
    
    
    player1->update(elapsed);
    
    ivec2 input =ivec2 (player1->drawPosition2D.x,player1->drawPosition2D.y);
    vec2 offset =  calibratorFloor.getOffsetForPoint(input);
    offset.y+=12;
    if(!useFloorCalibration)
    {
        offset.x = calibratorFloor.offX;
        offset.y = calibratorFloor.offY+ 12;
    }

    player1->drawPosition2DFloor =player1->drawPosition2D+offset;
    
    if(player1->hasNewCommand)
    {
        player1->hasNewCommand =false;
        arduinoHandlerOutput.sendCommand(player1->command);
    }
    
    
    
    ///////////////////////////////////
    ///////////////////////////////////
    
    
    
    
    player2->update(elapsed);
    
    ivec2 input2 =ivec2 (player2->drawPosition2D.x,player2->drawPosition2D.y);
    vec2 offset2 =  calibratorFloor.getOffsetForPoint(input2);
    offset2.y+=12;
    if(!useFloorCalibration)
    {
        offset2.x = calibratorFloor.offX;
        offset2.y = calibratorFloor.offY+ 12;
    }

    
    player2->drawPosition2DFloor =player2->drawPosition2D+offset2;
    
    if(player2->hasNewCommand)
    {
        player2->hasNewCommand =false;
        arduinoHandlerOutput.sendCommand(player2->command);
    }

    
    particleHandler.update(elapsed,player1,player2);
    mFrameRate = getAverageFps();
    
    
    
    
    
}

void Game_PCApp::draw()
{
    
	gl::clear( Color( 0,0,0 ) );

    if(debugView){
    
        drawDebug();
    }
    else
    {
      
        
        renderer.startShadowDraw();
        particleHandler.drawShadow(&renderer);
        renderer.stopShadowDraw();
        
        gl::color(1,1,1);
        
        ////////////
        
        renderer.startMainDraw();
        
        particleHandler.draw(&renderer);
        
        renderer.stopMainDraw();
        
        
        player1->draw();
        player2->draw();

    }
 
   
}





void Game_PCApp::drawDebug()
{
   
        
        gl::color(1,1,1);
        if(showCameraCalibration)
            calibratorCam.draw();
        
        if(showFloorCalibration)
            calibratorFloor.draw();
        
        gl::drawStrokedCircle(vec2(mCircleOffX,720/2), 720/2 );
        gl::drawStrokedCircle(vec2(mCircleOffX-(720/2),720/2), robotSize );
        gl::drawStrokedCircle(vec2(mCircleOffX+(720/2),720/2), robotSize );
        
        gl::drawLine(vec2(1280/2,0),vec2(1280/2,720));
        gl::drawLine(vec2(mCircleOffX,0),vec2(mCircleOffX,720));
        gl::drawLine(vec2(0,720/2),vec2(1280,720/2));
        
        
        if(useCameraCalibration)
        {
            player2->drawDebug(renderer.cameraProj);
            player1->drawDebug(renderer.cameraProj);
        }
        
        
        ;
        
        
        if(!useCameraCalibration)
        {
            
            vec2 offset;
            offset.x = calibratorCam.offX;
            offset.y = calibratorCam.offY;
            vec2 p ;
            p.x =cameraHandler.position2.currentPosition.x;
            p.y =cameraHandler.position2.currentPosition.y;
            gl::color(0,1,0);
            gl::drawLine(vec2(p.x-10,p.y),vec2(p.x+10,p.y));
            gl::drawLine(vec2(p.x,p.y-10),vec2(p.x,p.y+10));
            
            
            gl::color(1,1,1);
            p.x+=offset.x*2;
            p.y+=offset.y*2;
            gl::drawLine(vec2(p.x-10,p.y),vec2(p.x+10,p.y));
            gl::drawLine(vec2(p.x,p.y-10),vec2(p.x,p.y+10));
            
        }
        
        if(useCameraCalibration)
        {
            gl::color(0,1,0);
            for(auto p: cameraHandler.pointsTransform)
            {
                gl::drawLine(vec2(p.x-10,p.y),vec2(p.x+10,p.y));
                gl::drawLine(vec2(p.x,p.y-10),vec2(p.x,p.y+10));
                // gl::drawStrokedCircle(vec2(p.x,p.y),5);
                
            }
            gl::color(1,1,1);
            
            for(auto d: cameraHandler.pointsTransform)
            {
                
                ivec2 input =ivec2 (d.x,d.y);
                
                vec2 offset =  calibratorCam.getOffsetForPoint(input);
                if(!useCameraCalibration)
                {
                    offset.x = calibratorCam.offX;
                    offset.y = calibratorCam.offY;
                }
                vec2 p = vec2(d.x,d.y)+offset;
                
                gl::drawLine(vec2(p.x-10,p.y),vec2(p.x+10,p.y));
                gl::drawLine(vec2(p.x,p.y-10),vec2(p.x,p.y+10));
                // gl::drawStrokedCircle(vec2(p.x,p.y),5);
                
            }
        }
        
        mParams->draw();
   
    
}








void Game_PCApp::updateRobotParams()
{

    player1->robotSize = robotSize;
    player2->robotSize = robotSize;
}
void Game_PCApp::setupParams()
{
    mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( ivec2( 200, 700 ) ) );
    
    mParams->addSeparator();
    mParams->addParam( "Framerate", &mFrameRate, "", true );
    mParams->addSeparator();
    
    mParams->addParam( "offY_viewer", &renderer.offyCam ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
    mParams->addParam( "offZ_viewer", &renderer.offzCam ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
     mParams->addSeparator();
    mParams->addText("game position");
    mParams->addSeparator();
    mParams->addParam( "circleOffX", &mCircleOffX ).min( 1280/2 -200 ).max( 1280/2 +200).precision( 1 ).step( 0.2f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    
    
    mParams->addSeparator();
    mParams->addText("camera calibration main");

    mParams->addSeparator();
    mParams->addParam( "offX", &cameraHandler.mOffX ).min( 1280/2 -200 ).max( 1280/2 +200).precision( 1 ).step( 0.2f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "offY", &cameraHandler.mOffY ).min( 720/2  -200 ).max(720/2  +200).precision( 1 ).step( 0.2f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "scaleX", &cameraHandler.mScaleX).min( 0.5 ).max( 3).precision( 2 ).step( 0.01f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "scaleY", &cameraHandler.mScaleY ).min( 0.5 ).max( 3).precision( 2 ).step( 0.01f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    mParams->addParam( "mRot", &cameraHandler.mRot ).min( -0.5 ).max( 0.5).precision( 2).step( 0.01f ).updateFn( [this] {cameraHandler.updateMappingMatrix(); } );
    
    
    
    
    
    
    
    
    
    
    mParams->addSeparator();
    mParams->addText("camera calibration detail");
    mParams->addParam("showCamCalibration", &showCameraCalibration);
    mParams->addParam("useCamCalibration", &useCameraCalibration);
    mParams->addParam( "offX_cal", &calibratorCam.offX ).min( -12.5 ).max( 12.5).precision( 1 ).step( 0.1f );
    mParams->addParam( "offY_cal", &calibratorCam.offY ).min( -12.5 ).max( 12.5).precision( 1 ).step( 0.1f );
    mParams->addButton("savePoint", [this] {
        calibratorCam.addVector(vec2(cameraHandler.position2.currentPosition.x,cameraHandler.position2.currentPosition.y));
    });
    mParams->addButton("saveData", [this] {calibratorCam.saveCalibration(); });
    mParams->addButton("resetData", [this] {calibratorCam.reset(); });
    
    mParams->addSeparator();
    mParams->addText("camera calibration floor");
     mParams->addParam("showFloorCalibration", &showFloorCalibration);
    mParams->addParam("useFloorCalibration", &useFloorCalibration);
    mParams->addParam( "offX_cal_floor", &calibratorFloor.offX ).min( -12.5 ).max( 12.5).precision( 1 ).step( 0.2f );
    mParams->addParam( "offY_cal_floor", &calibratorFloor.offY ).min( -12.5 ).max( 12.5).precision( 1 ).step( 0.2f );
    mParams->addButton("savePoint_f", [this] {
        
        
        vec4 pos = cameraHandler.position1.currentPosition;
        ivec2 input =ivec2 (pos.x,pos.y);
        vec2 offset =  calibratorCam.getOffsetForPoint(input);
        pos.x+=offset.x;
        pos.y+=offset.y;
        calibratorFloor.addVector(vec2(pos.x,pos.y));
    
    });
    mParams->addButton("saveData_f", [this] {calibratorFloor.saveCalibration(); });
    mParams->addButton("resetData_f", [this] {calibratorFloor.reset(); });
    
    
    
    
    
    
    
    mParams->addSeparator();
    mParams->addText("robot");
    mParams->addParam( "robotsize", & robotSize  ).min( 20 ).max( 70).precision( 1 ).step( 0.2f ).updateFn( [this] {updateRobotParams();  });

    mParams->addParam( "moveOffset", &player1->moveOffset  ).min( 0 ).max( 70).precision( 1 ).step( 0.1f );
    mParams->addParam( "moveOffsetStart", &player1->moveOffsetStart  ).min( 0 ).max( 70).precision( 1 ).step( 0.1f );
}

//CINDER_APP( Game_PCApp,RendererGl(  ) )
CINDER_APP( Game_PCApp,RendererGl( RendererGl::Options().msaa( 16 ) ) )
