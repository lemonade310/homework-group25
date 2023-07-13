#pragma once
#include<iostream>
#include <string>
#include <vector>
#include "sm3.h"
using namespace std;


//�����
class node{
private:
	string hash_str;
	node* parent;
	node* children[2];
public:
	node();

	void setParent(node* parent);
	node* getParent();

	void setChildren(node* children_l, node* children_r);
	node* getChildren(int index);
	node* getSibling();

	void setHash(string hash_str);
	string getHash();
	bool ifleft();


	virtual ~node();
};
node::node(){
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
}

//���ù�ϣֵ
void node::setHash(string hash_str){
	this->hash_str = iteration(padding(hash_str));
}
node* node::getParent(){
	return parent;
}
void node::setParent(node* parent){
	this->parent = parent;
}
void node::setChildren(node* children_l, node* children_r){
	children[0] = children_l;
	children[1] = children_r;
}
node* node::getSibling() { //����ֵܽ���ֵ
	//�õ��ýڵ�ĸ��ڵ�
	node* parent = getParent();

	//�жϸ��ڵ�����Ӻͱ��ڵ��Ƿ���ͬ
	//��ͬ�����Һ��ӣ���ͬ��������
	return parent->getChildren(0) == this ? parent->getChildren(1) : parent->getChildren(0);
}
node* node::getChildren(int index){
	return index <= 1 ? children[index] : nullptr;
}
string node::getHash(){
	return hash_str;
}
bool node::ifleft(){
	//����丸�ڵ�������Ǹýڵ� ����0 �����򷵻�1
	return parent->getChildren(0) == this ? 0 : 1;
}

node::~node() {}

//Ĭ�˶�����
class MerkleTree{
private:
	string MerkleRoot;
	int makeBinary(vector<node*>& node_vector);
	void printTreeLevel(vector<node*> v);
	vector<vector<node*>> nodelist; //��������һ�����ڵ��б�
public:
	MerkleTree();
	void BuildTree();
	void BuildLeaves(vector<string> base_leaf);
	bool Hash_Verify(string hash);
	virtual ~MerkleTree();
};

MerkleTree::MerkleTree() {}

int MerkleTree::makeBinary(vector<node*>& node_vector) { //ʹҶ�ӽڵ��Ϊ˫��
	int vectSize = node_vector.size();
	if ((vectSize % 2) != 0) //���Ԫ�ظ���Ϊ�������Ͱ������һ���ڵ�push_backһ��
	{
		node_vector.push_back(node_vector.end()[-1]);
		vectSize++;
	}
	return vectSize;
}

void MerkleTree::printTreeLevel(vector<node*> v){
	for (node* el : v)
	{
		cout << el->getHash() << endl;
	}
	cout << endl;
}

void MerkleTree::BuildLeaves(vector<string> base_leaf) { //����Ҷ�ӽڵ��б�
	vector<node*> new_leaf;

	//cout << "Ҷ�ӽ�㼰��Ӧ�Ĺ�ϣֵ��" << endl;

	for (auto leaf : base_leaf) { //��ÿһ���ַ���������Ӧ�ڵ㣬��ͨ������ַ������ù�ϣֵ
		node* new_node = new node;
		new_node->setHash(leaf);
		//cout << leaf <<"�Ĺ�ϣֵΪ��"<< new_node->getHash() << endl;

		new_leaf.push_back(new_node);
	}

	nodelist.push_back(new_leaf);//��Ҷ�ӽ��ѹ�����б���
	cout << endl;
}

void MerkleTree::BuildTree() { //����Merkle Tree
	int count = 1;
	do{
		vector<node*> new_nodes;
		makeBinary(nodelist.end()[-1]); //����βԪ�� ��һ���ڵ��б�
		cout << "��"<<count<<"������������Ϊ:" << endl;
		printTreeLevel(nodelist.end()[-1]);//��ӡ�ò���
		cout << endl;
		for (int i = 0; i < nodelist.end()[-1].size(); i += 2){
			node* new_parent = new node; //���ø��׽ڵ�
			//���б�ĵ�i�͵�i+1�����ĸ��ڵ�����Ϊnew_parent
			nodelist.end()[-1][i]->setParent(new_parent);
			nodelist.end()[-1][i + 1]->setParent(new_parent);

			//���ڵ��ϣֵ=Hash(����+�Һ���)
			new_parent->setHash(nodelist.end()[-1][i]->getHash() + nodelist.end()[-1][i + 1]->getHash());
			//���ø��ڵ�����Һ��ӽڵ�����Ϊ��i�͵�i+1�����
			new_parent->setChildren(nodelist.end()[-1][i], nodelist.end()[-1][i + 1]);
			//��new_parentѹ��new_nodes
			new_nodes.push_back(new_parent);
		}
		
		nodelist.push_back(new_nodes); //����һ�ֵĸ��ڵ�new_nodesѹ��nodelist
		count++;

	} while (nodelist.end()[-1].size() > 1); //����ÿһ�ֵõ���һ��ĸ��ڵ㣬ֱ���õ����ڵ㣬�˳�ѭ��
	cout << "��" << count << "������������Ϊ:" << endl;
	printTreeLevel(nodelist.end()[-1]);
	cout << endl;

	cout << "��Ĭ�˶�����" << count << "��" << endl;
	MerkleRoot = nodelist.end()[-1][0]->getHash(); //���ڵ�Ĺ�ϣֵ
	cout << "���ڵ�Ϊ��" << MerkleRoot << endl << endl;
}

bool MerkleTree::Hash_Verify(string hash){//��֤�Ƿ��޸Ĺ�
	node* venode = nullptr;
	string act_hash = hash;

	for (int i = 0; i < nodelist[0].size(); i++){//Ѱ���Ƿ�ý���Ƿ����
		if (nodelist[0][i]->getHash() == hash){
			venode = nodelist[0][i];
		}
	}
	if (venode == nullptr){
		return 0;
	}

	cout << "��֤�Ĺ�ϣֵ:" << endl;
	cout << act_hash << endl;

	do {//��֤merkle tree�Ƿ�ı�� 
		if (venode->ifleft() == 0){//��Ϊ���ӣ���+���ֵ�����ϣ
			act_hash = iteration(padding(act_hash + venode->getSibling()->getHash()));
		}
		else{//��Ϊ�Һ��ӣ������ֵ�+����ϣ
			act_hash = iteration(padding(venode->getSibling()->getHash() + act_hash));
		}
		cout << act_hash << endl;

		venode = venode->getParent();
	} while ((venode->getParent()) != NULL); //������ڵ�

	return act_hash == MerkleRoot ? 1 : 0;
}

MerkleTree::~MerkleTree() {}

