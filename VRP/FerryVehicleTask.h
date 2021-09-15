#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;

// �ڵ��Ƿ���������DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight
enum FVTType{ FVT_TYPE_DEPOT, VARIABLE_FLIGHT, UNVARIABLE_FLIGHT, REAL_FLIGHT, FVT_UNKNOWN};
/*���⺽��ڵ�������Ϣ���������⺽�౾��ʱ�䴰�ɱ䶯������ֻ������ʱ�䴰����Υ����ʱ�䴰һ�����󣬵��Ǽ�ʱ���������Ҳ���ǿ��н�*/
class FerryVehicleTask
{
public:
    int id;                                         // 1�����           ��nodes�е�λ�ã��˴��д��Ż�
    Flight* flight;                                 // 2����ʵ�����      
    int predefined_earliest_arrival_time;           // 3�����絽��ʱ�� ���� �ڶɳ����������⺽�������ʱ��
    int predefined_earliest_service_start_time;     // 4���������翪ʼʱ�� ���� �ڶɳ�����������⺽�������ʱ��
    int predefined_service_latest_start_time;       // 5����������ʼʱ�� ���� �ڶɳ����������⺽�������ʱ��
    int predefined_service_max_before_;			    // 6�������ǰ�ȴ�ʱ��
    int boarding_time_;                             // 7������ʱ�� ���� �ϳ�ʱ�� + �³�ʱ�� + �ȴ�ʱ�䣨5, if 1��
    int alighting_time_;                            // 8������ʱ�� ���� �³�ʱ��
    enum FVTType type;                              // 9���ڵ�����

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

