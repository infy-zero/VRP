#pragma once
#include <string>
using namespace std;
enum LocationType{Depot,ParkingSpace,Terminal,RemoteBoardingGate};
// 节点
class ILocation
{
private:
	double longitude;// 经度
	double latitude;// 纬度
	enum LocationType locationType; // 类型
	int index;// 在List中的位置
	string name;


};

