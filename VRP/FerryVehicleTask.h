#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;

// 节点是否允许变更：DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight，
enum FVTType{ FVT_TYPE_DEPOT, UNVARIABLE_FIRST, UNVARIABLE_SECOND, VARIABLE_FLIGHT, REAL_FLIGHT, FVT_UNKNOWN};

/*虚拟航班节点所有信息，由于虚拟航班本身时间窗可变动，所以只是最大的时间窗，即违背该时间窗一定错误，但是及时满足该条件也并非可行解*/
class FerryVehicleTask
{
public:
    FerryVehicleTask() = default;
    FerryVehicleTask(
        int _id,
        Flight* flight,
        const double& serviceStartTime,
        const double& serviceEndTime,
        const double& predefined_service_max_before,
        const double& boarding_time,
        const double& alighting_time,
        const double& extra_service_time_,
        int from_index,
        int end_index,
        enum FVTType _type);

    friend ostream& operator<<(ostream& outs, FerryVehicleTask& flight);
    const int getID();
    Flight* getFlight();
    const double get_predefined_earliest_service_start_time();
    const double get_predefined_service_latest_start_time();
    const double get_predefined_earliest_arrival_time();
    const double get_boarding_time();
    void set_boarding_time(double boarding_time);
    // 上车时间 + 下车时间 + 旅行时间
    double get_total_service_time();

    int id_;                                            // 1、序号           在nodes中的位置！此处有待优化
    Flight* flight_;                                    // 2、真实航班号      
    double predefined_earliest_arrival_time_;           // 3、最早到达时间 —— 摆渡车允许到达虚拟航班的最早时间
    double predefined_latest_arrival_time_;             // 4、最晚到达时间 —— 摆渡车允许到达虚拟航班的最晚时间，通常和摆渡车允许到达虚拟航班的最晚时间相同，当且仅当第一辆摆渡车任务不同于其最晚服务时间。
    double predefined_earliest_service_start_time_;     // 5、服务最早开始时间 —— 摆渡车允许服务虚拟航班的最早时间(包含boarding_time_了)
    double predefined_service_latest_start_time_;       // 6、服务最晚开始时间 —— 摆渡车允许到达虚拟航班的最晚时间
    double predefined_earliest_leave_time_;             // 7、最早离开时间
    double predefined_service_max_before_;			    // 8、最大提前等待时间
    double boarding_time_;                              // 9、服务时间 —— 上车时间 + 下车时间 + 等待时间（5, if 1）
    double alighting_time_;                             // 10、服务时间 —— 下车时间
    double travel_time_;                                // 11、服务时间 —— 旅行时间     
    double extra_service_time_;                         // 12、额外旅行时间，只针对于第一个节点
    double total_service_time_;                         // 13、总服务时间 = 上车+下车+旅行
    double service_distance_;                            // 14、服务总里程
    int from_index_;                                    // 15、开始节点序号
    int end_index_;                                     // 16、结束节点序号
    enum FVTType type_;                                 // 17、节点类型
};

