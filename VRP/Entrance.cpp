#include <iostream>
#include <string>
#include <tinyxml.h>
#include "Configuration.h"
#include "ALNS.h"
#include "ALNS/BasicClass/IInformation.h"
using namespace std;

void welcomeInterface();
Configuration loadConfiguration();
void loadData();

bool loadXML();
void csv2task();

// ¥Û¡Ï”ÚÀ—À˜À„∑®
void ALNS_Entrance();

int main() {
    welcomeInterface();
    IInformation* information = new IInformation;
    // Configuration configuration = loadConfiguration();
    //loadData();
    ALNS alns;
    alns.registerInformation(information);
    alns.start();
    return 0;
}

void welcomeInterface()
{
    cout << "Welcome to use the Vehicle Routing Problem procedure!" << endl;
}
