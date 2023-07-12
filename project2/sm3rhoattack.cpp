#include<iostream>
#include<string>
#include<ctime>
#include"SM3.h"

using namespace std;
//根据输出值选择合适大小的取值范围
#define max_num 65537

string lista[max_num];  //用于存储原消息值
string listb[max_num];  //用于存储对应哈希值
bool sm3_rho_attack() {
	int rho = 0;
	int start = 0;
	for (int i = 0; i < max_num; i++) {
		if (rho != -1) {
			rho = 2* rho + 1;	//设定f(x)=2*x+1
		}
		else {//超过int的最大范围时，重新设定rho
			start += 1;
			rho = start;
		}
		string stri = to_string(rho);
		lista[i] = stri;  //原消息值
		string paddingValue = padding(stri);
		string result = iteration(paddingValue);
		listb[i] = result;  //对应哈希值

		for (int j = 0; j < i; j++) {
			if (listb[j].substr(0, 64) == result.substr(0, 64) && stri != lista[j]) {
				cout << "碰撞值:" << stri << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << result.substr(8 * i, 8) << ' ';
				}
				cout << endl;
				
				cout << "碰撞值:" << lista[j] << endl;
				cout << "其哈希值为:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << listb[j].substr(8 * i, 8) << ' ';
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
	if (sm3_rho_attack() == 0)
		cout << "本次Pollard rho算法失败！" << endl;
	time2 = clock();
	cout << "所用时间为：" << (time2 - time1) << "ms" << endl;
	return 0;
}