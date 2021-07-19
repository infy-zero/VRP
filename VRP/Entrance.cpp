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

// �����������㷨
void ALNS_Entrance();

int main() {
    welcomeInterface();
    // Configuration configuration = loadConfiguration();
    IInformation* information = new IInformation;
    vector<FerryVehicleTask>* ferryVehicleTasks = information->getNodes();
    // �������
    vector<vector<double>>* matrix = information->getMatrix();
    // ����-������ϣ��
    unordered_map<char, int>* flight2FerryVehcle=information->getFlight2FerryVehcle();
    // �Ǳ�-��������
    vector<string>* index2flightName = information->getIndex2flightName();
    // ����-�Ǳ��ϣ��
    unordered_map<string, int>* flightName2index = information->getFlightName2index();
    // �ɻ�
    vector<Flight>* flightTasks = information->getFlightTasks();
    // Լ��
    vector<vector<int>>* consequence = information->getConsequence();
    // ��ȡ����
    loadData(flight2FerryVehcle, index2flightName, flightName2index, flightTasks, ferryVehicleTasks, consequence, matrix);
    ISolution curSol;
    // ����������ѹ��RemovedList
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
