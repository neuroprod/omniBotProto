#include <stdio.h>
#include <unistd.h>
#include "camera.h"
#include "graphics.h"

#define MAIN_TEXTURE_WIDTH 640
#define MAIN_TEXTURE_HEIGHT 480

char tmpbuff[MAIN_TEXTURE_WIDTH*MAIN_TEXTURE_HEIGHT*4];

//entry point
int main(int argc, const char **argv)
{
	//should the camera convert frame data from yuv to argb automatically?
	bool do_argb_conversion = true;

	//how many detail levels (1 = just the capture res, > 1 goes down by half each level, 4 max)
	int num_levels = 1;

	//init graphics and the camera
	InitGraphics();
	CCamera* cam = StartCamera(MAIN_TEXTURE_WIDTH, MAIN_TEXTURE_HEIGHT,60,num_levels,do_argb_conversion);

	//create 4 textures of decreasing size
	GfxTexture textures[4];
	for(int texidx = 0; texidx < num_levels; texidx++)
		textures[texidx].Create(MAIN_TEXTURE_WIDTH >> texidx, MAIN_TEXTURE_HEIGHT >> texidx);

	printf("Running frame loop\n");
	for(int i = 0; i < 600; i++)
	{
		//pick a level to read based on current frame (flicking through them every 30 frames)
		int texidx = 0;

		//lock the chosen frame buffer, and copy it directly into the corresponding open gl texture
		const void* frame_data; int frame_sz;
		if(cam->BeginReadFrame(texidx,frame_data,frame_sz))
		{
			if(do_argb_conversion)
			{
				//if doing argb conversion the frame data will be exactly the right size so just set directly
				textures[texidx].SetPixels(frame_data);
			}
			else
			{
				//if not converting argb the data will be the wrong size and look weird, put copy it in
				//via a temporary buffer just so we can observe something happening!
				memcpy(tmpbuff,frame_data,frame_sz);
				textures[texidx].SetPixels(tmpbuff);
			}
			cam->EndReadFrame(texidx);
		}

		//begin frame, draw the texture then end frame (the bit of maths just fits the image to the screen while maintaining aspect ratio)
		BeginFrame();
		float aspect_ratio = float(MAIN_TEXTURE_WIDTH)/float(MAIN_TEXTURE_HEIGHT);
		float screen_aspect_ratio = 1280.f/720.f;
		DrawTextureRect(&textures[texidx],-aspect_ratio/screen_aspect_ratio,-1.f,aspect_ratio/screen_aspect_ratio,1.f);
		EndFrame();
	}

	StopCamera();
}
