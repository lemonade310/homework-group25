#include<iostream>
#include<string>
#include<ctime>
#include"sm3.h"

using namespace std;

int main() {
	string str;
	str = "abcdabcdabcdabcdabcdabcdabcdabcd";
	clock_t time1, time2;
	time1 = clock();
	string paddingValue = padding(str);
	string result = iteration(paddingValue);
	cout << "�Ӵ�ֵ��" << endl;
	for (int i = 0; i < 8; i++) {
		cout << result.substr(i * 8, 8) << "  ";
	}
	cout << endl;
	time2 = clock();
	cout << "����ʱ��Ϊ��" << (time2 - time1) << "ms" << endl;
	return 0;
}