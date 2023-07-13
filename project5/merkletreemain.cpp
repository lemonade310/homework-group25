#include <iostream>
#include<string>
#include<ctime>
#include "merkletree.h"
using namespace std;


int main()
{
	cout << "请输入Merkle Tree的叶子结点的数据，以‘#’作为结束符: " << endl;
	vector<string> leaf;

	while (1) { //输入叶子节点
		string str;
		cin >> str;
		if (str != "#"){
			leaf.push_back(str);
		}
		else
			break;
	}
	clock_t time1, time2;
	time1 = clock();
	MerkleTree newtree;
	newtree.BuildLeaves(leaf);
	cout << "MerkleTree各层值如下：" << endl;
	newtree.BuildTree();
	cout << endl;
	time2 = clock();
	cout << "构造默克尔树所用时间为：" << (time2 - time1) << "ms" << endl;

	string check_str = "";
	cout << "请输入想要验证的数据: " << endl;
	cin >> check_str; //输入想验证的叶子节点
	check_str = iteration(padding(check_str));
	cout << "验证的数据的哈希值: " << check_str << endl;
	
	clock_t time3, time4;
	time3 = clock();
	if (newtree.Hash_Verify(check_str)) {//验证有无这个节点 树有无改变
		cout << endl;
		cout << "该默克尔树上存在验证的数据的叶子结点";
		cout << "并且该树未被改变。" << endl;
	}
	else{
		cout << "Merkle树上不存在验证的数据" << endl;
	}
	time4 = clock();
	cout << "验证所用时间为：" << (time4 - time3) << "ms" << endl;
	return 0;
}

