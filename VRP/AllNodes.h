#pragma once

#include <unordered_map>

#include "ALNS/BasicClass/ISolutionNode.h"
using namespace std;
/*
* type˵����Ϊ�̶���վ+��վ+��ͨ�ڵ� 1+1+1=0b111
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

	/*���ýڵ�״̬*/
	// ���нڵ���Ϊδ֪
	void set_state_all_unknown();
	// ����node�ڵ�����Ϊδ���
	void set_nodes_unchecked();
	// ����const_depot�ڵ�����Ϊδ���
	void set_const_depot_unchecked();
	// ����depot�ڵ�����Ϊδ���
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

	// �����վ�ڵ�
	int count(int id);

	// ȫ�ֺ���
	enum NodeType get_node_type(int node_index);
	// ��ô�С
	const int get_size(int type);
	// �������
	const FerryVehicleTask get_task(int task_index);
	// ��������
	void push_back_flight_tasks(vector<int>& tasks);
	

private:
	unordered_map<int, ISolutionNode*>				nodes_; // ��ͨ�ڵ�
	unordered_map<int, ISolutionNode*>				const_depots_; // ��ʼ��վ
	unordered_map<int, ISolutionNode*>				depots_; // ��ʱ��վ�ڵ�
	vector<vector<int>>*							flight_tasks_; // �ڶɳ������б�
	
	int												node_num_unused  = 1;
	int												depot_num_unused = -3; // ��Ϊ�̶���վ�����������Դ˴���-3��ʼ�������ͻ��
	vector<int>										node_unused;
	vector<int>										depot_unused;
};

