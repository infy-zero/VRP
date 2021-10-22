#pragma once

#include <random>
#include <vector>

using namespace std;
// ö����
enum VehicleType { LARGE }; // ��������

// struct
struct EnumClassHash //ö�����ϣ��
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

struct NodeLocation
{
    int vehicle_index;
    int cur_pos;
    int max_pos;
};

//class

static class Util
{
public:
    // ���[0, 1]�������
	static double getRandom();
    // ����÷֣���ñ�ѡ��ĽǱ�
    static const int roulette(vector<double> grades);
};
