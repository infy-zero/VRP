#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>
#include <unordered_map>
#include "Flight.h"
#include "FerryTaskSetting.h"
#include "FerryVehicleTask.h"
/**
 Date:    2021-06-25 20:20:00
 Author:  Lv Ning
 version: 0.1
 Description: This cpp file is used to load the Data.
*/
using namespace std;

void loadData(unordered_map<char, int>* flight2FerryVehcle, vector<string>* index2flightName, unordered_map<string, int>* flightName2index, vector<Flight>* flightTasks, vector<FerryVehicleTask>* ferryVehicleTasks, vector<vector<int>>* consequence, vector<vector<double>>* matrix);
string Trim(string& str);
time_t strTime2unix(string timeStamp);
void readF2FV(string filepath, unordered_map<char, int>* map);
void LoadComparationTable(string filename, vector<string>* index2flight, unordered_map<string, int>* flight2index);
void csv2task(string filename, unordered_map<char, int>& map, vector<Flight>*  flightTasks, unordered_map<string, int>* flight2index);
void flight2FerryVehcleTasks(vector<FerryVehicleTask>* ferryVehicleTasks, vector<Flight>* flightTasks, vector<vector<int>>* consequence);
void loadDisMatrix(string matrixPath, vector<vector<double>>* matrix, int size);
void showDataInformation(unordered_map<char, int>& map, vector<Flight>& flightTasks, vector<FerryVehicleTask>& ferryVehicleTasks);
// 以下为函数区域
// 读入各个场站、远机位、航站楼、临时停车位置距离

// 读入航班需要的摆渡车数量表
void readF2FV(string filepath, unordered_map<char, int>* map)
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
		char type= Trim(fields[0]).c_str()[0];
		int needVehicle = atoi(Trim(fields[1]).c_str());
		map->insert(pair<char, int>(type, needVehicle));
	}
	cout << "Successful to load the flight-ferry vehicle number table." << endl;
}
void LoadComparationTable(string filename, vector<string>* index2flight, unordered_map<string, int>* flight2index)
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
		int index = atoi(Trim(fields[0]).c_str())-1;
		string name = Trim(fields[1]);
		index2flight->push_back(name);
		flight2index->insert(pair<string,int> (name,index));
	}
	cout << "Successful to read the comparation table!" << endl;
}
// 读入航班数据
void csv2task(string filename, unordered_map<char, int>* map, vector<Flight>* flightTasks, unordered_map<string, int>* flight2index)
{
	ifstream fin(filename, ios::in);
	if (!fin.is_open())
		throw  "Fail to open the file:"+filename;
	string line;
	int count = -2;
	FerryTaskSetting fts;
	int relaxingTime = fts.relaxingTime;
	time_t startTime;
	while (getline(fin, line))
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
		int ferryVehicles = map->at(flightclass);
		string apron = Trim(fields[5]);
		string stand = Trim(fields[6]);
		time_t rdy= strTime2unix(Trim(fields[7])+":00");
		string terminal = Trim(fields[10]);
		if (count == 0)
		{
			startTime = rdy;
			//cout << "startTime=" << startTime << endl;
		}

		rdy -= startTime - relaxingTime;
		enum Direction direction = Trim(fields[8])._Equal("arrive")?arrive:depart;
		Flight tmp(id, flightcompany, flighttype, flightclass, ferryVehicles, apron, stand,
			flight2index->at(stand),static_cast<int>(rdy), direction, terminal, flight2index->at(terminal));
		flightTasks->push_back(tmp);
	}
	
	cout << "Successful to load the flight tasks!" << endl;
}
// 将航班转换为摆渡车任务
void flight2FerryVehcleTasks(vector<FerryVehicleTask>* ferryVehicleTasks, vector<Flight>* flightTasks, vector<vector<int>>* consequence)
{
	int num = 0;
	FerryTaskSetting fts;
	// transfer flightTasks to ferryVehicleTasks
	for (auto flightTask : *flightTasks)
	{
		vector<int> con;
		for (int i = 0; i < flightTask.getFerryVehicles(); i++)
		{
			con.push_back(num);
			int serviceStartTime;
			int serviceEndTime;
			int boardingTime;
			// 最早开始服务时间
			if (i == 0)// 第一辆车最早提前8分钟到达
				serviceStartTime = flightTask.getServiceStartTime() - fts.firstBefore - fts.timeWindow;
			else// 剩下车辆按照服务时间增加
				serviceStartTime = flightTask.getServiceStartTime() + fts.waitingTime + i * fts.boardingTime - fts.timeWindow;
			// 最晚开始服务时间
			if (i == 0)// 第一辆车最晚提前五分钟到达
				serviceEndTime = flightTask.getServiceStartTime() - fts.firstBefore;
			else// 剩下车辆按照服务时间增加
				serviceEndTime = flightTask.getServiceStartTime() + i * (fts.boardingTime + fts.maxDeltaTime);
			boardingTime = fts.boardingTime;
			FerryVehicleTask
				tmp(num++,
					flightTask,
					serviceStartTime,
					serviceEndTime,
					boardingTime);
			ferryVehicleTasks->push_back(tmp);
		}
		consequence->push_back(con);
	}
	cout << "Ferry vehicles tasks are loaded successfully!" << endl;
}
// 读入距离矩阵
void loadDisMatrix(string filePath, vector<vector<double>>* matrix, int size)
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
		matrix->push_back(segments);
	}
	cout << "Distance matrix is load successfully!" << endl;
}
// 显示数据
void showDataInformation(unordered_map<char, int>& map, vector<Flight>& flightTasks, vector<FerryVehicleTask>& ferryVehicleTasks)
{
	cout << "The Data is loaded!" << endl;
	cout << "**********************" << endl;
	cout << "  Data Information:" << endl;
	for (auto node : map)
		cout << "\t" << node.first << "\t" << node.second << endl;
	cout << "\tflights:" << flightTasks.size() << endl;
	cout << "\ttasks:" << ferryVehicleTasks.size() << endl;
	for (auto ferryVehicleTask : ferryVehicleTasks)
		cout << ferryVehicleTask;
	cout << "**********************" << endl;
}

// Load all the data needed
void loadData(unordered_map<char, int>* flight2FerryVehcle, vector<string>* index2flightName, unordered_map<string, int>* flightName2index, vector<Flight>* flightTasks, vector<FerryVehicleTask>* ferryVehicleTasks, vector<vector<int>>* consequence, vector<vector<double>>* matrix)
{
	/* 读取航班需要的摆渡车数量列表*/
	string flight2ferry = "flight2ferry.csv";
	// 航班需要的摆渡车数量列表
	readF2FV(flight2ferry, flight2FerryVehcle);

	/* 读取停机位对照表*/
	string flightindex = "comparationTable2.txt";
	// 序号到停机位名称列表
	// 停机位到序号名称列表
	LoadComparationTable(flightindex, index2flightName, flightName2index);

	/* 读取飞机列表*/
	// 飞机列表
	string flightCSV = "finalTasks80.csv";
	csv2task(flightCSV,flight2FerryVehcle, flightTasks, flightName2index);

	/* 生成摆渡车任务列表*/
	// 同一飞机的摆渡车任务列表
	// 摆渡车任务列表
	flight2FerryVehcleTasks(ferryVehicleTasks, flightTasks, consequence);

	/*读取距离矩阵*/
	string matrixPath = "distance.txt";
	loadDisMatrix(matrixPath, matrix, index2flightName->size());


	/* 打印数据*/
	// showDataInformation(flight2FerryVehcle,flightTasks, ferryVehicleTasks);
}