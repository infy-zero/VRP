#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;

// �ڵ��Ƿ���������DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight
enum FVTType{ FVT_TYPE_DEPOT, VARIABLE_FLIGHT, UNVARIABLE_FLIGHT, REAL_FLIGHT};
/*���⺽��ڵ�������Ϣ���������⺽�౾��ʱ�䴰�ɱ䶯������ֻ������ʱ�䴰����Υ����ʱ�䴰һ�����󣬵��Ǽ�ʱ���������Ҳ���ǿ��н�*/
class FerryVehicleTask
{
public:
    int id;                 // 1�����           ��nodes�е�λ�ã��˴��д��Ż�
    Flight* flight;         // 2����ʵ�����      
    int predefined_earliest_arrival_time;// 3�����絽��ʱ�� ���� �ڶɳ����������⺽�������ʱ��
    int predefined_earliest_service_start_time;   // 4���������翪ʼʱ�� ���� �ڶɳ�����������⺽�������ʱ��
    int predefined_service_latest_start_time;     // 5����������ʼʱ�� ���� �ڶɳ����������⺽�������ʱ��
    int serviceTime;        // 6������ʱ�� ���� �ϳ�ʱ�� + �³�ʱ�� + �ȴ�ʱ�䣨5, if 1��
    enum FVTType type;      // 7���ڵ�����

    FerryVehicleTask();
    FerryVehicleTask(
        int _id,
        Flight* flight,
        int _serviceStartTime,
        int _serviceEndTime,
        int _serviceTime,
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
        return serviceTime;
    }
    void setServiceTime(int _serviceTime)
    {
        serviceTime = _serviceTime;
    }
};

