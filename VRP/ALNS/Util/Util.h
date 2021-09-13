#pragma once
#include <vector>
#include <random>

// 枚举类

enum VehicleType { LARGE }; // 车辆类型

// struct
struct EnumClassHash //枚举类哈希化
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
