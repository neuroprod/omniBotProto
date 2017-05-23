//
//  CameraHandler.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 14/05/2017.
//
//

#ifndef CameraHandler_hpp
#define CameraHandler_hpp
#include "cinder/app/App.h"
#include "cinder/Serial.h"

class CameraHandler
{

public:
    CameraHandler();
    ~CameraHandler();
    void setup(std::string serialPort);
    void update();
    void updateMappingMatrix();

    ci::SerialRef	mSerial;
    
    std::vector<ci::vec4> pointsInput;
     std::vector<ci::vec4> pointsTransform;
    ci::vec4 center;
    
    
    unsigned char buffer[200];
    int bufferPos=0;
   

    glm::mat4 mappingMatrix;
   
    float mOffX =654.2 ;
    float mOffY=322.2 ;
    float mScaleX=2.0;
    float mScaleY=2.0;
    float mRot=0.0;
    
    
    glm::vec4 currentPosition;
    glm::vec4 currentDirection;
    bool newPos =false;
    
};
#endif /* CameraHandler_hpp */
