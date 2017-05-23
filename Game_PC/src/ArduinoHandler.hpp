//
//  ArduinoHandler.hpp
//  Game_PC
//
//  Created by Kris Temmerman on 14/05/2017.
//
//

#ifndef ArduinoHandler_hpp
#define ArduinoHandler_hpp
#include "cinder/app/App.h"
#include "cinder/Serial.h"
#include "Player.hpp"

class ArduinoHandler
{
    
public:
    ArduinoHandler();
    ~ArduinoHandler();
    void setup(std::string serialPort);
    void update();
    void sendCommand(std::string command);
    ci::SerialRef	mSerial;
    
    
    PlayerRef player1;
    PlayerRef player2;
    
};
#endif /* ArduinoHandler_hpp */
