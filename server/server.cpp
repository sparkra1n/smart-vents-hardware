#include "server.h"
#include "../client/vent.h"

#include <stdio.h>
#include <vector>
#include <Arduino.h>

class Server
{
    private: 
        std::vector<Vent> vents;
        const char* ssid;
        const char pswd;
    
    public:
        Server() : vents(vents), ssid(ssid), pswd(pswd) {}


};

int main()
{
    while (1)
    {
        
    }
}