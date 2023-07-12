#include<iostream>
#include<string>
#include<ctime>
#include"sm3.h"

using namespace std;

//�����ʼ����IV��Ϊ�����ĵ���ѹ������
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

//������չ������ʵ��
string sm3_len_extension_attack(string hash1,int len, string exstr) {
	string paddingvalue = padding(exstr);//��չ��Ϣ���
	string newstr = DecToHex(HexToDec(paddingvalue) + len);//����չ����Ϣ��ԭʼ������װ��һ��
	string hash2 = new_iteration(newstr, hash1);  //��ԭʼ��ϣֵ��ΪIV������е���ѹ��
	return hash2;
}

int main() {
	string origin="secretdata32aheshlydj1234sdh7iuqsecretdata32aheshlydj1234sdh7iuq";
	string paddingvalue = padding(origin);
	string hashorigin = iteration(paddingvalue);  //ԭʼ��ϣֵ
	cout << "ԭʼ��ϣֵ��" << hashorigin << endl;

	int lenth = paddingvalue.length();  //�������ݵĳ���

	string extend = "dataextend32atwweuodkjnbxfghjtjhdataextend32atwweuodkjnbxfghjtjh";  //��չ������
	clock_t time1, time2;
	time1 = clock();
	
	string hashextend;
	hashextend = sm3_len_extension_attack(hashorigin, lenth, extend);
	cout << "��չ���ϣֵ��" << hashextend << endl;

	if (hashextend == hashorigin) {
		cout << "������չ�����ɹ���" << endl;
	}
	else {
		cout << "������չ����ʧ��!" << endl;
	}
	
	time2 = clock();
	cout << "����ʱ��Ϊ��" << (time2 - time1) << "ms" << endl;
	return 0;
}