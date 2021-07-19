#include <iostream>
#include <string>
#include <tinyxml.h>
#include "Configuration.h"
#include "ALNS.h"
#include "ALNS/BasicClass/IInformation.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "FerryVehicleTask.h"
using namespace std;

void welcomeInterface();
Configuration loadConfiguration();

void loadData(unordered_map<char, int>* flight2FerryVehcle, vector<string>* index2flightName, unordered_map<string, int>* flightName2index, vector<Flight>* flightTasks, vector<FerryVehicleTask>* ferryVehicleTasks, vector<vector<int>>* consequence, vector<vector<double>>* matrix);

bool loadXML();
void csv2task();

// 大领域搜索算法
void ALNS_Entrance();

int main() {
    welcomeInterface();
    // Configuration configuration = loadConfiguration();
    IInformation* information = new IInformation;
    vector<FerryVehicleTask>* ferryVehicleTasks = information->getNodes();
    // 距离矩阵
    vector<vector<double>>* matrix = information->getMatrix();
    // 类型-数量哈希表
    unordered_map<char, int>* flight2FerryVehcle=information->getFlight2FerryVehcle();
    // 角标-名称数组
    vector<string>* index2flightName = information->getIndex2flightName();
    // 名称-角标哈希表
    unordered_map<string, int>* flightName2index = information->getFlightName2index();
    // 飞机
    vector<Flight>* flightTasks = information->getFlightTasks();
    // 约束
    vector<vector<int>>* consequence = information->getConsequence();
    // 读取数据
    loadData(flight2FerryVehcle, index2flightName, flightName2index, flightTasks, ferryVehicleTasks, consequence, matrix);
    ISolution curSol;
    // 将所有任务压入RemovedList
    for (auto fvt : *ferryVehicleTasks)
    {
        ISolutionNode sn(&fvt);
        curSol.addRemovedList(sn);
    }
    ALNS alns;
    
    alns.setInformation(information);
    alns.setSolution(curSol);
    alns.start();
    return 0;
}

void welcomeInterface()
{
    cout << "Welcome to use the Vehicle Routing Problem procedure!" << endl;
}
