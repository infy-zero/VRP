#pragma once
#include <list>

#include "ISolutionNode.h"
using namespace std;
class IVehicle
{
private:
	bool update = true;	// �ж��ڽ���ɾ���������Ƿ���¡�
	list<ISolutionNode*> vehicles;	// �ڵ��б�
	/* �洢����λ�� */
	list<ISolutionNode*>::iterator firstBestLoc; // ��õ�λ��
	list<ISolutionNode*>::iterator secondBestLoc; // �ڶ��õ�λ��

public:
	bool isUpdate();
	list<ISolutionNode*>::iterator& getFirstBestLoc();
	list<ISolutionNode*>::iterator& getSecondBestLoc();
	void clearBestLoc();
	double getLength();

};