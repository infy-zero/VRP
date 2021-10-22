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
// ����Ϊ��������
// ���������վ��Զ��λ����վ¥����ʱͣ��λ�þ���

// ���뺽����Ҫ�İڶɳ�������
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
		// ����������վ TODO(Lvning): ��������task�Ĳ�������Ȼû��Ӱ��
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
// ���뺽������
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
		// �жϳ�ȡ��/��/����������Ŀǰ��ȡ��������
		if (A_S range == EAST && terminal.find("T3") == -1) {
			continue;
		}
		else if (A_S range == WEST && terminal.find("T3") != -1)
		{
			continue;
		}
		// �����������������
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
// ������ת��Ϊ�ڶɳ�����
void flight2FerryVehcleTasks()
{
	int num = 0;
	// transfer flight_flights to ferryVehicleTasks
	vector<int> node_indexs;
	for (int j = 0; j < inf flight_flights.size(); j++)
	{
		auto flight = inf flight_flights.at(j);
		vector<int> con;
		for (int i = 0; i < flight->ferryVehicles && i< A_S max_count; i++) // ��֤���ڵ���
		{
			// ����˳���¼��Ӧ�ڵ��λ�� pair<��ţ��������>?g
			inf positions.push_back(make_pair(j, i));
			con.push_back(num);
			double service_start_time;	// ����ʼʱ��
			double service_end_time;	// �������ʱ��
			double boarding_time;  // �ϳ�ʱ��
			double alighting_time;  // �³�ʱ��
			// ����ʱ��
			if (flight->direction == DIRECTION_ARRIVE || flight->direction == DIRECTION_DEPART) {
				boarding_time = fts FTS_boardingTime;
				alighting_time = fts FTS_alightingTime;
			}
			else {
				throw MyException("Direction type is wrong.");
			}
			// ����ڵ�ʱ�䴰����
			enum FVTType type = FVT_UNKNOWN; // �ڵ����ͣ�ǰ����ʱ�䴰���ɱ�UVF������ڵ㶯̬ʱ�䴰�ɱ�VVF
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
			// ����������ʱ�䴰��
			if ( i == 0 ) {
				// ����ʱ��
				service_start_time = flight->serviceStartTime - fts FTS_firstBefore;// ��һ����������ǰ5+timewindow(3)=8���ӵ���
				service_end_time = flight->serviceStartTime - fts FTS_firstBefore;// ��һ����������ǰ����ӵ���
				extra_service_time = fts FTS_firstBefore; // ��һ�������5���ӷ���ʱ��
			} else {
				// ����ʱ��
				service_start_time = flight->serviceStartTime;// ʣ�³���0ʱ�̿�ʼ����
				service_end_time = flight->serviceStartTime + fts FTS_max_time_window;// 5������ȫ������
				extra_service_time = 0;
			}
			enum Direction direction = flight->direction;
			int from_index, end_index;
			// ����ʱ�䡢��������ص�ͷ����йأ����ﺽ������Զ��λ->��վ¥���뿪���ࣺ��վ¥->Զ��λ
			if (direction == DIRECTION_DEPART) {
				from_index = flight->terminal_index;
				end_index = flight->stand_index;
			} else if (direction == DIRECTION_ARRIVE) {
				from_index = flight->stand_index; 
				end_index = flight->terminal_index;
			} else{
				throw MyException("Wrong direction.");
			}
			// ���ڶɳ�����ѹ��flight_task
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
// ����������
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
// ��������ڵ������Ϣ
void cal_distance() {
	// ���½ڵ���Ϣ
	for (int i = 0; i < inf nodes_indexs.at(0b001).size(); i++) {
		ISolutionNode* node = inf nodes.get_node(inf nodes_indexs.at(0b001).at(i));
		FerryVehicleTask* task = node->task;
		task->travel_time_ = inf cal_travel_time(inf nodes, inf nodes_indexs.at(0b001).at(i), inf nodes_indexs.at(0b001).at(i), 0b001);
		task->total_service_time_ = task->boarding_time_ + task->alighting_time_ + task->travel_time_ + task->extra_service_time_;
		task->service_distance_ = inf cal_travel_distance(inf nodes, inf nodes_indexs.at(0b001).at(i), inf nodes_indexs.at(0b001).at(i), 0b001);
	}
	// ���¹̶���վ��Ϣ
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
// ��ʾ����
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
	outFile.open("D:\\ѧϰ����\\�о�����\\�ж�\\������\\alogorithm\\Gurobi\\tasks.csv", ios::out);
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
	/* ��ȡ������Ҫ�İڶɳ������б�*/
	string flight2ferry = "flight2ferry.csv";
	// ������Ҫ�İڶɳ������б�
	readF2FV(flight2ferry);

	/* ��ȡͣ��λ���ձ�*/
	string flightindex = "comparationTable2.txt";
	// ��ŵ�ͣ��λ�����б�
	// ͣ��λ����������б�
	LoadComparationTable(flightindex);

	/* ��ȡ�ɻ��б�*/
	// �ɻ��б�
	string flightCSV = A_S input_dir;
	csv2task(flightCSV);

	/* ���ɰڶɳ������б�*/
	// ͬһ�ɻ��İڶɳ������б�
	// �ڶɳ������б�
	flight2FerryVehcleTasks();

	/*��ȡ�������*/
	string matrixPath = "distance.txt";
	loadDisMatrix(matrixPath, inf kIndexToPositionName.size());

	// ����ÿ���ڵ�ķ���ʱ��
	cal_distance();
	/* ��ӡ����*/
	showDataInformation();
}
