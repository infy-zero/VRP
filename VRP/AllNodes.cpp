#include "AllNodes.h"

#include "MyException.h"

int AllNodes::get_max_node_num() {
	return node_max_num;
}

void AllNodes::remove_node(int node_id) {
	node_max_num--;
	auto iter = nodes_.find(node_id);
	// 如果没有找到该点，直接报错
	if (iter == nodes_.end()) {
		throw MyException("The node does not exists.");
	} else { // 找到则进行删除操作
		node_unused.push_back(node_id);
		nodes_.erase(iter);
	}
}

void AllNodes::remove_depot(int depot_id) {
	auto iter = depots_.find(depot_id);
	// 如果没有找到该点，直接报错
	if (iter == depots_.end()) {
		throw MyException("The depot does not exists.");
	}
	else { // 找到则进行删除操作
		depot_unused.push_back(depot_id);
		depots_.erase(iter);
	}
}

int AllNodes::push_node(ISolutionNode& node) {
	node_max_num++;
	// unused为空，则使用cur_num_unused
	if (node_unused.empty()) {
		nodes_.insert(make_pair(node_num_unused, node));
		return node_num_unused++;
	} else { // 使用并删除最后一个序号
		int num = node_unused.at(node_unused.size()-1);
		node_unused.pop_back();
		nodes_.insert(make_pair(num, node));
		return num;
	}
}

int AllNodes::push_node(int node_id, ISolutionNode& node) {
	if (nodes_.count(node_id)) {
		throw MyException("You can not insert depot already exists.");
	}
	nodes_.insert(make_pair(node_id, node));
	return node_id;

}

int AllNodes::push_depot(ISolutionNode& depot) {
	// depot_unused为空，则使用depot_num_unused
	if (depot_unused.empty()) {
		nodes_.insert(make_pair(depot_num_unused, depot));
		return depot_num_unused--;
	}
	else { // 使用并删除最后一个序号
		int num = depot_unused.at(depot_unused.size() - 1);
		depot_unused.pop_back();
		depots_.insert(make_pair(num, depot));
		return num;
	}
}

int AllNodes::push_depot(int depot_id, ISolutionNode& depot) {
	if (depots_.count(depot_id)) {
		throw MyException("You can not insert depot already exists.");
	}
	depots_.insert(make_pair(depot_id, depot));
	return depot_id;
}

ISolutionNode* AllNodes::get(int id)
{
	auto result_node = nodes_.find(id);
	if (result_node != nodes_.end()) {
		return &(result_node->second);
	}
	auto result_depot = depots_.find(id);
	if (result_depot != depots_.end()) {
		return &(result_depot->second);
	}
	return nullptr;
}
int AllNodes::count(int id) {
	int result = nodes_.count(id);
	if (result > 0) {
		return result;
	} else {
		return depots_.count(id);
	}
}