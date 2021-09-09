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
#include "ALNS_Setting.h"
#include "IInformation.h"
#include "ALNS/BasicClass/ISolutionNode.h"

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
		// ����������վ
		if (name._Equal("BGS"))
		{
			shared_ptr<FerryVehicleTask> task_bgs(new FerryVehicleTask(index, NULL, INT_MIN, INT_MAX, 0, DP));
			ISolutionNode node_bgs(task_bgs);
			inf nodes.push_depot(-1, node_bgs);

		}
		else if (name._Equal("CA"))
		{
			shared_ptr<FerryVehicleTask> task_ca(new FerryVehicleTask(index, NULL, INT_MIN, INT_MAX, 0, DP));
			ISolutionNode node_ca(task_ca);
			inf nodes.push_depot(-2, node_ca);
		}
	}
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
	int relaxingTime = FerryTaskSetting::relaxingTime;
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
		if (A_S range == EAST && terminal.find("T3") == -1)
			continue;
		else if (A_S range == WEST && terminal.find("T3") != -1)
			continue;
		rdy -= startTime - relaxingTime;
		enum Direction direction = Trim(fields[8])._Equal("arrive") ? arrive : depart;
		Flight* tmp = new Flight(id, flightcompany, flighttype, flightclass, ferryVehicles, apron, stand,
			inf flightName2index.at(stand), static_cast<int>(rdy), direction, terminal, inf flightName2index.at(terminal));
		inf flightTasks.push_back(tmp);
	}

	cout << "Successful to load the flight tasks!" << endl;
}
// ������ת��Ϊ�ڶɳ�����
void flight2FerryVehcleTasks()
{
	int num = 0;
	// transfer flightTasks to ferryVehicleTasks
	for (int j = 0; j < inf flightTasks.size(); j++)
	{
		auto flightTask = inf flightTasks.at(j);
		vector<int> con;
		for (int i = 0; i < flightTask->ferryVehicles; i++)
		{
			// ����˳���¼��Ӧ�ڵ��λ�� pair<��ţ��������>?g
			inf positions.push_back(make_pair(j, i));
			con.push_back(num);
			int serviceStartTime;
			int serviceEndTime;
			// ���翪ʼ����ʱ��
			if (i == 0)// ��һ����������ǰ5+timewindow(3)=8���ӵ���
				serviceStartTime = flightTask->getServiceStartTime() - FerryTaskSetting::firstBefore;
			else if (i == 1) // �ڶ�����������ǰ
				serviceStartTime = flightTask->getServiceStartTime();
			else // ʣ�³������շ���ʱ������
				serviceStartTime = flightTask->getServiceStartTime() + FerryTaskSetting::waitingTime + i * FerryTaskSetting::boardingTime;
			// ����ʼ����ʱ��
			if (i == 0)// ��һ����������ǰ����ӵ���
				serviceEndTime = flightTask->getServiceStartTime() - FerryTaskSetting::firstBefore;
			else// ʣ�³������շ���ʱ������
				serviceEndTime = flightTask->getServiceStartTime() + i * (FerryTaskSetting::boardingTime + FerryTaskSetting::maxDeltaTime);

			enum FVTType type; // �ڵ����ͣ�ǰ����ʱ�䴰���ɱ�UVF������ڵ㶯̬ʱ�䴰�ɱ�VVF
			if (i <= 2)
				type = UVF;
			else
				type = VVF;
			shared_ptr<FerryVehicleTask>
				tmp_task(new FerryVehicleTask(num++,
					flightTask,
					serviceStartTime,
					serviceEndTime,
					fts boardingTime,
					type));
			ISolutionNode tmp_node(tmp_task);
			inf nodes.push_node(tmp_node.task->id, tmp_node);
		}
		inf consequence.push_back(con);

	}
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
// ��ʾ����
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
	string flightCSV = "finalTasks80.csv";
	csv2task(flightCSV);

	/* ���ɰڶɳ������б�*/
	// ͬһ�ɻ��İڶɳ������б�
	// �ڶɳ������б�
	flight2FerryVehcleTasks();

	/*��ȡ�������*/
	string matrixPath = "distance.txt";
	loadDisMatrix(matrixPath, inf index2flightName.size());

	/* ��ӡ����*/
	showDataInformation();
}
