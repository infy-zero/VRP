#pragma once
#include <string>
using namespace std;

// typedef shared_ptr<ISolutionNode> spn;	// node �Ĺ���ָ��
// typedef shared_ptr<IVehicle> spv;		// vehicle �Ĺ���ָ��

class RepairOperatorInformation
{
public:
	double weight;
	double grade;
	enum RepairOperatorNum type;
	RepairOperatorInformation(double _weight, double _grade, enum RepairOperatorNum _type) :weight(_weight), grade(_grade), type(_type) {}
	static RepairOperatorInformation build(double _weight, double _grade, enum RepairOperatorNum _type)
	{
		RepairOperatorInformation result(_weight, _grade, _type);
		return result;
	}
};
class DestroyOperatorInformation
{
public:
	double weight;
	double grade;
	enum DestroyOperatorNum type;
	DestroyOperatorInformation(double _weight, double _grade, enum DestroyOperatorNum _type) :weight(_weight), grade(_grade), type(_type) {}
	static DestroyOperatorInformation build(double _weight, double _grade, enum DestroyOperatorNum _type)
	{
		DestroyOperatorInformation result(_weight, _grade, _type);
		return result;
	}
};
enum SolutionFrequency { BEST, ALREADY, NEVER };
enum OperatorType { DESTROY, REPAIR };
enum ScopeRange { WEST, EAST, MIX };
enum RepairOperatorNum { GREEDYREPAIR };
enum DestroyOperatorNum { GREEDYDESTROY };
#define A_S ALNS_Setting::
static class ALNS_Setting
{
public:
	// Ŀ�꺯������
	static constexpr double vcost = 2000; // �����̶��ɱ�
	static constexpr double pcost = 2; // ������ʻ��λ����ɱ�
	

	// ��Ӧ�����
	static constexpr double r = 0.2; // ��������
	static constexpr int segment = 3; // ����Ƶ��
	static constexpr double initWeight = 1.0; // ��ʼȨ��
	static constexpr double best = 1.2;
	static constexpr double already = 0.8;
	static constexpr double never = 1.0;
	static constexpr double ratio = 0.1;
	
	// ��������
	static constexpr double startT = 100;
	static constexpr double endT = 10;
	static constexpr double decay = 0.6;
	static constexpr int decayTimes = 1;
	// static constexpr int additionalTimes = 5;

	// Լ������
	static constexpr double maxLength = 200000;     // ������
	static constexpr double speed = 30 / 3.6;       // ƽ����ʻ�ٶ�
	static string to_string(enum OperatorType ot);

	// ��������
	static constexpr enum ScopeRange range	= EAST; // ��Χ-����/����/���
	static constexpr int startIndex			= 0;	// ��ʼ����
	static constexpr int counts				= 31;	// ��ֹ����
};