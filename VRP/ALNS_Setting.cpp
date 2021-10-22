#include "ALNS_Setting.h"

string ALNS_Setting::output_dir = "D:\\ѧϰ����\\�о�����\\�ж�\\������\\����\\���ݽ��\\";
string ALNS_Setting::input_dir = "C:\\Users\\ln\\source\\repos\\VRP\\VRP\\resource\\finalTasks1000.csv";
unordered_map<char, int> ALNS_Setting::flight_type_to_count = {{'B',2},{'C',3},{'D',4},{'E',5},{'F',6},};

std::string ALNS_Setting::to_string(enum OperatorType ot)
{
	if (ot == REPAIR)
		return "repair";
	if (ot == DESTROY)
		return "destroy";
	return "NULL";
}

double ALNS_Setting::get_vehicle_cost(enum VehicleType vehicle_type_) {
	switch (vehicle_type_) {
	case LARGE:
		return vcost;
	default:
		throw MyException("No vehicle type found!");
		return INT_MAX;
	}
}
// ��ó�����ʻ�ɱ�
double ALNS_Setting::get_vehicle_per_cost(enum VehicleType vehicle_type_) {
	if (vehicle_type_ == LARGE) {
		return pcost;
	}
	throw MyException("No vehicle type found!");
	return -DBL_MAX;
}

// �����ʻ����
double ALNS_Setting::get_vehicle_max_length(enum VehicleType vehicle_type) {
	switch (vehicle_type) {
	case LARGE:
		return maxLength; // �����ʻ���200km
	default:
		throw MyException("No vehicle type found!");
		return INT_MAX;
	}
}
// �����ʻʱ��
double ALNS_Setting::get_vehicle_max_time(enum VehicleType vehicle_type) {
	switch (vehicle_type) {
	case LARGE:
		return maxTime; 
	default:
		throw MyException("No vehicle type found!");
		return INT_MAX;
	}
}
// ��õ÷�
double ALNS_Setting::get_grade_addition(enum SolutionFrequency sf) {
	double grade_add;
	switch (sf) {
	case BEST:
		grade_add = best;
		break;
	case NEVER_BUT_BETTER:
		grade_add = never;
		break;
	case SA_ACCEPT:
		grade_add = accept;
		break;
	case SA_REJECT:
		grade_add = reject;
		break;
	default:
		throw MyException("Unknown type");
	}
	return grade_add;
}
// ��õ÷�����
string ALNS_Setting::SolutionFrequency_to_string(enum SolutionFrequency sf) {
	string sf_string;
	switch (sf) {
	case BEST:
		sf_string = "BEST";
		break;
	case NEVER_BUT_BETTER:
		sf_string = "NEVER";
		break;
	case SA_ACCEPT:
		sf_string = "ACCEPT";
		break;
	case SA_REJECT:
		sf_string = "REJECT";
		break;
	default:
		throw MyException("Unknown type");
	}
	return sf_string;
}
