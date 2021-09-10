#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "AllNodes.h"
#include "IInformation.h"
#include "ALNS/BasicClass/ISolutionNode.h"

using namespace std;

class IVehicle
{
public:
	IVehicle() = delete;
	IVehicle(int _depot, AllNodes* _nodes);
	IVehicle(const IVehicle& other) = default;
	
	int at(int index);
	double cal_length();
	double cal_cost();
	bool beyond_max_length();
	void insert(int pos, int node);
	int size();


	vector<int> vehicle_nodes;
	AllNodes* nodes;
	int depot_;
	double vehicle_cost; //	车辆总成本
	double vehicle_per_cost; // 路径成本
	double vehicle_max_length;
	enum VehicleType vehicle_type = LARGE;
};

