//
// Created by Kris Temmerman on 21/03/17.
//
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "bcm_host.h"

#include "GLProgram.h"

using namespace std;
void GLProgram::load( string name)
{
    Src =NULL;

    string nameV =name+".v.glsl";
    string nameF =name+".f.glsl";
    GLuint  vertexShader =loadShader(nameV.c_str() , GL_VERTEX_SHADER);
    GLuint  fragmentShader =loadShader(nameF.c_str(), GL_FRAGMENT_SHADER);

    id= glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    // Prints the information log for a program object
    char log[1024];
    glGetProgramInfoLog(id,sizeof log,NULL,log);
    printf("%d:program:\n%s\n", id, log);
cout<< log<<endl;


}
void GLProgram::bind()
{
    glUseProgram(id);

}
GLuint GLProgram::loadShader(const char* filename, GLenum type)
{
    assert(!Src);
    FILE* f = fopen(filename, "rb");
    assert(f);
    fseek(f,0,SEEK_END);
    int sz = ftell(f);
    fseek(f,0,SEEK_SET);
    Src = new GLchar[sz+1];
    fread(Src,1,sz,f);
    Src[sz] = 0; //null terminate it!
    fclose(f);

    //now create and compile the shader

    GLuint Id = glCreateShader(type);
    glShaderSource(Id, 1, (const GLchar**)&Src, 0);
    glCompileShader(Id);


    //compilation check
    GLint compiled;
    glGetShaderiv(Id, GL_COMPILE_STATUS, &compiled);
    if(compiled==0)
    {
        printf("Failed to compile vertex shader %s:\n%s\n", filename, Src);
        printShaderInfoLog(Id);
        glDeleteShader(Id);
        return 0;
    }
    else
    {
        printf("Compiled vertex shader %s:\n%s\n", filename, Src);
    }

    return Id;
}
void GLProgram::printShaderInfoLog(GLint shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0)
    {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cout << "InfoLog : " << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}