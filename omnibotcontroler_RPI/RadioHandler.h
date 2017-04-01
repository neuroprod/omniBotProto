//
// Created by pi on 24/03/17.
//

#ifndef BOTTRACKER_RADIOHANDLER_H
#define BOTTRACKER_RADIOHANDLER_H
#include <iostream>
#include <stdio.h>
#include <RF24/RF24.h>
#include <RF24/bcm2835.h>
class RadioHandler {
public:
    RadioHandler(){};
    void setup();
    void send(int type,float value1,float value2,float value3);
    RF24 * radio;//( RPI_V2_GPIO_P2_23,  RPI_V2_GPIO_P2_24,  BCM2835_SPI_SPEED_8MHZ);


    struct RadioData
    {
        int type;
        float value1;
        float value2;
        float value3;

    } radioData;
};


#endif //BOTTRACKER_RADIOHANDLER_H
