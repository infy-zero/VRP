#pragma once
#include <string>
using namespace std;
enum LocationType{Depot,ParkingSpace,Terminal,RemoteBoardingGate};
// �ڵ�
class ILocation
{
private:
	double longitude;// ����
	double latitude;// γ��
	enum LocationType locationType; // ����
	int index;// ��List�е�λ��
	string name;


};

