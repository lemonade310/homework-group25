#include<iostream>
#include<string>
#include<ctime>
#include"sm3.h"

using namespace std;
//�������ֵѡ����ʴ�С��ȡֵ��Χ
#define max_num 65537

string list[max_num];  //������ϣֵ��Ӧ��
bool sm3_birthday_attack() {
	for (int i = 0; i < max_num; i++) {
		string stri = to_string(i);
		string paddingValue = padding(stri);
		string result = iteration(paddingValue);
		list[i] = result;
		for (int j = 0; j < i; j++) {
			if (list[j].substr(0, 64) == result.substr(0, 64)) {
				cout << "��ײֵ:" << stri << endl;
				cout << "���ϣֵΪ:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << result.substr(8 * i, 8) << ' ';
				}
				cout << endl;
				string strj = to_string(j);
				cout << "��ײֵ:" << strj << endl;
				cout << "���ϣֵΪ:" << endl;
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
		cout << "�˴����չ���ʧ�ܣ�" << endl;
	time2 = clock();
	cout << "����ʱ��Ϊ��" << (time2 - time1) << "ms" << endl;
	return 0;
}