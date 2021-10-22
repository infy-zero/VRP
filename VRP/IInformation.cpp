#include "IInformation.h"

AllNodes IInformation::nodes(0);
vector<vector<int>> IInformation::nodes_indexs(5);
vector<vector<double>> IInformation::matrix;
unordered_map<char, int> IInformation::flight2FerryVehcle;
vector<string> IInformation::kIndexToPositionName;
unordered_map<string, int> IInformation::kPositionNameToIndex;
vector<Flight*> IInformation::flight_flights;
vector<pair<int, int>> IInformation::positions;

double IInformation::cal_travel_time(AllNodes& nodes, int from_node_index, int to_node_index, int type, double fromTime)
{
	int start_position = nodes.get_node(from_node_index, type)->task->end_index_;
	int end_position = nodes.get_node(to_node_index, type)->task->from_index_;
	return matrix.at(start_position).at(end_position) / A_S speed;
}

/*计算从from到to的旅行距离*/
double IInformation::cal_travel_distance(AllNodes& nodes, int from_node_index, int to_node_index, int type, double fromTime)
{
	int start_position = nodes.get_node(from_node_index, type)->task->end_index_;
	int end_position = nodes.get_node(to_node_index, type)->task->from_index_;
	return matrix.at(start_position).at(end_position);
}
