#pragma once
#include <vector>
#include <random>

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
	static double getRandom();
};
