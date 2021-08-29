#pragma once
#include <vector>
#include <unordered_map>
#include "ALNS/BasicClass/ISolutionNode.h"
#include "FerryVehicleTask.h"
#include "ALNS/setting/ALNS_Setting.h"
#include "Flight.h"

#define inf IInformation::

using namespace std;
static class IInformation
{
public:
	static vector<FerryVehicleTask*> nodes;		// 1����������ڵ�

	static vector<FerryVehicleTask*> depots;	// 2�����г�վ�ڵ㣺BGS[0] + CA[1]
	
	static vector<vector<double>> matrix;		// 3���������
	
	static unordered_map<char, int> flight2FerryVehcle;	 // 4������-������ϣ��
	
	static vector<string> index2flightName;		// 5���Ǳ�-��������
	
	static unordered_map<string, int> flightName2index;  // 6������-�Ǳ��ϣ��
	
	static vector<Flight*> flightTasks;			// 7���ɻ�
	
	static vector<vector<int>> consequence;		// 8��Լ��

	static vector<pair<int, int>> positions;   // 9��ÿ���ڵ��Ӧ�����⺽������

	/*�����from��to������ʱ��*/
	static double calTravelTime(int from, int to, double fromTime = 0)
	{
		return matrix[from][to]/A_S speed;
	}
};