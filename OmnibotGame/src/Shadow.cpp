#include "Shadow.h"
#include "GSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;


Shadow::Shadow()
{

}

ShadowRef Shadow::create()
{
	return std::make_shared<Shadow>();
}

void Shadow::setup()
{
	gl::Texture2d::Format depthFormat;


	depthFormat.setInternalFormat(GL_DEPTH_COMPONENT32F);
	depthFormat.setCompareMode(GL_COMPARE_REF_TO_TEXTURE);
	depthFormat.setMagFilter(GL_LINEAR);
	depthFormat.setMinFilter(GL_LINEAR);
	depthFormat.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	depthFormat.setCompareFunc(GL_LEQUAL);

	mShadowMapTex1 = gl::Texture2d::create(FBO_WIDTH, FBO_HEIGHT, depthFormat);


	gl::Fbo::Format fboFormat;
	// fboFormat.disableColor();
	fboFormat.attachment(GL_DEPTH_ATTACHMENT, mShadowMapTex1);
	mFbo1 = gl::Fbo::create(FBO_WIDTH, FBO_HEIGHT, fboFormat);

	gl::Texture2d::Format depthFormat2;


	depthFormat2.setInternalFormat(GL_DEPTH_COMPONENT32F);
	depthFormat2.setCompareMode(GL_COMPARE_REF_TO_TEXTURE);
	depthFormat2.setMagFilter(GL_LINEAR);
	depthFormat2.setMinFilter(GL_LINEAR);
	depthFormat2.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	depthFormat2.setCompareFunc(GL_LEQUAL);

	mShadowMapTex2 = gl::Texture2d::create(FBO_WIDTH, FBO_HEIGHT, depthFormat2);


	gl::Fbo::Format fboFormat2;
	fboFormat2.attachment(GL_DEPTH_ATTACHMENT, mShadowMapTex2);
	//fboFormat2.disableColor();

	mFbo2 = gl::Fbo::create(FBO_WIDTH, FBO_HEIGHT, fboFormat2);


	mLightPos.x = GSettings::windowHeight / 2 ;
	mLightPos.y = GSettings::windowHeight/ 2;
	mLightPos.z = -2000;


}
void Shadow::startDraw(int id)
{

	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::enable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 2.0f);

	if (id == 0){
		mFbo1->bindFramebuffer();
		gl::viewport(vec2(0.0f), mFbo1->getSize());
	}
	else
	{
		mFbo2->bindFramebuffer();
		gl::viewport(vec2(0.0f), mFbo2->getSize());
	}
	gl::color(Color::white());
	gl::clear(Color::white());
	gl::pushMatrices();

	// Set up camera from the light's viewpoint
	mLightCam.setPerspective(50.0f, mFbo1->getAspectRatio(), 1.f, 5000.0f);
	mLightCam.setWorldUp(vec3(0, 0, -1));
	mLightCam.lookAt(mLightPos, vec3(GSettings::windowWidth / 2, GSettings::windowHeight / 2, 0), vec3(0, 0, 1));


	gl::setMatrices(mLightCam);
	shadowMatrix = mLightCam.getProjectionMatrix() * mLightCam.getViewMatrix();

}

void Shadow::stopDraw(int id)
{
	gl::popMatrices();
	if (id == 0){
		mFbo1->unbindFramebuffer();
	}
	else
	{
		mFbo2->unbindFramebuffer();
	}
	gl::disable(GL_POLYGON_OFFSET_FILL);
	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::viewport(vec2(0.0f), vec2(GSettings::windowWidth, GSettings::windowHeight));
}