#include "AllNodes.h"

#include "MyException.h"

AllNodes::AllNodes(int code) {
	if (code != 0) {
		throw MyException("Wrong code.");
	}
}

// 需要单独拷贝nodes和depots
AllNodes::AllNodes(const AllNodes& other):const_depots_(other.const_depots_),flight_tasks_(other.flight_tasks_),node_num_unused(other.node_num_unused), depot_num_unused(other.depot_num_unused), node_unused(other.node_unused), depot_unused(other.depot_unused){
	nodes_.clear();
	depots_.clear();
	for (auto& node_pair : other.nodes_) {
		ISolutionNode* node = new ISolutionNode(*node_pair.second);
		nodes_.emplace(node_pair.first, node);
	}
	for (auto& depot_pair : other.depots_) {
		ISolutionNode* depot = new ISolutionNode(*depot_pair.second);
		depots_.emplace(depot_pair.first, depot);
	}
}
// 需要单独析构nodes和depots
AllNodes::~AllNodes() {
	for (auto& node_pair : nodes_) {
		ISolutionNode* node = node_pair.second;
		delete node;
	}
	for (auto& depot_pair : depots_) {
		ISolutionNode* depot = depot_pair.second;
		delete depot;
	}
}


// 将所有节点设置为unknown	
void AllNodes::set_state_all_unknown() {
	for (auto& node_iter : nodes_) {
		node_iter.second->state = UNKNOWN;
	}
	for (auto& depot_iter : const_depots_) {
		depot_iter.second->state = UNKNOWN;
	}
}

// 将所有node节点设置为unchecked
void AllNodes::set_nodes_unchecked() {
	for (auto& node_iter : nodes_) {
		node_iter.second->state = UNCHECKED_NODE;
	}
}

// 将所有const_depot节点设置为unchecked
void AllNodes::set_const_depot_unchecked() {
	for (auto& node_iter : const_depots_) {
		node_iter.second->state = UNCHECKED_DEPOT;
	}
}
// 将所有depot节点设置为unchecked
void AllNodes::set_depot_unchecked() {
	for (auto& depot_iter : depots_) {
		depot_iter.second->state = UNCHECKED_DEPOT;
	}
}
void AllNodes::remove_node(int node_id) {
	auto iter = nodes_.find(node_id);
	// 如果没有找到该点，直接报错
	if (iter == nodes_.end()) {
		throw MyException("The node does not exists.");
	} else { // 找到则进行删除操作
		node_unused.emplace_back(node_id);
		delete iter->second;
		nodes_.erase(iter);
	}
}

void AllNodes::remove_const_depot(int depot_id) {
	auto iter = const_depots_.find(depot_id);
	// 如果没有找到该点，直接报错
	if (iter == const_depots_.end()) {
		throw MyException("The const_depot does not exists.");
	}
	else { // 找到则进行删除操作
		depot_unused.emplace_back(depot_id);
		delete iter->second;
		const_depots_.erase(iter);
	}
}

void AllNodes::remove_depot(int depot_id) {
	auto iter = depots_.find(depot_id);
	// 如果没有找到该点，直接报错
	if (iter == depots_.end()) {
		throw MyException("The depot does not exists.");
	}
	else { // 找到则进行删除操作
		depot_unused.emplace_back(depot_id);
		depots_.erase(iter);
	}
}
int AllNodes::push_node(ISolutionNode* node) {
	// unused为空，则使用cur_num_unused
	if (node_unused.empty()) {
		push_node(node_num_unused, node);
		return node_num_unused++;
	} else { // 使用并删除最后一个序号
		int num = node_unused.back();
		node_unused.pop_back();
		push_node(num, node);
		return num;
	}
}

int AllNodes::push_node(int node_id, ISolutionNode* node) {
	if (nodes_.count(node_id)) {
		throw MyException("You can not insert depot already exists.");
	}
	nodes_.emplace(node_id, node);
	return node_id;

}

int AllNodes::push_const_depot(ISolutionNode* const_depot) {
	// depot_unused为空，则使用depot_num_unused
	if (depot_unused.empty()) {
		push_const_depot(depot_num_unused, const_depot);
		return depot_num_unused--;
	}
	else { // 使用并删除最后一个序号
		int num = depot_unused.back();
		depot_unused.pop_back();
		push_const_depot(num, const_depot);
		return num;
	}
}

int AllNodes::push_const_depot(int depot_id, ISolutionNode* const_depot) {
	if (const_depots_.count(depot_id)) {
		throw MyException("You can not insert depot already exists.");
	}
	const_depots_.emplace(depot_id, const_depot);
	return depot_id;
}

int AllNodes::push_depot(ISolutionNode* depot) {
	// depot_unused为空，则使用depot_num_unused
	if (depot_unused.empty()) {
		push_depot(depot_num_unused, depot);
		return depot_num_unused--;
	}
	else { // 使用并删除最后一个序号
		int num = depot_unused.at(depot_unused.size() - 1);
		depot_unused.pop_back();
		push_depot(num, depot);
		return num;
	}
}
int AllNodes::push_depot(int depot_id, ISolutionNode* depot) {
	if (depots_.count(depot_id)) {
		throw MyException("You can not insert depot already exists.");
	}
	depots_.emplace(depot_id, move(depot));
	return depot_id;
}

ISolutionNode* AllNodes::get_node(int id, int type)
{
	if ((type & 0b001) == 0b001) {
		auto result_node = nodes_.find(id);
		if (result_node != nodes_.end()) {
			return &(*result_node->second);
		}
	}
	if ((type & 0b010) == 0b010) {
		auto result_depot = depots_.find(id);
		if (result_depot != depots_.end()) {
			return &(*result_depot->second);
		}
	}
	if ((type & 0b100) == 0b100) {
		auto result_const_depot = const_depots_.find(id);
		if (result_const_depot != const_depots_.end()) {
			return &(*result_const_depot->second);
		}
	}
	throw MyException("No Node here.");
}

int AllNodes::count(int id) {
	int result = nodes_.count(id);
	if (result > 0) {
		return result;
	} else {
		return const_depots_.count(id);
	}
}

enum NodeType AllNodes::get_node_type(int node_index) {
	if (nodes_.count(node_index)) {
		return NodeType::TYPE_NODE;
	}
	if (depots_.count(node_index)) {
		return NodeType::TYPE_DEPOT;
	}
	throw MyException("Unknwon node is required.");
	return NodeType::ERROR_TYPE;
}
const int AllNodes::get_size(int type) {
	switch (type)
	{
	case 0b001:
		return nodes_.size();
	case 0b010:
		return depots_.size();
	case 0b100:
		return const_depots_.size();
	default:
		throw MyException("Unknown type.");
	}
}

void AllNodes::push_back_flight_tasks(vector<int>& tasks) {
	if (flight_tasks_ == nullptr) {
		flight_tasks_ = new vector<vector<int>>;
	}
	flight_tasks_->emplace_back(move(tasks));
}