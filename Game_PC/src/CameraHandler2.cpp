//
//  CameraHandler2.cpp
//  WebcamCinderTest
//
//  Created by Kris Temmerman on 09/06/2017.
//
//

#include "CameraHandler2.hpp"
#include "CinderOpenCV.h"

using namespace ci;
using namespace ci::app;
using namespace std;

CameraHandler2::CameraHandler2()
{

}
CameraHandler2::~CameraHandler2()
{

    stopThread = true;
    if (cameraThread.joinable())cameraThread.join();
}


void CameraHandler2::setup(bool _useThread)
{
    updateMappingMatrix();
    useThread =_useThread;
    
    for( const auto &device : Capture::getDevices() ) {
        console() << "Device: " << device->getName() << endl;
    }
    
    mCapture = Capture::create( 1280, 720 );
    mCapture->start();
    
    if(useThread)
    {
        stopThread = false;
        cameraThread = std::thread(std::bind(&CameraHandler2::worker, this));
    }
}
void CameraHandler2::update()
{
    

    pointsTransform.clear();
    
    pointMutex.lock();
    for(auto p: centers)
    {
        
        glm::vec4 result = mappingMatrix* vec4(p.x,p.y,0,1);
        pointsTransform.push_back(result);
    }
    pointMutex.unlock();
    
    vec4 currentPosition;
    vec4 currentDirection;
    vec4 center;
    
    vector <CameraPosition> tempPositions;
    
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
                            CameraPosition pos;
                            pos.currentPosition =currentPosition;
                            pos.currentDirection =currentDirection;
                            
                            tempPositions.push_back(pos);
                        }
                    }
                    
                    
                }
            }
            
        }
    }
    
    position1.bestDistance=10000;
    position2.bestDistance=10000;
    
    for(auto pos: tempPositions)
    {
        
        float dist1 = glm::distance(pos.currentPosition,position1.currentPosition);
        float dist2 = glm::distance(pos.currentPosition,position2.currentPosition);
        if(dist1<dist2)
        {
            if(dist1<position1.bestDistance)
            {
                position1.bestDistance=dist1;
                position1.currentPosition = pos.currentPosition;
                position1.currentDirection = pos.currentDirection;
            }
            
        }else
        {
            if(dist2<position2.bestDistance)
            {
                position2.bestDistance=dist2;
                position2.currentPosition = pos.currentPosition;
                position2.currentDirection = pos.currentDirection;
            }
            
            
            
            
        }
        
        
    }

    
    
    
    
    
    
    ///////////////////////////////////////////////
    if(useThread) return;
    if( mCapture && mCapture->checkNewFrame() ) {
        
        Surface8u s =*mCapture->getSurface();
          double current =getElapsedSeconds()*1000;
        
        
      
        
        
        
        
        Surface::ConstIter maskIter( s.getIter() ); // using const because we're not modifying it
        std::array< unsigned char,1280* 720> d;
        d.fill(0);
        int i=0;
        while( maskIter.line()  )
        { // line by line
            while( maskIter.pixel()  )
            {
                if(maskIter.r()>100)
                {
                    d[i] =255;
                }
               
                i++;
            }
        }
        centers.clear();
        cv::Mat greyMat( 720,1280, CV_8UC1, d.data());
        std::vector <std::vector<cv::Point>> contours;
        cv::findContours(greyMat, contours, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        for (size_t index = 0; index < contours.size(); index++) {
            cv::Moments moms = cv::moments(cv::Mat(contours[index]));
            glm::vec2 p = glm::vec2(moms.m10 / moms.m00, moms.m01 / moms.m00);
            centers.push_back(p);
            
        }
          double prev =getElapsedSeconds()*1000 -current;
        ci::app::console()<<prev<<endl;;
        mTexture = gl::Texture::create( fromOcv( greyMat ), gl::Texture::Format().loadTopDown() );
        
    }
}


void CameraHandler2::updateMappingMatrix()
{
    
    mappingMatrix=glm::translate(glm::mat4(1.0f), glm::vec3(mOffX,mOffY,0));
    mappingMatrix=glm::scale(mappingMatrix, glm::vec3(mScaleX,mScaleY,1));
    mappingMatrix=glm::rotate(mappingMatrix,mRot ,glm::vec3(0,0,1));
    mappingMatrix= glm::translate(mappingMatrix, glm::vec3(-1280/2, -720/2 ,0));
}


void CameraHandler2::worker()
{
    unsigned char * d = new unsigned char[921600];
    int count=0;
    while (!stopThread)
    {
        if( mCapture && mCapture->checkNewFrame() )
        {
           
            Surface s =*mCapture->getSurface();
            Surface::ConstIter maskIter( s.getIter() );
            
            int i=0;
            while( maskIter.line()  )
            { // line by line
                while( maskIter.pixel()  )
                {
                    if(maskIter.r()>100)
                    {
                        d[i] =255;
                    }else
                    {
                        d[i] =0;
                    }
                    i++;
                }
            }
           
            cv::Mat greyMat( 720,1280, CV_8UC1, d);
            std::vector <std::vector<cv::Point>> contours;
            cv::findContours(greyMat, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

            pointMutex.lock();
            
            centers.clear();
            for (size_t index = 0; index < contours.size(); index++) {
                cv::Moments moms = cv::moments(cv::Mat(contours[index]));
                glm::vec2 p = glm::vec2(moms.m10 / moms.m00, moms.m01 / moms.m00);
                centers.push_back(p);
                
            }
            newPos =true;
            pointMutex.unlock();
   
            
            
            
           
            count++;
            if(count%10==0){
             double current =getElapsedSeconds()*1000;
                console()<<" "<<(int)(10000/(current-pervTime))<<" fps"<<endl;
            
            pervTime =current;
            }
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
void CameraHandler2::draw()
{
    if(useThread) return;
    if( mTexture ) {
        gl::draw( mTexture );
        
    }

}
