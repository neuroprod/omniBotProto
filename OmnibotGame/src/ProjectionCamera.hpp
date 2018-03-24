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
#include "cinder/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

class ProjectionCamera: public cinder::Camera {



public:
    
    ProjectionCamera()
    {
        mProjectionCached = false;
        mModelViewCached = true;
        mInverseModelViewCached = true; 
    }
	void updateSetting();
    virtual bool	isPersp() const { return true; }
    glm::vec3 cameraPosition;
protected:
    virtual void	calcProjection() const;
   


};
#endif /* ProjectionCamera_hpp */

