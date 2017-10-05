//
//  Butterfly.cpp
//  Game_PC
//
//  Created by Kris Temmerman on 03/10/2017.
//
//

#include "Butterfly.hpp"




using namespace ci;
using namespace ci::app;
using namespace std;


ButterflyRef Butterfly::create()
{
    return  make_shared<Butterfly>();
}
