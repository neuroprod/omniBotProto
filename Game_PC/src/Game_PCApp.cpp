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

#include "Level.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class Game_PCApp : public App {
  public:
    
    bool debugView =false;
    bool useCameraPositioning =false;
    
    ///////////////////////////
    
	void setup() override;
    void keyDown( KeyEvent event ) override;
	void update() override;
	void draw() override;
    void setupParams();
  
    
    void drawDebug();
    
    GameRenderer renderer;
    
    CameraHandler2 cameraHandler;
    ArduinoHandler arduinoHandlerInput;
    ArduinoHandler arduinoHandlerOutput;
   
    PlayerRef player1;
    PlayerRef player2;
    
    
    Level level;
    
     ci::gl::TextureRef mask;
    
    ParticleHandler particleHandler;

    params::InterfaceGlRef	mParams;
    
    float mCircleOffX =674.2 ;
    float robotSize =52;
   
 
   
  
    
   
    bool useCameraCalibration =true;
    bool showCameraCalibration =false;
    bool useFloorCalibration =true;
    bool showFloorCalibration =false;
    Calibrator calibratorCam;
    Calibrator calibratorFloor;
    
  
    float   mFrameRate;
    double previousTime;
    double previousCameraTime;
    
    vec2 pointCenter1;
    vec2 pointCenter2;

    vec2 pointCenter1off;
    vec2 pointCenter2off;
    
    float moveDelayFactor=12;
    float moveDelayLowPass =0.5;
};

void Game_PCApp::setup()
{
    //projector size = 1280 720
    //camera size =1280 720;
    
    setWindowSize(1280, 720);
    
    
    for( const auto &dev : Serial::getDevices() ){
        console() << "Device: " << dev.getName() << endl;
    }
    if(useCameraPositioning){
        cameraHandler.setup(true);
    }
    
    arduinoHandlerOutput.setup("tty.usbmodem14111");
    arduinoHandlerInput.setup("tty.usbmodem14141");
    //arduinoHandlerInput.setup("tty.usbmodem14211");
    
   
    
    player1 =Player::create();
    player1->setup();
    player1->name ="1:";
    player1->id =0;
    player1->circleCenter =vec2(mCircleOffX, 720/2);
    player1->setUseCamera(useCameraPositioning);
    
    player2 =Player::create();
    player2->setup();
    player2->name ="2:";
    player2->id =1;
    player2->circleCenter =vec2(mCircleOffX, 720/2);
    player2->setUseCamera(useCameraPositioning);
    
    arduinoHandlerInput.player1 = player1;
    arduinoHandlerInput.player2 = player2;
   
    
    level.setup(player1,player2);
    player1->levelSize =level.levelSize;
    player2->levelSize =level.levelSize;
    
    
   // particleHandler.setup(720/2, vec2(mCircleOffX,720/2));
    
   
    
    calibratorCam.setup();
    calibratorFloor.setup("floor");
    
    renderer.setup();
    
    setupParams();
    
    mask =gl::Texture::create( loadImage(getAssetPath("mask.png")),gl::Texture::Format().loadTopDown());
    previousTime  =getElapsedSeconds();
    previousCameraTime = previousTime ;

    
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
      //  particleHandler.reset();
        
    }

};


void Game_PCApp::update()
{
    // why asinc?
    if(!calibratorCam.loaded)calibratorCam.loadCalibration();
    if(!calibratorFloor.loaded)calibratorFloor.loadCalibration();

    
    
    double currentTime  =getElapsedSeconds();
    double elapsed = (currentTime-previousTime)*1000;
    previousTime =currentTime;
    
    
    
    

    if (useCameraPositioning)cameraHandler.update();
    arduinoHandlerInput.update();
    arduinoHandlerOutput.update();
    
    
    
    if(useCameraPositioning )
    {
        if(cameraHandler.newPos){
            
            //set new player pos
            cameraHandler.newPos =false;
            double elapsedCamera = (currentTime-previousCameraTime)*1000;
            previousCameraTime =currentTime;
            
            
            vec4 pos1 = cameraHandler.position1.currentPosition;
            ivec2 input1 =ivec2 (pos1.x,pos1.y);
            vec2 offset1 =  calibratorCam.getOffsetForPoint(input1);
            
            
            cameraHandler.position1.setSpeedLP(player1->controler, moveDelayFactor, moveDelayLowPass);
            
            
            pos1.x+=offset1.x+cameraHandler.position1.speedLP.x;
            pos1.y+=offset1.y+cameraHandler.position1.speedLP.y;
           
            
            player1->setRobotPosition(pos1,cameraHandler.position1.currentDirection,elapsedCamera);
            
            
            
            
            
            
            vec4 pos2 = cameraHandler.position2.currentPosition;
            ivec2 input2 =ivec2 (pos2.x,pos2.y);
            vec2 offset2 =  calibratorCam.getOffsetForPoint(input2);
            
            cameraHandler.position2.setSpeedLP(player2->controler, moveDelayFactor, moveDelayLowPass);

            
            pos2.x+=offset2.x+cameraHandler.position2.speedLP.x;
            pos2.y+=offset2.y+cameraHandler.position2.speedLP.y;
            
            player2->setRobotPosition(pos2,cameraHandler.position2.currentDirection,elapsedCamera);
            
            
        }
    }
   
    
    //set LevelPos
    if(debugView)
    {
    
        player1->updateDebug(elapsed);
        player2->updateDebug(elapsed);
    }else
    {
        //update player pos
        player1->update(elapsed);
        player2->update(elapsed);
    
    }
    
    //calculate mask
    
    vec2 distVec =player1->playerViewPos-  player2->playerViewPos;
    float distance = glm::length(distVec );
    
    vec2 perpVec =vec2(-distVec.y ,distVec.x);
    
    vec2 centerPos = (player1->playerViewPos+  player2->playerViewPos)/2.f;
    vec2 perpVecNorm = glm::normalize(perpVec);
    
    pointCenter1=centerPos-perpVecNorm*720.f;
    pointCenter2 =centerPos+perpVecNorm*720.f;
    
    vec2 distVecNorm =glm::normalize(distVec);
    
    pointCenter1off = pointCenter1+distVecNorm*720.f;
    pointCenter2off =pointCenter2+distVecNorm*720.f;
    
   //calculate robot distance
    
   /* if(distance<200)
    {
    
        float offsetDistance = 200-distance;
        
        vec2 distVecN = glm::normalize(distVec);
        
        player1->playerViewPos+=distVecN*offsetDistance/2.f;
        player2->playerViewPos-=distVecN*offsetDistance/2.f;
    
    }*/
    
    //set finall robot pos
    if(!debugView)
    {
        player1->updateWorldOffset();
        player2->updateWorldOffset();
    
    }

    //
    level.updatePlayerPositions( pointCenter1,pointCenter2);
    
    //update all physics
    level.update();
    
    
    //
    
    
    
    if(player1->hasNewCommand)
    {
        player1->hasNewCommand =false;
        arduinoHandlerOutput.sendCommand(player1->command);
    }

    
    if(player2->hasNewCommand)
    {
        player2->hasNewCommand =false;
        arduinoHandlerOutput.sendCommand(player2->command);
    }
    

    //render shadows
    
    renderer.startShadowDraw(0);
    level.drawShadow(0);
    renderer.stopShadowDraw(0);
    
  
    
    renderer.startShadowDraw(1);
    level.drawShadow(1);
    renderer.stopShadowDraw(1);
    
    
    //
    
    mFrameRate = getAverageFps();
    
    
    
    
    
}

void Game_PCApp::draw()
{
    gl::color(1,1,1);
    level.floorMap.generator.generate();

   	gl::clear( Color( 0,0,0 ) );

    if(debugView){
    
        drawDebug();
    }
    else
    {
     
        
        
        
        glEnable(GL_STENCIL_TEST);
        gl::clear( Color( 0,0,0 ) );
        glStencilMask(0xFF);
        gl::clear( GL_STENCIL_BUFFER_BIT );
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        // Draw floor
        
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
        glStencilMask(0xFF); // Write to stencil buffer
        
        gl::color(0.5,0.5,0.5,1);
        
        gl::drawSolidTriangle(pointCenter1, pointCenter2 ,pointCenter1off);
         gl::drawSolidTriangle(pointCenter2 ,pointCenter2off,pointCenter1off);
       
        glStencilMask(0x00);
        renderer.startMainDraw();
        
       
        glStencilFunc(GL_EQUAL, 1, 0xFF);
     
       
        level.draw(0,renderer.mFbo1,renderer.shadowMatrix);
     
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        gl::clear(GL_DEPTH_BUFFER_BIT);
        level.draw(1,renderer.mFbo2,renderer.shadowMatrix);
        
        
        renderer.stopMainDraw();
        
        
        
        
         glDisable(GL_STENCIL_TEST);
      
      
        
        
        gl::color(0,0,0);
       
       
        
        gl::drawLine(pointCenter1, pointCenter2);
     
        
        player1->draw();
        
        player2->draw();
        
      //  gl::draw(mask,vec2(((1280/2)-mCircleOffX)/2,0));
        
        gl::color(0.8,0.8,0.8);
      // gl::draw(renderer.mFbo1->getColorTexture(),Rectf(0,0,300,300));
       // gl::draw(renderer.mFbo2->getColorTexture(),Rectf(0,0,600,600));

    }

    gl::color(1,1,1);
  // gl::draw(level.floorMap.generator.mFbo->getColorTexture(),Rectf(0,0,800,800));
    gl::color(1,1,1);
   // mParams->draw();
    drawDebug();
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
            
            gl::drawStrokedCircle(p, robotSize);
            
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




void Game_PCApp::setupParams()
{
    mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( ivec2( 200, 700 ) ) );
    
    mParams->addSeparator();
    mParams->addParam( "Framerate", &mFrameRate, "", true );
    mParams->addSeparator();
    
    mParams->addParam( "offY_viewer", &renderer.offyCam ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
    mParams->addParam( "offZ_viewer", &renderer.offzCam ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
     mParams->addSeparator();
    
    
    mParams->addParam( "lightX", &renderer.mLightPos.x ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
    mParams->addParam( "lightY", &renderer.mLightPos.y ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
     mParams->addParam( "lightZ", &renderer.mLightPos.z ).step( 10.f ).updateFn( [this] {renderer.updateCameraPosition(); });
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
    mParams->addParam( "moveDelayFactor", & moveDelayFactor ).min( 0 ).max( 20).precision( 2 ).step( 0.01f );
    mParams->addParam( "moveDelayLPowPass", & moveDelayLowPass ).min( 0 ).max( 1).precision( 2 ).step( 0.01f );
    mParams->addSeparator();
    mParams->addText("robot");
    mParams->addParam( "robotsize", & robotSize  ).min( 20 ).max( 70).precision( 1 ).step( 0.2f ).updateFn( [this] { player1->robotSize = robotSize;
      
        
        
        player2->robotSize = robotSize; });

   // mParams->addParam( "moveOffset", &player1->moveOffset  ).min( 0 ).max( 70).precision( 1 ).step( 0.1f );
    //mParams->addParam( "moveOffsetStart", &player1->moveOffsetStart  ).min( 0 ).max( 70).precision( 1 ).step( 0.1f );
}

//CINDER_APP( Game_PCApp,RendererGl(  ) )
CINDER_APP( Game_PCApp,RendererGl( RendererGl::Options().msaa( 16 ).stencil() ) )
