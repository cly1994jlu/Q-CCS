#ifndef CARRIER_CLASS
#define CARRIER_CLASS

#include <ctime>
#include <iostream>
#include <list>
#include "Event.h"

using namespace std;

struct User  //定义用户结构体，用于存放放置在某载波上的用户信息
{
	UserType Utype;  //用户类型
	int UserID;  //用户ID
	double ThrRequirment;  //用户吞吐量需求
	double ThrActural;  //用户实际吞吐量大小
};

typedef list<User> LU;  //定义变量类型：存放用户信息的list

class Carrier  //定义载波类
{	
    private:
		LU LU1;  //用户类型1的信息
		LU LU2;  //用户类型2的信息
		int CarrierID;  //载波ID
		double SumThrU1 = 0;  //用户类型1的总吞吐量
		int SizeU2 = 0;  //用户类型2的总数目
		int Nw;  //WiFi用户的数目
		double Rl;  //该载波上的LTE系统总吞吐量，与信道质量有关
		double Rw;  //该载波上的WiFi网络总吞吐量，与信道质量有关

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

Carrier::Carrier(LU L1, LU L2, int C, int N, double R1, double R2)  //载波的构造函数，初始化载波信息
{
	LU1 = L1;
	LU2 = L2;
	CarrierID = C;
	Nw = N;
	Rl = R1;
	Rw = R2;
}

void Carrier::SetLU1(LU L1)  //外部函数，设置该载波上的用户类型1信息
{
	LU1.clear();
	LU1 = L1;
}

LU Carrier::GetLU1()  //外部函数，获取该载波上的用户类型1信息
{
	return LU1;
}

void Carrier::SetLU2(LU L2)  //外部函数，设置该载波上的用户类型2信息
{
	LU2.clear();
	LU2 = L2;
}

LU Carrier::GetLU2()  //外部函数，获取该载波上的用户类型2信息
{
	return LU2;
}

int Carrier::GetCarrierID()  //外部函数，获取该载波的ID
{
	return CarrierID;
}

double Carrier::GetSumThrU1()  //外部函数，获取该载波上的类型1用户的总吞吐量需求  
{
	
	LU::iterator iter;  //声明一个LU迭代器
	SumThrU1 = 0;  //每次获取之前需要先清零之前的数据
	for (iter = LU1.begin(); iter != LU1.end(); iter++)
	{
		SumThrU1 += iter->ThrRequirment;  //计算LU1 list中类型1用户的总吞吐量需求
	}
	return SumThrU1;
}

int Carrier::GetSizeU2()  //外部函数，获取该载波上的类型2用户数目
{
	return LU2.size();
}

int Carrier::GetNw()  //外部函数，获取该载波上的WiFi用户数目
{
	return Nw;
}

double Carrier::GetRl()  //外部函数，获取该载波上的LTE系统总吞吐量
{
	return Rl;
}
double Carrier::GetRw()  //外部函数，获取该载波上的WiFi网络总吞吐量
{
	return Rw;
}

#endif