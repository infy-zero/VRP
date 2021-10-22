#include <iostream>
#include <string>
#include <tinyxml.h>

#include "ALNS.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "Configuration.h"
#include "FerryVehicleTask.h"
#include "IInformation.h"


using namespace std;

void welcomeInterface();
Configuration loadConfiguration();

void loadData();

bool loadXML();
void csv2task();

// 大领域搜索算法
void ALNS_Entrance();
void qt_environment();

int main() {
    welcomeInterface();
    Configuration configuration = loadConfiguration();
   
    // 读取数据
    loadData();
    ALNS alns_cur;
    alns_cur.start();
    return 0;
}

void qt_environment() {

}

void welcomeInterface()
{
    cout << "Welcome to use the Vehicle Routing Problem procedure!" << endl;
}
