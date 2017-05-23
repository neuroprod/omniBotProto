//
//  ArduinoHandler.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 14/05/2017.
//
//

#include "ArduinoHandler.hpp"
#include "cinder/Utilities.h"
using namespace std;
using namespace ci;
using namespace ci::app;

ArduinoHandler::ArduinoHandler()
{
    
}

ArduinoHandler::~ArduinoHandler()
{
    
}

void ArduinoHandler::setup(std::string serialPort)
{
    try {
        Serial::Device dev = Serial::findDeviceByNameContains( serialPort);
        mSerial = Serial::create( dev, 115200 );
        char b = '*';
        while( mSerial->getNumBytesAvailable() > -1 ) {
            b = mSerial->readByte();
            console() << b << "_";
        }
        mSerial->flush();
    }
    catch( SerialExc &exc ) {
        // CI_LOG_EXCEPTION( "coult not initialize the serial device", exc );
        // exit( -1 );
        console()<<"failed to connect to "<<serialPort <<endl;
    }
    
   

    
}
void ArduinoHandler::update()
{
    if(mSerial && mSerial->getNumBytesAvailable() >0)
    {
        //console()<<mSerial->getNumBytesAvailable()<<" ";
        string input = mSerial->readStringUntil('\n');
       // console()<<input.size()<<"----"<< input<<endl;;
        
        vector<string> substrings = split( input, " " );

        if(substrings[0]=="0")
        {
            
            player1->parseControles( substrings);
            
          
        }
        
        
        //mSerial->flush();
    }

}

void ArduinoHandler::sendCommand(std::string command)
{
    if(mSerial){
        mSerial->writeString(command);
    }

}
