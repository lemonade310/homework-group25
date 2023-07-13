#include <iostream>
#include<string>
#include<ctime>
#include "merkletree.h"
using namespace std;


int main()
{
	cout << "������Merkle Tree��Ҷ�ӽ������ݣ��ԡ�#����Ϊ������: " << endl;
	vector<string> leaf;

	while (1) { //����Ҷ�ӽڵ�
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
	cout << "MerkleTree����ֵ���£�" << endl;
	newtree.BuildTree();
	cout << endl;
	time2 = clock();
	cout << "����Ĭ�˶�������ʱ��Ϊ��" << (time2 - time1) << "ms" << endl;

	string check_str = "";
	cout << "��������Ҫ��֤������: " << endl;
	cin >> check_str; //��������֤��Ҷ�ӽڵ�
	check_str = iteration(padding(check_str));
	cout << "��֤�����ݵĹ�ϣֵ: " << check_str << endl;
	
	clock_t time3, time4;
	time3 = clock();
	if (newtree.Hash_Verify(check_str)) {//��֤��������ڵ� �����޸ı�
		cout << endl;
		cout << "��Ĭ�˶����ϴ�����֤�����ݵ�Ҷ�ӽ��";
		cout << "���Ҹ���δ���ı䡣" << endl;
	}
	else{
		cout << "Merkle���ϲ�������֤������" << endl;
	}
	time4 = clock();
	cout << "��֤����ʱ��Ϊ��" << (time4 - time3) << "ms" << endl;
	return 0;
}

