#pragma once
#include <iostream>
#include <string>

using namespace std;
class Parallel
{
private:
	int threads = 1;
public:
	int getThreads()
	{
		return threads;
	}
	string toString()
	{
		return "\tTotal Threads\t\t= " + to_string(threads) + "\n";
	}
};

