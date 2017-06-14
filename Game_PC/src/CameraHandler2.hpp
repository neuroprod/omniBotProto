//
//  CameraHandler2.hpp
//  WebcamCinderTest
//
//  Created by Kris Temmerman on 09/06/2017.
//
//

#ifndef CameraHandler2_hpp
#define CameraHandler2_hpp


#include "cinder/app/App.h"
#include <thread>
#include <mutex>
#include "cinder/Capture.h"


class CameraPosition
{
    
public:
    CameraPosition(){};
    ~CameraPosition(){};
    glm::vec4 currentPosition;
    glm::vec4 currentDirection;
    float bestDistance;
    int id;
    
};




class CameraHandler2
{
public:
    
    CameraHandler2();
    ~CameraHandler2();
    void setup(bool useThread);
    void update();
    void worker();
    void draw();
    void updateMappingMatrix();
    ci::CaptureRef	mCapture;
    
    std::thread cameraThread;
    std::mutex pointMutex;
    std::vector<glm::vec2> centers;
    bool stopThread;
    bool useThread =true;
    double pervTime =0;
    
    cinder::gl::TextureRef		mTexture;

    
    
    glm::mat4 mappingMatrix;
    
    float mOffX =517.8 ;
    float mOffY=327.8;
    float mScaleX=0.87;
    float mScaleY=0.87;
    float mRot=0.0;
    
    
    // glm::vec4 currentPosition;
    //  glm::vec4 currentDirection;
    
    CameraPosition position1;
    CameraPosition position2;
    
     std::vector<ci::vec4> pointsTransform;

    bool newPos =false;
};

#endif /* CameraHandler2_hpp */
