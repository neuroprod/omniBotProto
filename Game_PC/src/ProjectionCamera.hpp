//
//  ProjectionCamera.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 25/05/2017.
//
//

#ifndef ProjectionCamera_hpp
#define ProjectionCamera_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "glm/gtc/matrix_transform.hpp"

class ProjectionCamera: public ci::Camera {



public:
    
    ProjectionCamera()
    {
    
        cameraPosition.x =0;
        cameraPosition.y =0;
        cameraPosition.z=-1000;
        calcProjection();
        mProjectionCached = false;
        mModelViewCached = true;
        mInverseModelViewCached = true;
        mNearClip = 2000.f;
        mFarClip = 24000.f;
    }
    void setCameraPos(glm::vec3 _cameraPosition)
    {
    
        cameraPosition =_cameraPosition;
        mNearClip = 0.1f;
        mFarClip = 3000.f;
        calcProjection();
    }
    virtual bool	isPersp() const { return true; }
    glm::vec3 cameraPosition;
protected:
    virtual void	calcProjection() const;
   


};
#endif /* ProjectionCamera_hpp */

