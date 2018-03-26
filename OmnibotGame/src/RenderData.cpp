#include "RenderData.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageFileTinyExr.h"
#include "cinder/ImageIo.h"


using namespace std;
using namespace ci;
using namespace ci::app;

RenderData::RenderData(){}

RenderDataRef  RenderData::create()
{
	return make_shared<RenderData>();
}



void RenderData::setup()
{
	console() << "setup hdr" << endl;
	
	ci::ImageSourceRef img = loadImage(loadAsset("irrad.hdr"));
	gl::TextureCubeMap::Format format;
	format.setDataType(GL_FLOAT);
	 irradianceCubeMap = gl::TextureCubeMap::create(loadImage(loadAsset("irrad.hdr")), format.internalFormat(GL_RGB));
	
}