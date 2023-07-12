#include<iostream>
#include<string>
#include<ctime>
#include"sm3.h"

using namespace std;

//加入初始向量IV作为参数的迭代压缩函数
string new_iteration(string str, string V) {
	int num = str.size() / 128;
	string B = "", extensionB = "", compressB = "";
	for (int i = 0; i < num; i++) {
		B = str.substr(i * 128, 128);
		extensionB = extension(B);
		compressB = compress(extensionB, V);
		V = XOR(V, compressB);
	}
	return V;
}

//长度扩展攻击的实现
string sm3_len_extension_attack(string hash1,int len, string exstr) {
	string paddingvalue = padding(exstr);//扩展消息填充
	string newstr = DecToHex(HexToDec(paddingvalue) + len);//将扩展的消息与原始长度组装在一起
	string hash2 = new_iteration(newstr, hash1);  //将原始哈希值作为IV对其进行迭代压缩
	return hash2;
}

int main() {
	string origin="secretdata32aheshlydj1234sdh7iuqsecretdata32aheshlydj1234sdh7iuq";
	string paddingvalue = padding(origin);
	string hashorigin = iteration(paddingvalue);  //原始哈希值
	cout << "原始哈希值：" << hashorigin << endl;

	int lenth = paddingvalue.length();  //填充后数据的长度

	string extend = "dataextend32atwweuodkjnbxfghjtjhdataextend32atwweuodkjnbxfghjtjh";  //扩展的数据
	clock_t time1, time2;
	time1 = clock();
	
	string hashextend;
	hashextend = sm3_len_extension_attack(hashorigin, lenth, extend);
	cout << "扩展后哈希值：" << hashextend << endl;

	if (hashextend == hashorigin) {
		cout << "长度扩展攻击成功！" << endl;
	}
	else {
		cout << "长度扩展攻击失败!" << endl;
	}
	
	time2 = clock();
	cout << "所用时间为：" << (time2 - time1) << "ms" << endl;
	return 0;
}