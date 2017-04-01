//
// Created by pi on 24/03/17.
//

#include "RadioHandler.h"

void RadioHandler::setup()
{
  radio =new RF24 (25,0,BCM2835_SPI_SPEED_16MHZ);
    const uint8_t pipes[][6] = {"1Node","2Node"};
    radio->begin();


    // optionally, increase the delay between retries & # of retries
    radio->setRetries(0,0);
    // Dump the configuration of the rf unit for debugging
    radio->printDetails();
    radio->openWritingPipe(pipes[0]);
    radio->openReadingPipe(1,pipes[1]);

}
void RadioHandler::send(int type,float value1,float value2,float value3)
{
    radioData.type =type;
    radioData.value1 =value1;
    radioData.value2 =value2;
    radioData.value3 =value3;

    bool ok = radio->write( &radioData, sizeof(radioData) );
/*
    if (!ok){
        printf("failed.\n");
    }else
    {
        printf("ok.\n");
    }
*/
}