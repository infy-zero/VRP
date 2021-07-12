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
// ����Ϊ��������
// ���������վ��Զ��λ����վ¥����ʱͣ��λ�þ���

// ���뺽����Ҫ�İڶɳ�������
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
// ���뺽������
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
// ������ת��Ϊ�ڶɳ�����
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
			// ���翪ʼ����ʱ��
			if (i == 0)// ��һ����������ǰ8���ӵ���
				serviceStartTime = flightTask.getServiceStartTime() - fts.firstBefore - fts.timeWindow;
			else// ʣ�³������շ���ʱ������
				serviceStartTime = flightTask.getServiceStartTime() + fts.waitingTime + i * fts.boardingTime - fts.timeWindow;
			// ����ʼ����ʱ��
			if (i == 0)// ��һ����������ǰ����ӵ���
				serviceEndTime = flightTask.getServiceStartTime() - fts.firstBefore;
			else// ʣ�³������շ���ʱ������
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
// ����������
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
// ��ʾ����
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
	/* ��ȡ������Ҫ�İڶɳ������б�*/
	string flight2ferry = "flight2ferry.csv";
	// ������Ҫ�İڶɳ������б�
	readF2FV(flight2ferry, flight2FerryVehcle);

	/* ��ȡͣ��λ���ձ�*/
	string flightindex = "comparationTable2.txt";
	// ��ŵ�ͣ��λ�����б�
	// ͣ��λ����������б�
	LoadComparationTable(flightindex, index2flightName, flightName2index);

	/* ��ȡ�ɻ��б�*/
	// �ɻ��б�
	string flightCSV = "finalTasks80.csv";
	csv2task(flightCSV,flight2FerryVehcle, flightTasks, flightName2index);

	/* ���ɰڶɳ������б�*/
	// ͬһ�ɻ��İڶɳ������б�
	// �ڶɳ������б�
	flight2FerryVehcleTasks(ferryVehicleTasks, flightTasks, consequence);

	/*��ȡ�������*/
	string matrixPath = "distance.txt";
	loadDisMatrix(matrixPath, matrix, index2flightName->size());


	/* ��ӡ����*/
	// showDataInformation(flight2FerryVehcle,flightTasks, ferryVehicleTasks);
}