#include "Util.h"

#include <random>
#include <vector>

#include "MyException.h"

double Util::getRandom()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0, 1.0);
	return dis(gen);
}

// TODO(Lvning): À©³ä³ÉÄ£°åº¯Êý
const int Util::roulette(vector<double> grades) {
	double total_grade = 0.0;
	if (grades.size() == 0) {
		throw MyException("Can't solve with zero values.");
	}
	if (grades.size() == 1) {
		return 0;
	}
	for (double grade : grades) {
		total_grade += grade;
	}
	double selected_grade = total_grade * getRandom();
	for (int i = 0; i < grades.size(); i++) {
		if (selected_grade < grades.at(i)) {
			return i;
		}
		selected_grade -= grades.at(i);
	}
	return grades.size() - 1;
}
