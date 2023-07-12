#include<iostream>
#include<string>
#include<ctime>
#include"SM3.h"

using namespace std;
//�������ֵѡ����ʴ�С��ȡֵ��Χ
#define max_num 65537

string lista[max_num];  //���ڴ洢ԭ��Ϣֵ
string listb[max_num];  //���ڴ洢��Ӧ��ϣֵ
bool sm3_rho_attack() {
	int rho = 0;
	int start = 0;
	for (int i = 0; i < max_num; i++) {
		if (rho != -1) {
			rho = 2* rho + 1;	//�趨f(x)=2*x+1
		}
		else {//����int�����Χʱ�������趨rho
			start += 1;
			rho = start;
		}
		string stri = to_string(rho);
		lista[i] = stri;  //ԭ��Ϣֵ
		string paddingValue = padding(stri);
		string result = iteration(paddingValue);
		listb[i] = result;  //��Ӧ��ϣֵ

		for (int j = 0; j < i; j++) {
			if (listb[j].substr(0, 64) == result.substr(0, 64) && stri != lista[j]) {
				cout << "��ײֵ:" << stri << endl;
				cout << "���ϣֵΪ:" << endl;
				for (int i = 0; i < 8; i++) {
					cout << result.substr(8 * i, 8) << ' ';
				}
				cout << endl;
				
				cout << "��ײֵ:" << lista[j] << endl;
				cout << "���ϣֵΪ:" << endl;
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
		cout << "����Pollard rho�㷨ʧ�ܣ�" << endl;
	time2 = clock();
	cout << "����ʱ��Ϊ��" << (time2 - time1) << "ms" << endl;
	return 0;
}