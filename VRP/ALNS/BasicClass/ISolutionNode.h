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
	ISolutionNode(FerryVehicleTask* _task, enum NodeType type);
	// �����нڵ�״̬���á�������Ϊ��������״̬
	void reset_infeasible();
	// �����нڵ�״̬���á�������Ϊ������״̬
	void reset_feasible();
	// �������絽��ʱ��
	double cal_arrive_earliest_time();	
	// �õ����絽��ʱ��
	double get_arrive_earliest_time();
	// ���ýڵ㣬�ýڵ�����ʱ����Ϣ

	/*˽����*/
	FerryVehicleTask* task;				// 1������

	/*���vehicle����*/
	// ����ϵ��
	double arrive_earliest_time;					// 2�����絽��ʱ��
	double service_earliest_start_time;				// 3���������翪ʼʱ��
	double leave_earliest_time;						// 4�������뿪ʱ��
	
	// ����ϵ��
	double arrive_latest_time;						// 5�����絽��ʱ��
	double service_latest_start_time;				// 6������������ʱ��
	double leave_latest_time;						// 7�������뿪ʱ��

	// ʵ������ʱ��
	double actual_arrive_latest_time_;				// 8��ʵ��������ʱ��
	double actual_service_latest_time_;				// 9��ʵ������ʼ����ʱ��
	double actual_leave_latest_time_;				// 10��ʵ�������뿪ʱ��

	enum NodeState state;							// 11����ǰ�ڵ�״̬
	enum NodeType  type_;							// 12����ǰ�ڵ�����

	// ���г�
	int trip_num;									// 13�����г̱��
};
