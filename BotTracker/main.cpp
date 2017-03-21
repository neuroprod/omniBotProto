//sudo modprobe bcm2835-v4l2

//https://github.com/stanleyseow/RF24

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "graphics.h"
#include "camera.h"
#include <RF24/RF24.h>


using namespace std;
RF24 radio(RPI_V2_GPIO_P1_23, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
const uint8_t pipes[][6] = {"1Node","2Node"};
int main() {
float testScale =2;
   // radio.begin();

    // optionally, increase the delay between retries & # of retries
   // radio.setRetries(15,15);
    // Dump the configuration of the rf unit for debugging
    //radio.printDetails();
    //radio.openWritingPipe(pipes[0]);
    //radio.openReadingPipe(1,pipes[1]);

    InitGraphics();

    CCamera* cam = StartCamera(640*testScale, 480*testScale,60,1,false);
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    double millisprev = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    /*radio.stopListening();

    // Take the time, and send it.  This will block until complete

    printf("Now sending...\n");
    unsigned long time = millis();

    bool ok = radio.write( &time, sizeof(unsigned long) );

    if (!ok){
        printf("failed.\n");
    }
    // Now, continue listening
    radio.startListening();*/
    while(true) {







        std::vector<cv::Point> centers;
        const void* frame_data; int frame_sz;
        if(cam->BeginReadFrame(0,frame_data,frame_sz))
        {
           // void *data = const_cast<void * >(frame_data);
            //void * out[frame_sz];

           /* for(int i=0;i<frame_sz;i++)
            {
                out[i] = (void *)(unsigned char *) (255);
            }*/



         cv::Mat greyMat (480*testScale,640*testScale,CV_8UC1,const_cast<void * >(frame_data));
            cv::Mat dst;//dst image
            cv::Size size(480,640);
            resize( greyMat,dst,size);

          threshold( dst, dst, 200, 255,0 );

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(dst,contours,cv::RETR_LIST,cv::CHAIN_APPROX_NONE);
            for(size_t index=0;index<contours.size();index++)
            {
                cv::Moments moms = cv::moments(cv::Mat(contours[index]));
                cv::Point p = cv::Point(moms.m10/moms.m00,moms.m01/moms.m00);
            //   cv::circle(greyMat,p,10,cv::Scalar(0,0,0),2);
                centers.push_back(p);

            }


            //cv::imshow("Video", greyMat);








            cam->EndReadFrame(0);

            BeginFrame();
            for(auto p:centers)
            {

                DrawTextureRect(p.x ,p.y);

            }
            EndFrame();


            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            double millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            double ellapsed = millis -millisprev;
            millisprev =millis;
           cout<<1000/ellapsed <<endl;




        }




       // int c = cvWaitKey(20);

        //exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27)
       // if(27 == char(c)) break;
    }
   // camera.release();
    return 0;
}