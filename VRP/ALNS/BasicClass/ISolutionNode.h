#pragma once

#include <memory>
#include <vector>

#include "FerryVehicleTask.h"
#include "ALNS_Setting.h"

enum NodeState { CHECKED, UNCHECKED, INREMOVELIST };   // �ڵ�״̬���ڵ�ǰ�⡢�ڻ����б�

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
	ISolutionNode(const shared_ptr<FerryVehicleTask>& _task);

	shared_ptr<FerryVehicleTask> task;// 0������
	bool isUpdate = true;		// 1���Ƿ���Ÿ���

	double curTime;				// 2�����ڵ㵽��ʱ��
	double minTime;				// 3��ʵ�����絽��ʱ��
	double maxTime;				// 4��ʵ��������ʱ��

	double minVehicleTime;		// 5�����ڵ����絽��ʱ�䣨������
	double maxVehicleTime;		// 6�����ڵ�������ʱ�䣨������

	vector<pair<double, double>> vft; // 7�������⺽���Ӧ�İڶɳ�ʱ�䴰
	double minFlightTime;		// 8�����ڵ����絽��ʱ�䣨���⺽�ࣩ
	double maxFlightTime;		// 9�����ڵ�������ʱ�䣨���⺽�ࣩ

	enum NodeState state;		// 10����ǰ�ڵ�״̬


};
