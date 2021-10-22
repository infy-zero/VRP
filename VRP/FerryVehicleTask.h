#pragma once
#include <iostream>
#include "Flight.h"

using namespace std;

// �ڵ��Ƿ���������DP = depot , VVF = variable virtual flight, UVF = unvariable virtual flight, RF = real flight��
enum FVTType{ FVT_TYPE_DEPOT, UNVARIABLE_FIRST, UNVARIABLE_SECOND, VARIABLE_FLIGHT, REAL_FLIGHT, FVT_UNKNOWN};

/*���⺽��ڵ�������Ϣ���������⺽�౾��ʱ�䴰�ɱ䶯������ֻ������ʱ�䴰����Υ����ʱ�䴰һ�����󣬵��Ǽ�ʱ���������Ҳ���ǿ��н�*/
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
    // �ϳ�ʱ�� + �³�ʱ�� + ����ʱ��
    double get_total_service_time();

    int id_;                                            // 1�����           ��nodes�е�λ�ã��˴��д��Ż�
    Flight* flight_;                                    // 2����ʵ�����      
    double predefined_earliest_arrival_time_;           // 3�����絽��ʱ�� ���� �ڶɳ����������⺽�������ʱ��
    double predefined_latest_arrival_time_;             // 4��������ʱ�� ���� �ڶɳ����������⺽�������ʱ�䣬ͨ���Ͱڶɳ����������⺽�������ʱ����ͬ�����ҽ�����һ���ڶɳ�����ͬ�����������ʱ�䡣
    double predefined_earliest_service_start_time_;     // 5���������翪ʼʱ�� ���� �ڶɳ�����������⺽�������ʱ��(����boarding_time_��)
    double predefined_service_latest_start_time_;       // 6����������ʼʱ�� ���� �ڶɳ����������⺽�������ʱ��
    double predefined_earliest_leave_time_;             // 7�������뿪ʱ��
    double predefined_service_max_before_;			    // 8�������ǰ�ȴ�ʱ��
    double boarding_time_;                              // 9������ʱ�� ���� �ϳ�ʱ�� + �³�ʱ�� + �ȴ�ʱ�䣨5, if 1��
    double alighting_time_;                             // 10������ʱ�� ���� �³�ʱ��
    double travel_time_;                                // 11������ʱ�� ���� ����ʱ��     
    double extra_service_time_;                         // 12����������ʱ�䣬ֻ����ڵ�һ���ڵ�
    double total_service_time_;                         // 13���ܷ���ʱ�� = �ϳ�+�³�+����
    double service_distance_;                            // 14�����������
    int from_index_;                                    // 15����ʼ�ڵ����
    int end_index_;                                     // 16�������ڵ����
    enum FVTType type_;                                 // 17���ڵ�����
};

