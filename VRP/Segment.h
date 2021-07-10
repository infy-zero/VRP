#pragma once
#include <vector>
#include "Node.h"

using namespace std;
class Segment
{
private:
	vector<Node> nodes;
public:
	vector<Node> getNodes()
	{
		return nodes;
	}
	vector<int> getNodesNum()
	{
		vector<int> nodeNums;
		for (Node node : nodes)
		{
			nodeNums.push_back(node.getNodeNum());
		}
		return nodeNums;
	}
};