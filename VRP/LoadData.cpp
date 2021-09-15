#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>

#include "ALNS_Setting.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "Flight.h"
#include "FerryTaskSetting.h"
#include "FerryVehicleTask.h"
#include "IInformation.h"

/**
 Date:    2021-06-25 20:20:00
 Author:  Lv Ning
 version: 0.1
 Description: This cpp file is used to load the Data.
*/

void loadData();
string Trim(string& str);
time_t strTime2unix(string timeStamp);
void readF2FV(string filepath, unordered_map<char, int>* map);
void LoadComparationTable(string filename);
void csv2task(string filename);
void flight2FerryVehcleTasks();
void loadDisMatrix(string matrixPath, int size);
void showDataInformation();
// 以下为函数区域
// 读入各个场站、远机位、航站楼、临时停车位置距离

// 读入航班需要的摆渡车数量表
void readF2FV(string filepath)
{
	ifstream fin(filepath, ios::in);
	if (!fin.is_open())
		throw "Fail to open the flight2ferry file";
	string line;
	int count = -1;
	while (getline(fin, line))
	{
		count++;
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		char type = Trim(fields[0]).c_str()[0];
		int needVehicle = atoi(Trim(fields[1]).c_str());
		inf flight2FerryVehcle.insert(pair<char, int>(type, needVehicle));
	}
	cout << "Successful to load the flight-ferry vehicle number table." << endl;
}
void LoadComparationTable(string filename)
{
	ifstream fin(filename, ios::in);
	if (!fin.is_open())
		throw "Fail to load the " + filename;
	string line;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
			fields.push_back(field);
		int index = atoi(Trim(fields[0]).c_str()) - 1;
		string name = Trim(fields[1]);
		if (index != inf index2flightName.size())
			throw "Index is not compatible.";
		inf index2flightName.push_back(name);
		inf flightName2index.insert(pair<string, int>(name, index));
		// 读入两个场站
		if (name._Equal("BGS"))
		{
			shared_ptr<FerryVehicleTask> task_bgs(new FerryVehicleTask(index, nullptr, INT_MIN, INT_MAX, 0, 0, FVTType::FVT_TYPE_DEPOT));
			ISolutionNode node_bgs(task_bgs, NodeType::TYPE_DEPOT);
			//node_bgs.
			inf nodes.push_depot(-1, node_bgs);

		}
		else if (name._Equal("CA"))
		{
			shared_ptr<FerryVehicleTask> task_ca(new FerryVehicleTask(index, nullptr, INT_MIN, INT_MAX, 0, 0, FVTType::FVT_TYPE_DEPOT));
			ISolutionNode node_ca(task_ca, NodeType::TYPE_DEPOT);
			inf nodes.push_depot(-2, node_ca);
		}
	}
	cout << "Successful to read the comparation table!" << endl;
}
// 读入航班数据
void csv2task(string filename)
{
	ifstream fin(filename, ios::in);
	if (!fin.is_open())
		throw  "Fail to open the file:" + filename;
	string line;
	int count = -2;
	time_t startTime;
	int max_count = 20;
	while (getline(fin, line) && max_count--)
	{
		count++;
		if (count == -1)
			continue;
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		// int id = atoi(Trim(fields[0]).c_str());
		int id = count;
		string flightcompany = Trim(fields[1]);
		string flighttype = Trim(fields[2]);
		char flightclass = Trim(fields[3])[0];
		string flightnumber = Trim(fields[4]);
		int ferryVehicles = inf flight2FerryVehcle.at(flightclass);
		string apron = Trim(fields[5]);
		string stand = Trim(fields[6]);
		time_t rdy = strTime2unix(Trim(fields[7]) + ":00");
		string terminal = Trim(fields[10]);
		if (count == 0)
		{
			startTime = rdy;
			//cout << "startTime=" << startTime << endl;
		}
		// 判断抽取东/西/所有区——目前抽取东区数据
		if (A_S range == EAST && terminal.find("T3") == -1)
			continue;
		else if (A_S range == WEST && terminal.find("T3") != -1)
			continue;
		rdy -= startTime;
		enum Direction direction = Trim(fields[8])._Equal("arrive") ? DIRECTION_ARRIVE : DIRECTION_DEPART;
		Flight* tmp = new Flight(id, flightcompany, flighttype, flightclass, ferryVehicles, apron, stand,
			inf flightName2index.at(stand), static_cast<int>(rdy), direction, terminal, inf flightName2index.at(terminal));
		inf flightTasks.push_back(tmp);
	}

	cout << "Successful to load the flight tasks!" << endl;
}
// 将航班转换为摆渡车任务
void flight2FerryVehcleTasks()
{
	int num = 0;
	// transfer flightTasks to ferryVehicleTasks
	for (int j = 0; j < inf flightTasks.size(); j++)
	{
		auto flight = inf flightTasks.at(j);
		vector<int> con;
		for (int i = 0; i < flight->ferryVehicles; i++)
		{
			// 按照顺序记录对应节点的位置 pair<组号，组内序号>?g
			inf positions.push_back(make_pair(j, i));
			con.push_back(num);
			int service_start_time	= INT_MAX;	// 服务开始时间
			int service_end_time		= INT_MIN;	// 服务结束时间
			int boarding_time		= INT_MAX;  // 上车时间
			int alighting_time		= INT_MAX;  // 下车时间
			// 服务时间
			if (flight->direction == DIRECTION_ARRIVE || flight->direction == DIRECTION_DEPART) {
				boarding_time = fts FTS_boardingTime;
				alighting_time = fts FTS_alightingTime;
			}
			else {
				throw MyException("Direction type is wrong.");
			}
			// 最早开始服务时间
			if (i == 0) { // 第一辆车最早提前5+timewindow(3)=8分钟到达
				service_start_time = flight->getServiceStartTime() - fts FTS_firstBefore - fts FTS_timewindow;
			}
			else { // 剩下车辆按照服务时间增加 开始时间最小值 = 车辆开始时间 + 上车时间 * ( i - 1 )
				service_start_time = flight->getServiceStartTime() + (i - 1) * FerryTaskSetting::FTS_boardingTime;
			}
			// 最晚开始服务时间
			if (i == 0) { // 第一辆车最晚提前五分钟到达
				service_end_time = flight->getServiceStartTime() - FerryTaskSetting::FTS_firstBefore;
			}
			else { // 剩下车辆按照服务时间增加：超过这个时间一定错误（这个时间是底线）
				service_end_time = flight->getServiceStartTime() + (i - 1) * (FerryTaskSetting::FTS_boardingTime + FerryTaskSetting::FTS_max_delta_time);
			}
	 
			enum FVTType type = FVT_UNKNOWN; // 节点类型，前两个时间窗不可变UVF，后面节点动态时间窗可变VVF
			if (i < 2)
				type = UNVARIABLE_FLIGHT;
			else
				type = VARIABLE_FLIGHT;
			shared_ptr<FerryVehicleTask>
				tmp_task(new FerryVehicleTask(num++,
					flight,
					service_start_time,
					service_end_time,
					fts FTS_arrive_before_start_timewindow,
					boarding_time,
					alighting_time,
					type));
			ISolutionNode tmp_node(tmp_task, TYPE_NODE);
			inf nodes.push_node(tmp_node.task->id, tmp_node);
		}
		inf consequence.push_back(con);

	}
	cout << "Ferry vehicles tasks are loaded successfully!" << endl;
}
// 读入距离矩阵
void loadDisMatrix(string filePath, int size)
{
	ifstream fin(filePath);
	if (!fin.is_open())
		throw  "fail to open" + filePath;
	string line;
	while (getline(fin, line))
	{
		vector<double> segments;
		istringstream sin(line);
		string segment;

		while (getline(sin, segment, ','))
		{
			segments.push_back(atof(segment.c_str()));
		}
		if (segments.size() != size)
			cout << "Data Wrong" << endl;
		inf matrix.push_back(segments);
	}
	cout << "Distance matrix is load successfully!" << endl;
}
// 显示数据
void showDataInformation()
{
	cout << "The Data is loaded!" << endl;
	cout << "**********************" << endl;
	cout << "  Data Information:" << endl;
	for (auto node : inf flight2FerryVehcle)
		cout << "\t" << node.first << "\t" << node.second << endl;
	cout << "\tflights:" << inf flightTasks.size() << endl;
	cout << "\ttasks:" << inf nodes.nodes_.size() << endl;
	for (auto node_entry : inf nodes.nodes_) {
		cout << *(node_entry.second.task);
	}
	cout << "\tdepots:" << inf nodes.depots_.size() << endl;
	/*for (auto depot_entry : inf nodes.depots_) {
		cout << *(depot_entry.second.task);
	}*/
	cout << "**********************" << endl;
}

// Load all the data needed
void loadData()
{
	/* 读取航班需要的摆渡车数量列表*/
	string flight2ferry = "flight2ferry.csv";
	// 航班需要的摆渡车数量列表
	readF2FV(flight2ferry);

	/* 读取停机位对照表*/
	string flightindex = "comparationTable2.txt";
	// 序号到停机位名称列表
	// 停机位到序号名称列表
	LoadComparationTable(flightindex);

	/* 读取飞机列表*/
	// 飞机列表
	string flightCSV = "finalTasks80.csv";
	csv2task(flightCSV);

	/* 生成摆渡车任务列表*/
	// 同一飞机的摆渡车任务列表
	// 摆渡车任务列表
	flight2FerryVehcleTasks();

	/*读取距离矩阵*/
	string matrixPath = "distance.txt";
	loadDisMatrix(matrixPath, inf index2flightName.size());

	/* 打印数据*/
	showDataInformation();
}
