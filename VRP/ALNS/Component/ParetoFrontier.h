#pragma once
#include <list>
#include <vector>
#include <string>
#include <assert.h>
#include "ISolution.h"
using namespace std;
class ParetoFrontier
{
private:
	list<pair<double,string>> paretoFrontier; // ÿ�������н���vector��string���ɣ�ǰ����Ŀ�꺯��ֵ�������ǽ�ı��ʽ
public:
	bool insert(ISolution& solution);
};

