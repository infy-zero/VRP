#pragma once

#include <unordered_map>

#include "ALNS/BasicClass/ISolutionNode.h"
using namespace std;
/*
* type说明，为固定场站+场站+普通节点 1+1+1=0b111
* 
* 
*/
class AllNodes
{
public:
	AllNodes() = delete;
	AllNodes(int code);
	AllNodes(const AllNodes& other);
	~AllNodes();

	/*设置节点状态*/
	// 所有节点设为未知
	void set_state_all_unknown();
	// 所有node节点设置为未检查
	void set_nodes_unchecked();
	// 所有const_depot节点设置为未检查
	void set_const_depot_unchecked();
	// 所有depot节点设置为未检查
	void set_depot_unchecked();

	void remove_node(int node_id);
	void remove_const_depot(int const_depot_id);
	void remove_depot(int depot_id);
	int push_node(ISolutionNode* node);
	int push_node(int depot_id, ISolutionNode* node);
	int push_const_depot(ISolutionNode* const_depot);
	int push_const_depot(int depot_id, ISolutionNode* const_depot);
	int push_depot(ISolutionNode* depot);
	int push_depot(int depot_id, ISolutionNode* depot);

	ISolutionNode* get_node(int id, int type = 0b011);

	// 清除场站节点
	int count(int id);

	// 全局函数
	enum NodeType get_node_type(int node_index);
	// 获得大小
	const int get_size(int type);
	// 获得任务
	const FerryVehicleTask get_task(int task_index);
	// 插入任务
	void push_back_flight_tasks(vector<int>& tasks);
	

private:
	unordered_map<int, ISolutionNode*>				nodes_; // 普通节点
	unordered_map<int, ISolutionNode*>				const_depots_; // 初始场站
	unordered_map<int, ISolutionNode*>				depots_; // 临时场站节点
	vector<vector<int>>*							flight_tasks_; // 摆渡车任务列表
	
	int												node_num_unused  = 1;
	int												depot_num_unused = -3; // 因为固定场站有两个，所以此处从-3开始，避免冲突。
	vector<int>										node_unused;
	vector<int>										depot_unused;
};

