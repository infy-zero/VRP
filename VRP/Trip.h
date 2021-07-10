#pragma once
#include <vector>
#include "Segment.h"

using namespace std;
class Trip
{
private:
	vector<Segment> segments;
public:
	vector<Segment> getSegments()
	{
		return segments;
	}
	vector<int> getSegmentsNum()
	{
		vector<int> segmentsNums;
		for (Segment nodes : segments)
		{
			// ��segment���л�ԭ
			auto tmp = nodes.getNodesNum();
			segmentsNums.insert(segmentsNums.end(),tmp.begin(),tmp.end());
		}
		return segmentsNums;
	}
};