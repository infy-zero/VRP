#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;
enum FVTType{DP, VVF, UVF, RF};  // DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight
/*虚拟航班节点所有信息，由于虚拟航班本身时间窗可变动，所以只是最大的时间窗，即违背该时间窗一定错误，但是及时满足该条件也并非可行解*/
class FerryVehicleTask
{
public:
    int id;                 // 1、序号           在nodes中的位置！此处有待优化
    Flight* flight;         // 2、真实航班号      
    int earliestArrivalTime;// 3、最早到达时间 —— 摆渡车允许到达虚拟航班的最早时间
    int serviceStartTime;   // 4、服务最早开始时间 —— 摆渡车允许服务虚拟航班的最早时间
    int serviceEndTime;     // 5、服务最晚开始时间 —— 摆渡车允许到达虚拟航班的最晚时间
    int serviceTime;        // 6、服务时间 —— 上车时间 + 下车时间 + 等待时间（5, if 1）
    enum FVTType type;      // 7、节点类型

    FerryVehicleTask();
    FerryVehicleTask(
        int _id,
        Flight* flight,
        int _serviceStartTime,
        int _serviceEndTime,
        int _serviceTime,
        enum FVTType _type = UVF);
    friend ostream& operator<<(ostream& outs, FerryVehicleTask& flight);
    const int getID()
    {
        return id;
    }
    Flight* getFlight()
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

