#pragma once

#include <unordered_map>

#include "ALNS/BasicClass/ISolutionNode.h"
using namespace std;

class AllNodes
{
public:
	void remove_node(int node_id);
	void remove_depot(int depot_id);
	int push_node(ISolutionNode& node);
	int push_node(int depot_id, ISolutionNode& node);
	int push_depot(ISolutionNode& depot);
	int push_depot(int depot_id, ISolutionNode& depot);
	ISolutionNode* get(int id);
	int count(int id);

	unordered_map<int, ISolutionNode>				nodes_;
	unordered_map<int, ISolutionNode>				depots_;
private:	
	int												node_num_unused  = 1;
	int												depot_num_unused = -1;
	vector<int>										node_unused;
	vector<int>										depot_unused;
};

