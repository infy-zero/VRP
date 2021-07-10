#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "lib/jsoncpp/json.h"
#include <unordered_map>
using namespace std;

unordered_map<string, int> loadMap();
void loadJSON()
{
	unordered_map<string, int> map = loadMap();
	int size = map.size();
	double** result = new double* [size];
	for (int i = 0; i < size; i++)
		result[i] = new double[size];
	cout << "Start to load the data.";
	ifstream in("ODLines_FeaturesToJSON.json", ios::binary);
	if (!in.is_open())
	{
		cout << "wrong" << endl;
		exit(1);
	}
	else
		cout << "open successfully!" << endl;
	int nRole;
	// string name;
	int origin;
	int destination;
	// int dr;
	double length;
	// double sl;

	Json::Reader reader;
	Json::Value root;

	int ObjectIDMax = 0;
	if (reader.parse(in, root))
	{
		cout << "root size=" << root.size() << endl;
		cout << "features is member:" << root.isMember("features") << endl;
		Json::Value tmp0 = root["features"];
		cout << "Total size " << tmp0.size() << endl;
		for (int i = 0; i < tmp0.size(); i++)
		{
			Json::Value tmp = root["features"][i]["attributes"];
			// cout << " count= " << i << endl;
			nRole = tmp["ObjectID"].asInt();
			// name = tmp["Name"].asString();
			origin = tmp["OriginID"].asInt();
			ObjectIDMax = origin > ObjectIDMax ? origin : ObjectIDMax;
			destination = tmp["DestinationID"].asInt();
			// dr = tmp["DestinationRank"].asInt();
			length = tmp["Total_Length"].asDouble();
			// sl = tmp["Shape_Length"].asDouble();
			result[origin - 1][destination - 1] = length;
			//cout << origin - 1 <<","<< destination - 1 << "," << length << endl;
		}

	}
	in.close();
	ofstream os("distance.txt");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			os << result[i][j];
			if (j != size - 1)
				os << ",";
		}
		os << endl;
	}
	os.close();
	cout << "Finished!" << endl;
}

unordered_map<string, int> loadMap()
{
	ifstream in("Í£»úÎ»ÖÃ_FeaturesToJSON.json", ios::binary);
	if (!in.is_open())
	{
		cout << "wrong" << endl;
		exit(1);
	}
	else
		cout << "open successfully!" << endl;

	int nRole;
	string id;

	Json::Reader reader;
	Json::Value root;

	int ObjectIDMax = 0;
	unordered_map<string, int> map;
	// ofstream os("comparationTable2.txt");
	if (reader.parse(in, root))
	{
		cout << "root size=" << root.size() << endl;
		cout << "features is member:" << root.isMember("features") << endl;
		Json::Value tmp0 = root["features"];
		cout << "Total size " << tmp0.size() << endl;
		for (int i = 0; i < tmp0.size(); i++)
		{
			Json::Value tmp = root["features"][i]["attributes"];
			nRole = tmp["OBJECTID"].asInt();
			id = tmp["ID"].asString();
			//cout << " count= " << i << ",objectID=" << nRole << ",id=" << id << endl;
			map.insert(pair<string, int>(id, nRole - 1));
			// os << nRole << "," << id << endl;
		}
	}
	in.close();
	return map;

}