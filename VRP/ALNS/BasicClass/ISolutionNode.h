#pragma once

#include <memory>
#include <vector>

#include "FerryVehicleTask.h"
#include "ALNS_Setting.h"

// �ڵ�״̬��δ֪���ѱ����node���ѱ����depot��δ����顢�ڻ����б�
enum NodeState { UNKNOWN, CHECKED_NODE, CHECKED_DEPOT, UNCHECKED_NODE, UNCHECKED_DEPOT, INREMOVELIST };
// �ڵ����ͣ���վ����ͨ�ڵ�
enum NodeType { TYPE_DEPOT, TYPE_NODE, ERROR_TYPE };

using namespace std;
/* 1����һ���ڶɳ���ǰ5 - 8���ӵ��0���ӿ�ʼ�ṩ����0+1���ӽ�������
	2���ڶ����ڶɳ�[-2,1]�����ڵ���,1���ӿ�ʼ�ṩ����
	3���������ڶɳ����Ժ���ǰһ�����뿪ʱLT_n-1��ʼ�ṩ������[LT_n-1 - 2, LT_n-1]ʱ���ڵ��
	4��ͷ������ʱ�䴰���ɱ䣬��������ʼʱ�䴰�ɱ䣨���ܳ�����Ӧ��curTime����
	5��ʱ�䴰��Ϊ���֣���������ʱ�䴰�����⺽��ʱ�䴰��
	6�����������еĽڵ�[minVT,maxVT]���������⺽��ʱ�䴰�����й�ͬ������
	7�����⺽���еĽڵ�[minFT,maxFT]���������⺽���������нڵ�curTime��ͬ������*/
class ISolutionNode
{
public:
	ISolutionNode() = default;
	ISolutionNode(const ISolutionNode& other) = default;
	ISolutionNode(const shared_ptr<FerryVehicleTask>& _task, enum NodeType type);
	void reset_infeasible();
	void reset_feasible();
	double cal_arrive_earliest_time();				// �������絽��ʱ��

	/*˽����*/
	shared_ptr<FerryVehicleTask> task;				// 0������

	/*���vehicle����*/
	double curTime;									// 1�����ڵ㵽��ʱ��
	double arrive_earliest_time;					// 2���Ƿ���Ÿ���
	double service_earliest_start_time;				// 3��ʵ�����絽��ʱ��
	double service_latest_end_time;					// 4��ʵ��������ʱ��

	double minVehicleTime;							// 5�����ڵ����絽��ʱ�䣨������
	double maxVehicleTime;							// 6�����ڵ�������ʱ�䣨������

	vector<pair<double, double>> vft;				// 7�������⺽���Ӧ�İڶɳ�ʱ�䴰
	double minFlightTime;							// 8�����ڵ����絽��ʱ�䣨���⺽�ࣩ
	double maxFlightTime;							// 9�����ڵ�������ʱ�䣨���⺽�ࣩ

	enum NodeState state;							// 10����ǰ�ڵ�״̬
	enum NodeType  type_;							// 11����ǰ�ڵ�����

};
