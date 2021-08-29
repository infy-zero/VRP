#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;
enum FVTType{DP, VVF, UVF, RF};  // DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight
/*���⺽��ڵ�������Ϣ���������⺽�౾��ʱ�䴰�ɱ䶯������ֻ������ʱ�䴰����Υ����ʱ�䴰һ�����󣬵��Ǽ�ʱ���������Ҳ���ǿ��н�*/
class FerryVehicleTask
{
public:
    int id;                 // 1�����           ��nodes�е�λ�ã��˴��д��Ż�
    Flight* flight;         // 2����ʵ�����      
    int earliestArrivalTime;// 3�����絽��ʱ�� ���� �ڶɳ����������⺽�������ʱ��
    int serviceStartTime;   // 4���������翪ʼʱ�� ���� �ڶɳ�����������⺽�������ʱ��
    int serviceEndTime;     // 5����������ʼʱ�� ���� �ڶɳ����������⺽�������ʱ��
    int serviceTime;        // 6������ʱ�� ���� �ϳ�ʱ�� + �³�ʱ�� + �ȴ�ʱ�䣨5, if 1��
    enum FVTType type;      // 7���ڵ�����

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

