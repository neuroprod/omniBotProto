//
// Created by Kris Temmerman on 21/03/17.
//

#include "CameraHandler.h"
#include "GLTexture.h"
using namespace std;
void CameraHandler::setup()
{
    drawTexture =false;
    camWidth =640;
    camHeight =480;
    cam = StartCamera(camWidth , camHeight,40,1,false);



}
void CameraHandler::setupDraw()
{

    baseProgram.load("textureShader");


    static const GLfloat quad_vertex_positions[] = {
            0.0f, 0.0f,	1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };
    glGenBuffers(1, &GQuadVertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_positions), quad_vertex_positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



}
void CameraHandler::update()
{


    const void* frame_data; int frame_sz;
    if(cam->BeginReadFrame(0,frame_data,frame_sz)) {

        centers.clear();
        cv::Mat greyMat(camHeight, camWidth, CV_8UC1, const_cast<void * >(frame_data));
        if(drawTexture)
        {

            cv::Mat color;
            cv::cvtColor(greyMat,color,cv::COLOR_GRAY2RGB);
            texture.setMat(color);

        }
        threshold(greyMat, greyMat, 230, 255, 0);


        std::vector <std::vector<cv::Point>> contours;
        cv::findContours(greyMat, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
        for (size_t index = 0; index < contours.size(); index++) {
            cv::Moments moms = cv::moments(cv::Mat(contours[index]));
            cv::Point p = cv::Point(moms.m10 / moms.m00, moms.m01 / moms.m00);

            centers.push_back(p);

        }


        cam->EndReadFrame(0);
    }


}
void CameraHandler::draw(){

    if(drawTexture) {

        GLuint program = baseProgram.id;
        glUseProgram(program);

        glUniform2f(glGetUniformLocation(program, "offset"), -1, -1);
        glUniform2f(glGetUniformLocation(program, "scale"), 2, 2);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);

        glBindBuffer(GL_ARRAY_BUFFER, GQuadVertexBuffer);


        glBindTexture(GL_TEXTURE_2D, texture.Id);

        GLuint loc = glGetAttribLocation(program, "vertex");


        glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 16, 0);
        glEnableVertexAttribArray(loc);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glFinish();
        glFlush();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

}