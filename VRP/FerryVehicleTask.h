#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;
class FerryVehicleTask
{

private:
    int id;
    Flight flight;
    int serviceStartTime;
    int serviceEndTime;
    int serviceTime;
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
        return serviceTime;
    }
    void setServiceTime(int _serviceTime)
    {
        serviceTime = _serviceTime;
    }
};

