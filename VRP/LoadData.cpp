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
	vector<int> const_depot_indexs;
	while (getline(fin, line))
	{
		istringstream sin(line);
		vector<string> fields;
		string field;
		while (getline(sin, field, ','))
			fields.push_back(field);
		int index = atoi(Trim(fields[0]).c_str()) - 1;
		string name = Trim(fields[1]);
		if (index != inf kIndexToPositionName.size())
			throw "Index is not compatible.";
		inf kIndexToPositionName.push_back(name);
		inf kPositionNameToIndex.insert(pair<string, int>(name, index));
		// 读入两个场站 TODO(Lvning): 修正输入task的参数，虽然没有影响
		if (name._Equal("BGS"))
		{
			FerryVehicleTask* task_bgs = new FerryVehicleTask(index, nullptr, -A_S time_max_value, A_S time_max_value, 0.0, 0.0, 0.0, 0.0, index, index, FVTType::FVT_TYPE_DEPOT);
			ISolutionNode* node_bgs = new ISolutionNode(task_bgs, NodeType::TYPE_DEPOT);
			//node_bgs.
			inf nodes.push_const_depot(-1, node_bgs);
			const_depot_indexs.push_back(-1);

		}
		else if (name._Equal("CA"))
		{
			FerryVehicleTask* task_ca = new FerryVehicleTask(index, nullptr, -A_S time_max_value, A_S time_max_value, 0.0, 0.0, 0.0, 0.0, index, index, FVTType::FVT_TYPE_DEPOT);
			ISolutionNode* node_ca = new ISolutionNode(task_ca, NodeType::TYPE_DEPOT);
			inf nodes.push_const_depot(-2, node_ca);
			const_depot_indexs.push_back(-2);
		}
	}
	inf nodes_indexs.at(0b100) =  const_depot_indexs;
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
	int max_count = A_S max_count;
	while (getline(fin, line) && max_count > 0)
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
		if (A_S range == EAST && terminal.find("T3") == -1) {
			continue;
		}
		else if (A_S range == WEST && terminal.find("T3") != -1)
		{
			continue;
		}
		// 控制输入参数的数量
		max_count -= A_S flight_type_to_count.at(flightclass);
		rdy -= startTime;
		enum Direction direction = Trim(fields[8])._Equal("arrive") ? DIRECTION_ARRIVE : DIRECTION_DEPART;
		Flight* tmp = new Flight(id, flightcompany, flighttype, flightclass, ferryVehicles, apron, stand,
			inf kPositionNameToIndex.at(stand), static_cast<int>(rdy), direction, terminal, inf kPositionNameToIndex.at(terminal));
		inf flight_flights.push_back(tmp);
	}
	fin.close();
	cout << "Successful to load the flight tasks!" << endl;
}
// 将航班转换为摆渡车任务
void flight2FerryVehcleTasks()
{
	int num = 0;
	// transfer flight_flights to ferryVehicleTasks
	vector<int> node_indexs;
	for (int j = 0; j < inf flight_flights.size(); j++)
	{
		auto flight = inf flight_flights.at(j);
		vector<int> con;
		for (int i = 0; i < flight->ferryVehicles && i< A_S max_count; i++) // 保证最大节点数
		{
			// 按照顺序记录对应节点的位置 pair<组号，组内序号>?g
			inf positions.push_back(make_pair(j, i));
			con.push_back(num);
			double service_start_time;	// 服务开始时间
			double service_end_time;	// 服务结束时间
			double boarding_time;  // 上车时间
			double alighting_time;  // 下车时间
			// 服务时间
			if (flight->direction == DIRECTION_ARRIVE || flight->direction == DIRECTION_DEPART) {
				boarding_time = fts FTS_boardingTime;
				alighting_time = fts FTS_alightingTime;
			}
			else {
				throw MyException("Direction type is wrong.");
			}
			// 航班节点时间窗类型
			enum FVTType type = FVT_UNKNOWN; // 节点类型，前两个时间窗不可变UVF，后面节点动态时间窗可变VVF
			if (i == 0) {
				type = UNVARIABLE_FIRST;
			} else if (i == 1) {
				type = UNVARIABLE_SECOND;
			} else if (i >= 2){
				type = VARIABLE_FLIGHT;
			} else {
				throw MyException("Error index input.");
			}
			double extra_service_time; 
			// 计算服务最大时间窗口
			if ( i == 0 ) {
				// 服务时间
				service_start_time = flight->serviceStartTime - fts FTS_firstBefore;// 第一辆车最早提前5+timewindow(3)=8分钟到达
				service_end_time = flight->serviceStartTime - fts FTS_firstBefore;// 第一辆车最晚提前五分钟到达
				extra_service_time = fts FTS_firstBefore; // 第一个任务多5分钟服务时间
			} else {
				// 服务时间
				service_start_time = flight->serviceStartTime;// 剩下车辆0时刻开始服务
				service_end_time = flight->serviceStartTime + fts FTS_max_time_window;// 5分钟内全部到达
				extra_service_time = 0;
			}
			enum Direction direction = flight->direction;
			int from_index, end_index;
			// 任务时间、出发点和重点和方向有关，到达航班任务：远机位->航站楼，离开航班：航站楼->远机位
			if (direction == DIRECTION_DEPART) {
				from_index = flight->terminal_index;
				end_index = flight->stand_index;
			} else if (direction == DIRECTION_ARRIVE) {
				from_index = flight->stand_index; 
				end_index = flight->terminal_index;
			} else{
				throw MyException("Wrong direction.");
			}
			// 将摆渡车任务压入flight_task
			FerryVehicleTask*
				tmp_task = new FerryVehicleTask(num++,
					flight,
					service_start_time,
					service_end_time,
					fts FTS_arrive_before_start_timewindow,
					boarding_time,
					alighting_time,
					extra_service_time,
					from_index,
					end_index,
					type);
			ISolutionNode* tmp_node = new ISolutionNode(tmp_task, TYPE_NODE);
			inf nodes.push_node(tmp_node->task->id_, tmp_node);
			node_indexs.push_back(tmp_node->task->id_);
		}
		inf nodes.push_back_flight_tasks(con);
	}
	inf nodes_indexs.at(0b001) = node_indexs;
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
// 计算各个节点基础信息
void cal_distance() {
	// 更新节点信息
	for (int i = 0; i < inf nodes_indexs.at(0b001).size(); i++) {
		ISolutionNode* node = inf nodes.get_node(inf nodes_indexs.at(0b001).at(i));
		FerryVehicleTask* task = node->task;
		task->travel_time_ = inf cal_travel_time(inf nodes, inf nodes_indexs.at(0b001).at(i), inf nodes_indexs.at(0b001).at(i), 0b001);
		task->total_service_time_ = task->boarding_time_ + task->alighting_time_ + task->travel_time_ + task->extra_service_time_;
		task->service_distance_ = inf cal_travel_distance(inf nodes, inf nodes_indexs.at(0b001).at(i), inf nodes_indexs.at(0b001).at(i), 0b001);
	}
	// 更新固定场站信息
	for (int i = 0; i < inf nodes_indexs.at(0b100).size(); i++) {
		
		ISolutionNode* const_depot = inf nodes.get_node(inf nodes_indexs.at(0b100).at(i), 0b100);
		auto task = const_depot->task;
		task->travel_time_ = inf cal_travel_time(inf nodes, inf nodes_indexs.at(0b100).at(i), inf nodes_indexs.at(0b100).at(i), 0b100);
		task->total_service_time_ = task->boarding_time_ + task->alighting_time_ + task->travel_time_ + task->extra_service_time_;
		task->predefined_earliest_leave_time_ = task->predefined_earliest_service_start_time_ + task->total_service_time_;
		task->service_distance_ = 0;

		const_depot->arrive_earliest_time = const_depot->task->predefined_earliest_arrival_time_;
		const_depot->service_earliest_start_time = const_depot->task->predefined_earliest_arrival_time_;
		const_depot->leave_earliest_time = task->predefined_earliest_leave_time_;

		const_depot->arrive_latest_time = const_depot->task->predefined_latest_arrival_time_;
		const_depot->service_latest_start_time = const_depot->task->predefined_service_latest_start_time_;
		const_depot->leave_latest_time = task->predefined_service_latest_start_time_ + task->total_service_time_;
	}
}
// 显示数据
void showDataInformation()
{
	cout << "The Data is loaded!" << endl;
	cout << "**********************" << endl;
	cout << "  Data Information:" << endl;
	for (auto node : inf flight2FerryVehcle)
		cout << "\t" << node.first << "\t" << node.second << endl;
	cout << "\tflights:" << inf flight_flights.size() << endl;
	cout << "\ttasks:" << inf nodes.get_size(0b001) << endl;
	ofstream outFile;
	outFile.open("D:\\学习资料\\研究进度\\研二\\大论文\\alogorithm\\Gurobi\\tasks.csv", ios::out);
	outFile << "flightnum,company,flight_type,flight_class,ferry_vehicle_num,apron,start_pos,start_index,end_pos,end_index,stand_index,ea,es,ls,serT,serDis,boardT,alightT" << endl;
	for (int i = 0; i < inf nodes.get_size(0b001); i++) {
		ISolutionNode* node = inf nodes.get_node(i);
		cout << *(node->task);
		outFile << *(node->task);
	}
	outFile.close();
	cout << "\tdepots:" << inf nodes.get_size(0b100) << endl;
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
	string flightCSV = A_S input_dir;
	csv2task(flightCSV);

	/* 生成摆渡车任务列表*/
	// 同一飞机的摆渡车任务列表
	// 摆渡车任务列表
	flight2FerryVehcleTasks();

	/*读取距离矩阵*/
	string matrixPath = "distance.txt";
	loadDisMatrix(matrixPath, inf kIndexToPositionName.size());

	// 计算每个节点的服务时间
	cal_distance();
	/* 打印数据*/
	showDataInformation();
}
