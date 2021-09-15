#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;

// 节点是否允许变更：DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight
enum FVTType{ FVT_TYPE_DEPOT, VARIABLE_FLIGHT, UNVARIABLE_FLIGHT, REAL_FLIGHT, FVT_UNKNOWN};
/*虚拟航班节点所有信息，由于虚拟航班本身时间窗可变动，所以只是最大的时间窗，即违背该时间窗一定错误，但是及时满足该条件也并非可行解*/
class FerryVehicleTask
{
public:
    int id;                                         // 1、序号           在nodes中的位置！此处有待优化
    Flight* flight;                                 // 2、真实航班号      
    int predefined_earliest_arrival_time;           // 3、最早到达时间 —— 摆渡车允许到达虚拟航班的最早时间
    int predefined_earliest_service_start_time;     // 4、服务最早开始时间 —— 摆渡车允许服务虚拟航班的最早时间
    int predefined_service_latest_start_time;       // 5、服务最晚开始时间 —— 摆渡车允许到达虚拟航班的最晚时间
    int predefined_service_max_before_;			    // 6、最大提前等待时间
    int boarding_time_;                             // 7、服务时间 —— 上车时间 + 下车时间 + 等待时间（5, if 1）
    int alighting_time_;                            // 8、服务时间 —— 下车时间
    enum FVTType type;                              // 9、节点类型

    FerryVehicleTask() = default;
    FerryVehicleTask(
        int _id,
        Flight* flight,
        int _serviceStartTime,
        int _serviceEndTime,
        int predefined_service_max_before,
        int boarding_time,
        int alighting_time,
        enum FVTType _type = UNVARIABLE_FLIGHT);
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
        return predefined_earliest_service_start_time;
    }
    const int getServiceEndTime()
    {
        return predefined_service_latest_start_time;
    }
    const int getServiceTime()
    {
        return boarding_time_;
    }
    void setServiceTime(int _serviceTime)
    {
        boarding_time_ = _serviceTime;
    }
};

