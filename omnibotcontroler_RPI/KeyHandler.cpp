//
// Created by pi on 24/03/17.
//

#include "KeyHandler.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#include <iostream>

using namespace std;

void KeyHandler::update()
{


        static int fd = -1;
        const int width=GScreenWidth, height=GScreenHeight;
        static int x=0, y=0;
        const int XSIGN = 1<<4, YSIGN = 1<<5;
        if (fd<0) {
            fd = open("/dev/input/mouse0",O_RDONLY|O_NONBLOCK);
        }
        if (fd>=0) {
            struct {char buttons, dx, dy; } m;
            while (1) {
                int bytes = read(fd, &m, sizeof m);
                if (bytes < (int)sizeof m) {

                    return;
                }
                if (m.buttons&8) {
                    break; // This bit should always be set
                }
                read(fd, &m, 1); // Try to sync up again
            }
            if (m.buttons&3)
                return ;
            x+=m.dx;
            y+=m.dy;
            if (m.buttons&XSIGN)
                x-=256;
            if (m.buttons&YSIGN)
                y-=256;
            if (x<0) x=0;
            if (y<0) y=0;
            if (x>width) x=width;
            if (y>height) y=height;

           // cout << width<<" "<< height<<" " << x<<" "<< y<<" "<< m.dx<<" "<< m.dy<<endl;
            yapp  = height-y;
            xapp  = x;

           // cout << xapp << " " <<yapp<<endl;
        }






}