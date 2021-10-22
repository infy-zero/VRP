#pragma once
#include <vector>
#include <unordered_map>

#include "AllNodes.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "ALNS_Setting.h"
#include "FerryVehicleTask.h"
#include "Flight.h"
#include "MyException.h"
#include "Util/Util.h"

#define inf IInformation::

#define CONST_DEPOT_CODE 0b100
#define DEPOT_CODE 0b010
#define NODE_CODE 0b001

enum CalDistanceType {CDT_Task, CDT_Index};

static class IInformation
{
public:
	static AllNodes nodes;						// 1�����ж���+��վ�ڵ㣺BGS[0] + CA[1]

	static vector<vector<int>> nodes_indexs;

	static vector<vector<double>> matrix;		// 3���������

	static unordered_map<char, int> flight2FerryVehcle;  // 4������-������ϣ��

	static vector<string> kIndexToPositionName;		// 5���Ǳ�-��������

	static unordered_map<string, int> kPositionNameToIndex;  // 6������-�Ǳ��ϣ��

	static vector<Flight*> flight_flights;			// 7���ɻ�

	static vector<pair<int, int>> positions;	// 8��ÿ���ڵ��Ӧ�����⺽������

	/*�����from��to������ʱ�䣬��λ����*/ // TODO(Lvning): ����ʱ��Ӧ������һ�ڵ�����ڵ㵽��һ�ڵ㿪ʼ�ڵ�
	static double cal_travel_time(AllNodes& nodes, int from_node_index, int to_node_index, int type = 0b011, double fromTime = 0);

	/*�����from��to�����о���*/
	static double cal_travel_distance(AllNodes& nodes, int from_node_index, int to_node_index, int type = 0b011, double fromTime = 0);

};
