#pragma once
#include<iostream>
#include <string>
#include <vector>
#include "sm3.h"
using namespace std;


//结点类
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

//设置哈希值
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
node* node::getSibling() { //获得兄弟结点的值
	//得到该节点的父节点
	node* parent = getParent();

	//判断父节点的左孩子和本节点是否相同
	//相同返回右孩子，不同返回左孩子
	return parent->getChildren(0) == this ? parent->getChildren(1) : parent->getChildren(0);
}
node* node::getChildren(int index){
	return index <= 1 ? children[index] : nullptr;
}
string node::getHash(){
	return hash_str;
}
bool node::ifleft(){
	//如果其父节点的左孩子是该节点 返回0 否则则返回1
	return parent->getChildren(0) == this ? 0 : 1;
}

node::~node() {}

//默克尔树类
class MerkleTree{
private:
	string MerkleRoot;
	int makeBinary(vector<node*>& node_vector);
	void printTreeLevel(vector<node*> v);
	vector<vector<node*>> nodelist; //里面存的是一个个节点列表
public:
	MerkleTree();
	void BuildTree();
	void BuildLeaves(vector<string> base_leaf);
	bool Hash_Verify(string hash);
	virtual ~MerkleTree();
};

MerkleTree::MerkleTree() {}

int MerkleTree::makeBinary(vector<node*>& node_vector) { //使叶子节点成为双数
	int vectSize = node_vector.size();
	if ((vectSize % 2) != 0) //如果元素个数为奇数，就把再最后一个节点push_back一次
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

void MerkleTree::BuildLeaves(vector<string> base_leaf) { //建立叶子节点列表
	vector<node*> new_leaf;

	//cout << "叶子结点及对应的哈希值：" << endl;

	for (auto leaf : base_leaf) { //给每一个字符串创建对应节点，并通过这个字符串设置哈希值
		node* new_node = new node;
		new_node->setHash(leaf);
		//cout << leaf <<"的哈希值为："<< new_node->getHash() << endl;

		new_leaf.push_back(new_node);
	}

	nodelist.push_back(new_leaf);//将叶子结点压入结点列表中
	cout << endl;
}

void MerkleTree::BuildTree() { //构造Merkle Tree
	int count = 1;
	do{
		vector<node*> new_nodes;
		makeBinary(nodelist.end()[-1]); //传入尾元素 即一个节点列表
		cout << "第"<<count<<"层结点由左至右为:" << endl;
		printTreeLevel(nodelist.end()[-1]);//打印该层结点
		cout << endl;
		for (int i = 0; i < nodelist.end()[-1].size(); i += 2){
			node* new_parent = new node; //设置父亲节点
			//将列表的第i和第i+1个结点的父节点设置为new_parent
			nodelist.end()[-1][i]->setParent(new_parent);
			nodelist.end()[-1][i + 1]->setParent(new_parent);

			//父节点哈希值=Hash(左孩子+右孩子)
			new_parent->setHash(nodelist.end()[-1][i]->getHash() + nodelist.end()[-1][i + 1]->getHash());
			//将该父节点的左右孩子节点设置为第i和第i+1个结点
			new_parent->setChildren(nodelist.end()[-1][i], nodelist.end()[-1][i + 1]);
			//将new_parent压入new_nodes
			new_nodes.push_back(new_parent);
		}
		
		nodelist.push_back(new_nodes); //将新一轮的父节点new_nodes压入nodelist
		count++;

	} while (nodelist.end()[-1].size() > 1); //这样每一轮得到新一层的父节点，直至得到根节点，退出循环
	cout << "第" << count << "层结点由左至右为:" << endl;
	printTreeLevel(nodelist.end()[-1]);
	cout << endl;

	cout << "该默克尔树共" << count << "层" << endl;
	MerkleRoot = nodelist.end()[-1][0]->getHash(); //根节点的哈希值
	cout << "根节点为：" << MerkleRoot << endl << endl;
}

bool MerkleTree::Hash_Verify(string hash){//验证是否被修改过
	node* venode = nullptr;
	string act_hash = hash;

	for (int i = 0; i < nodelist[0].size(); i++){//寻找是否该结点是否存在
		if (nodelist[0][i]->getHash() == hash){
			venode = nodelist[0][i];
		}
	}
	if (venode == nullptr){
		return 0;
	}

	cout << "验证的哈希值:" << endl;
	cout << act_hash << endl;

	do {//验证merkle tree是否改变过 
		if (venode->ifleft() == 0){//若为左孩子，则+右兄弟做哈希
			act_hash = iteration(padding(act_hash + venode->getSibling()->getHash()));
		}
		else{//若为右孩子，则左兄弟+做哈希
			act_hash = iteration(padding(venode->getSibling()->getHash() + act_hash));
		}
		cout << act_hash << endl;

		venode = venode->getParent();
	} while ((venode->getParent()) != NULL); //到达根节点

	return act_hash == MerkleRoot ? 1 : 0;
}

MerkleTree::~MerkleTree() {}

