#pragma once
#include <vector>
#include <unordered_map>

#include "AllNodes.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "ALNS_Setting.h"
#include "FerryVehicleTask.h"
#include "Flight.h"
#include "MyException.h"

#define inf IInformation::

enum VehicleType { LARGE }; // ��������

static class IInformation
{
public:
	static AllNodes nodes;						// 1�����ж���+��վ�ڵ㣺BGS[0] + CA[1]

	static vector<vector<double>> matrix;		// 3���������

	static unordered_map<char, int> flight2FerryVehcle;
	// 4������-������ϣ��

	static vector<string> index2flightName;		// 5���Ǳ�-��������

	static unordered_map<string, int> flightName2index;
	// 6������-�Ǳ��ϣ��

	static vector<Flight*> flightTasks;			// 7���ɻ�

	static vector<vector<int>> consequence;		// 8��Լ��

	static vector<pair<int, int>> positions;	// 9��ÿ���ڵ��Ӧ�����⺽������

	/*�����from��to������ʱ��*/
	static double cal_travel_time(int pre, int next, double fromTime = 0)
	{
		int from = nodes.get(pre)->task->id;
		int to   = nodes.get(next)->task->id;
		return matrix[from][to] / A_S speed;
	}

	// ��ó����ɱ�
	static double get_vehicle_cost(enum VehicleType vehicle_type) {
		if (vehicle_type == LARGE) {
			return 100;
		}
		throw MyException("No vehicle type found!");
		return INT_MIN;
	}
	// ��ó�����ʻ�ɱ�
	static double get_vehicle_per_cost(enum VehicleType vehicle_type) {
		if (vehicle_type == LARGE) {
			return 1;
		}
		throw MyException("No vehicle type found!");
		return INT_MIN;
	}

};
