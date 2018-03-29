//
//  ProjectionCamera.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 25/05/2017.
//
//

#include "ProjectionCamera.hpp"
#include "GSettings.h"
ProjectionCameraRef ProjectionCamera::create()
{

	return std::make_shared<ProjectionCamera>();
}

void ProjectionCamera::updateSetting()
{


	

	cameraPosition.x = 0;
	cameraPosition.y = -GSettings::windowHeight - offyCam;
	cameraPosition.z = offzCam;

	setEyePoint(ci::vec3(GSettings::windowHeight / 2, GSettings::windowHeight + offyCam / 2, offzCam / 2));

	lookAt(ci::vec3(GSettings::windowHeight / 2, GSettings::windowHeight + offyCam / 2, 0));

	mNearClip = 100.f;
	mFarClip = 3000.f;
	calcProjection();

}


void	ProjectionCamera::calcProjection() const
{
    
    
	float w = GSettings::windowHeight;
	float h = GSettings::windowHeight;
  
    mFrustumLeft = mNearClip* (-w- cameraPosition.x) / cameraPosition.z;
    mFrustumRight  = mNearClip * (w - cameraPosition.x) / cameraPosition.z;
    mFrustumBottom = mNearClip * (-h - cameraPosition.y) / cameraPosition.z;
    mFrustumTop = mNearClip * (h - cameraPosition.y) / cameraPosition.z;
    
    //glm::mat4 frustrumMat = glm::frustum (left,right, bottom, top, nearPlane, farPlane);
    
    
    glm::mat4 &p = mProjectionMatrix;
    p[0][0] =  2.0f * mNearClip / ( mFrustumRight - mFrustumLeft );
    p[1][0] =  0.0f;
    p[2][0] =  ( mFrustumRight + mFrustumLeft ) / ( mFrustumRight - mFrustumLeft );
    p[3][0] =  0.0f;
    
    p[0][1] =  0.0f;
    p[1][1] =  2.0f * mNearClip / ( mFrustumTop - mFrustumBottom );
    p[2][1] =  ( mFrustumTop + mFrustumBottom ) / ( mFrustumTop - mFrustumBottom );
    p[3][1] =  0.0f;
    
    p[0][2] =  0.0f;
    p[1][2] =  0.0f;
    p[2][2] = -( mFarClip + mNearClip ) / ( mFarClip - mNearClip );
    p[3][2] = -2.0f * mFarClip * mNearClip / ( mFarClip - mNearClip );
    
    p[0][3] =  0.0f;
    p[1][3] =  0.0f;
    p[2][3] = -1.0f;
    p[3][3] =  0.0f;
    
    glm::mat4 &m = mInverseProjectionMatrix;
    m[0][0] =  ( mFrustumRight - mFrustumLeft ) / ( 2.0f * mNearClip );
    m[1][0] =  0.0f;
    m[2][0] =  0.0f;
    m[3][0] =  ( mFrustumRight + mFrustumLeft ) / ( 2.0f * mNearClip );
    
    m[0][1] =  0.0f;
    m[1][1] =  ( mFrustumTop - mFrustumBottom ) / ( 2.0f * mNearClip );
    m[2][1] =  0.0f;
    m[3][1] =  ( mFrustumTop + mFrustumBottom ) / ( 2.0f * mNearClip );
    
    m[0][2] =  0.0f;
    m[1][2] =  0.0f;
    m[2][2] =  0.0f;
    m[3][2] = -1.0f;
    
    m[0][3] =  0.0f;
    m[1][3] =  0.0f;
    m[2][3] = -( mFarClip - mNearClip ) / ( 2.0f * mFarClip*mNearClip );
    m[3][3] =  ( mFarClip + mNearClip ) / ( 2.0f * mFarClip*mNearClip );
    
    mProjectionCached = true;
    
};
