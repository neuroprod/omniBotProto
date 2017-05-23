//
//  CameraHandler.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 14/05/2017.
//
//

#include "CameraHandler.hpp"


using namespace std;
using namespace ci;
using namespace ci::app;

CameraHandler::CameraHandler()
{

}

CameraHandler::~CameraHandler()
{

}

void CameraHandler::setup(std::string serialPort)
{
    try {
        Serial::Device dev = Serial::findDeviceByNameContains( serialPort);
        mSerial = Serial::create( dev, 115200 );
        char b = '*';
        while( mSerial->getNumBytesAvailable() > -1 ) {
            b = mSerial->readByte();
            console() << b << "_";
        }
        
        mSerial->flush();
    }
    catch( SerialExc &exc ) {
        // CI_LOG_EXCEPTION( "coult not initialize the serial device", exc );
        // exit( -1 );
        console()<<"failed to connect to "<<serialPort <<endl;
    }
    
    pointsInput.push_back(vec4(325.5,214.5,0,1));
    pointsInput.push_back(vec4(343,224.5,0,1));
    pointsInput.push_back(vec4(342,244.8,0,1));
    
    
    updateMappingMatrix();

}
void CameraHandler::update()
{

    if(mSerial && mSerial->getNumBytesAvailable() >0)
    {
        while(mSerial->getNumBytesAvailable() >0)
        {
            unsigned char a =mSerial->readChar();
            if(a==255)
            {
                
                
                if(bufferPos>11 && bufferPos%4==0)
                {
                    
                    int  numPoints =bufferPos/4;
                    pointsInput.clear();
                    
                    for(int i=0;i< numPoints;i++)
                    {
                        int index = i*4;
                        
                        int x1 =buffer[index];
                        int x2 =buffer[index+1];
                        float x = (x1*100 +x2)/10.f;
                        
                        int y1 =buffer[index+2];
                        int y2 =buffer[index+3];
                        
                        float y =480- (y1*100 +y2)/10.f;
                        
                        
                        
                        pointsInput.push_back(vec4(x,y,0,1));
                        
                    }
                    
                }
                bufferPos=0;
            }else{
                buffer[bufferPos] =a;
                bufferPos++;
                
            }
            
            
            
            newPos =true;
            
            
        }
    }
        pointsTransform.clear();
        for(auto p: pointsInput)
        {
            
            glm::vec4 result = mappingMatrix* p;
            pointsTransform.push_back(result);
        }
        
        for(int i=0;i<pointsTransform.size()-1;i++)
        {
            
            for(int j=i+1;j<pointsTransform.size();j++)
            {
                float dist = glm::distance(pointsTransform[i], pointsTransform[j]);
                if(dist > 60 && dist <80)
                {
                    center =(pointsTransform[i]+ pointsTransform[j])/2.f;
                    for(int k=0;k<pointsTransform.size();k++)
                    {
                        if(k!=i && k!=j)
                        {
                            
                            float dist2 = glm::distance(pointsTransform[k], center);
                            if(dist2>15 && dist2 <25)
                            {
                                currentPosition =center;
                                
                                currentDirection =pointsTransform[k] -center;
                            }
                        }
                        
                        
                    }
                }
                
            }
        }

}




void CameraHandler::updateMappingMatrix()
{
        
        mappingMatrix=glm::translate(glm::mat4(1.0f), glm::vec3(mOffX,mOffY,0));
        mappingMatrix=glm::scale(mappingMatrix, glm::vec3(mScaleX,mScaleY,1));
        mappingMatrix=glm::rotate(mappingMatrix,mRot ,glm::vec3(0,0,1));
        
        
        mappingMatrix= glm::translate(mappingMatrix, glm::vec3(-640/2,-480/2,0));
        
        
        
        
        
        
       // console()<< "updateMapping"<< endl;
        
        
}


