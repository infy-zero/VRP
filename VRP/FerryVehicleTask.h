#pragma once
#include <iostream>
#include "Flight.h"
#include "ALNS/BasicClass/ALNSNode.h"

using namespace std;
class FerryVehicleTask:public ALNSNode
{
private:
    int id;
    Flight flight;
    int serviceStartTime;
    int serviceEndTime;
    int boardingTime;
public:
    FerryVehicleTask();
    FerryVehicleTask(
        int _id,
        Flight& flight,
        int _serviceStartTime,
        int _serviceEndTime,
        int _serviceTime);
    friend ostream& operator<<(ostream& outs, FerryVehicleTask& flight);
    const int getID()
    {
        return id;
    }
    Flight& getFlight()
    {
        return flight;
    }
    const int getServiceStartTime()
    {
        return serviceStartTime;
    }
    const int getServiceEndTime()
    {
        return serviceEndTime;
    }
    const int getServiceTime()
    {
        return boardingTime;
    }
    void setServiceTime(int _serviceTime)
    {
        boardingTime = _serviceTime;
    }
};

