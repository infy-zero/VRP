#pragma once

#include <random>
#include <vector>

using namespace std;
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
    // 获得[0, 1]的随机数
	static double getRandom();
    // 输入得分，获得被选择的角标
    static const int roulette(vector<double> grades);
};
