#include<iostream>
#include<string>
#include<ctime>
#include"sm3.h"

using namespace std;
//根据输出值选择合适大小的取值范围
#define max_num 65537

string list[max_num];  //建立哈希值对应表
bool sm3_birthday_attack() {
	for (int i = 0; i < max_num; i++) {
		string stri = to_string(i);
		string paddingValue = padding(stri);
		string result = iteration(paddingValue);
		list[i] = result;
		for (int j = 0; j < i; j++) {
			if (list[j].substr(0, 64) == result.substr(0, 64)) {
				cout << "碰撞值:" << stri << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << result.substr(8 * i, 8) << ' ';
				}
				cout << endl;
				string strj = to_string(j);
				cout << "碰撞值:" << strj << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << list[j].substr(8 * i, 8) << ' ';
				}
				cout << endl;
				return 1;
			}
		}
	}
	return 0;
}

int main() {
	clock_t time1, time2;
	time1 = clock();
	if (sm3_birthday_attack() == 0)
		cout << "此次生日攻击失败！" << endl;
	time2 = clock();
	cout << "所用时间为：" << (time2 - time1) << "ms" << endl;
	return 0;
}
