#ifndef CARRIER_CLASS
#define CARRIER_CLASS

#include <ctime>
#include <iostream>
#include <list>
#include "Event.h"

using namespace std;

struct User  //�����û��ṹ�壬���ڴ�ŷ�����ĳ�ز��ϵ��û���Ϣ
{
	UserType Utype;  //�û�����
	int UserID;  //�û�ID
	double ThrRequirment;  //�û�����������
	double ThrActural;  //�û�ʵ����������С
};

typedef list<User> LU;  //����������ͣ�����û���Ϣ��list

class Carrier  //�����ز���
{	
    private:
		LU LU1;  //�û�����1����Ϣ
		LU LU2;  //�û�����2����Ϣ
		int CarrierID;  //�ز�ID
		double SumThrU1 = 0;  //�û�����1����������
		int SizeU2 = 0;  //�û�����2������Ŀ
		int Nw;  //WiFi�û�����Ŀ
		double Rl;  //���ز��ϵ�LTEϵͳ�������������ŵ������й�
		double Rw;  //���ز��ϵ�WiFi�����������������ŵ������й�

	public:
		Carrier(LU L1, LU L2, int C, int N, double R1, double R2);
		void SetLU1(LU L1);
		LU GetLU1();
		void SetLU2(LU L2);
		LU GetLU2();
		int GetCarrierID();
		double GetSumThrU1();
		int GetSizeU2();
		int GetNw();
		double GetRl();
		double GetRw();
};

Carrier::Carrier(LU L1, LU L2, int C, int N, double R1, double R2)  //�ز��Ĺ��캯������ʼ���ز���Ϣ
{
	LU1 = L1;
	LU2 = L2;
	CarrierID = C;
	Nw = N;
	Rl = R1;
	Rw = R2;
}

void Carrier::SetLU1(LU L1)  //�ⲿ���������ø��ز��ϵ��û�����1��Ϣ
{
	LU1.clear();
	LU1 = L1;
}

LU Carrier::GetLU1()  //�ⲿ��������ȡ���ز��ϵ��û�����1��Ϣ
{
	return LU1;
}

void Carrier::SetLU2(LU L2)  //�ⲿ���������ø��ز��ϵ��û�����2��Ϣ
{
	LU2.clear();
	LU2 = L2;
}

LU Carrier::GetLU2()  //�ⲿ��������ȡ���ز��ϵ��û�����2��Ϣ
{
	return LU2;
}

int Carrier::GetCarrierID()  //�ⲿ��������ȡ���ز���ID
{
	return CarrierID;
}

double Carrier::GetSumThrU1()  //�ⲿ��������ȡ���ز��ϵ�����1�û���������������  
{
	
	LU::iterator iter;  //����һ��LU������
	SumThrU1 = 0;  //ÿ�λ�ȡ֮ǰ��Ҫ������֮ǰ������
	for (iter = LU1.begin(); iter != LU1.end(); iter++)
	{
		SumThrU1 += iter->ThrRequirment;  //����LU1 list������1�û���������������
	}
	return SumThrU1;
}

int Carrier::GetSizeU2()  //�ⲿ��������ȡ���ز��ϵ�����2�û���Ŀ
{
	return LU2.size();
}

int Carrier::GetNw()  //�ⲿ��������ȡ���ز��ϵ�WiFi�û���Ŀ
{
	return Nw;
}

double Carrier::GetRl()  //�ⲿ��������ȡ���ز��ϵ�LTEϵͳ��������
{
	return Rl;
}
double Carrier::GetRw()  //�ⲿ��������ȡ���ز��ϵ�WiFi������������
{
	return Rw;
}

#endif