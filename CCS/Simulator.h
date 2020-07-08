#ifndef SIMULATOR_CLASS
#define SIMULATOR_CLASS

#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <vector>
#include <list>
#include <stdlib.h>
#include <math.h>
#include "Carrier.h"
#include "RandomNumber.h"
#include "Event.h"

#define MAX 1000  //宏定义一个最大值为1000
#define NumCarriers 5  //宏定义载波个数为5
#define Portion 0.8  //宏定义类型1用户比例为0.5
#define Rwmin 8.0  //宏定义WiFi网络最小吞吐量需求为8.0
int Nw[NumCarriers] = { 5, 5, 5, 5, 5 };  //预加载各载波上的WiFi网络用户数，事先设置的结果，假设仿真过程中不发生变化
double RlTot[NumCarriers] = { 175, 165, 155, 149, 154 };  //预加载各载波上的LTE系统总吞吐量，由matlab仿真得到
double RwTot[NumCarriers] = { 49, 55, 50, 59, 49 };  //预加载各载波上的WiFi网络总吞吐量，由matlab仿真得到
double U1Thr[4] = { 10, 20, 30, 40 };  //定义LTE用户类型1的吞吐量需求，随机从4种里面选择

using namespace std;

class Simulator
{
private:
	int StartTime, EndTime;  //模拟起止时间
	double PacketLow, PacketHigh;  //数据包大小的分布范围：均匀分布
	int lamda;  //用户到达均值：泊松分布
	int NumArrived = 0;  //统计到达的用户数	
	int NumAccepted = 0;  //统计接受服务的用户数
	int NumRejected = 0;  //统计被拒绝的用户数
	int NumRejectedl1 = 0;  //统计被拒绝的LTE类型1用户数
	int NumRejectedl2 = 0;  //统计被拒绝的LTE类型2用户数

	vector<Carrier> CCstat;  //承载各载波信息的容器
	priority_queue<Event> Queue;  //承载各事件的优先级队列
	vector<double> Rl1_A;  //承载Rl1_A数据的容器，LTE类型1用户实际获得的吞吐量统计
	vector<double> Rl1_R;  //承载Rl1_R数据的容器，LTE类型1用户需要获得的吞吐量统计
	vector<double> Rl2;  //承载Rl2数据的容器
	vector<double> Rw;  //承载Rw数据的容器
	RandomNumber r;  //随机数

	int NextArrivalTime(int lamda);  //获取下一个用户的到达时间		
	double GetPacketLength();  //获取数据包大小
	int SelectCarrier(UserType ut, double thr);  //载波选择机制
	double GetServiceThr(UserType ut, int CID, double thr);  //计算用户获得的吞吐量
	void UpdateCarrierInfor(EventType et, UserType ut, int UID, int CID, double thrR, double thrA);  //更新载波信息			

public:
	Simulator();  //构造函数，初始化调度器
	void RunSimulation();  //执行仿真程序
	void PrintSimulation();  //输出仿真结果
};

int Simulator::NextArrivalTime(int lamda)  //计算下一个用户的到达时间间隔，服从泊松分布
{
	return (r.possion(lamda));
}

double Simulator::GetPacketLength()  //计算数据包的大小
{
	return (PacketLow + r.random(PacketHigh - PacketLow + 1));
}

int Simulator::SelectCarrier(UserType ut, double thr)  //载波选择机制
{
	int i = 0;
	int SelectedCarrierIndex = 0;  //所选择的载波号，默认为0
	double Rl2ThrSelected = 0;  //所选择的载波上类型2用户平均吞吐量
	double RwThrSelected = 0;  //所选择的载波上WiFi用户平均吞吐量
	vector<Carrier>::iterator iter;  //声明一个承载载波信息的vector迭代器
	switch (ut)
	{
	case U1:
		Rl2ThrSelected = (1 - (thr / CCstat[SelectedCarrierIndex].GetRl()) - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRl()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2());  //计算在所选中的载波上类型2用户所能获得的的平均吞吐量
		RwThrSelected = (1 - (thr / CCstat[SelectedCarrierIndex].GetRl()) - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRw()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2());  //计算在所选中的载波上WiFi用户所能获得的的平均吞吐量
		if (RwThrSelected < Rwmin)
		{
			Rl2ThrSelected = 0;  //WiFi用户性能保障
		}
		for (iter = CCstat.begin(); iter != CCstat.end(); iter++)
		{

			double Rl2ThrCurrent = (1 - (thr / iter->GetRl()) - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRl()) / (iter->GetNw() + iter->GetSizeU2());  //计算在此载波上类型2用户所能获得的的平均吞吐量
			double RwThrCurrent = (1 - (thr / iter->GetRl()) - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRw()) / (iter->GetNw() + iter->GetSizeU2());  //计算在此载波上WiFi用户所能获得的平均吞吐量
			if (RwThrCurrent < Rwmin)
			{
				Rl2ThrCurrent = 0;  //WiFi用户性能保障
			}
			if (Rl2ThrCurrent > Rl2ThrSelected)
			{
				SelectedCarrierIndex = iter->GetCarrierID();
				Rl2ThrSelected = Rl2ThrCurrent;  //更新选择载波信息
			}
		}
		if (Rl2ThrSelected == 0)
		{
			SelectedCarrierIndex = NumCarriers + 1;  //若不存在合适的载波，则返回载波数目加1
		}
		break;
	case U2:
		Rl2ThrSelected = (1 - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRl()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2() + 1);  //计算在所选中的载波上类型2用户所能获得的的平均吞吐量
		RwThrSelected = (1 - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRw()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2() + 1);  //计算在所选中的载波上WiFi用户所能获得的的平均吞吐量
		if (RwThrSelected < Rwmin)
		{
			Rl2ThrSelected = 0;  //WiFi用户性能保障
		}
		for (iter = CCstat.begin(); iter != CCstat.end(); iter++)
		{

			double Rl2ThrCurrent = (1 - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRl()) / (iter->GetNw() + iter->GetSizeU2() + 1);  //计算在此载波上类型2用户所能获得的的平均吞吐量
			double RwThrCurrent = (1 - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRw()) / (iter->GetNw() + iter->GetSizeU2() + 1);  //计算在此载波上WiFi用户所能获得的平均吞吐量
			if (RwThrCurrent < Rwmin)
			{
				Rl2ThrCurrent = 0;  //WiFi用户性能保障
			}
			if (Rl2ThrCurrent > Rl2ThrSelected)
			{
				SelectedCarrierIndex = iter->GetCarrierID();
				Rl2ThrSelected = Rl2ThrCurrent;  //更新选择载波信息
			}
		}
		if (Rl2ThrSelected == 0)
		{
			SelectedCarrierIndex = NumCarriers + 1;  //若不存在合适的载波，则返回载波数目加1
		}
		break;
	default:
		SelectedCarrierIndex = NumCarriers + 1;
		break;
	}
	return SelectedCarrierIndex;
}

double Simulator::GetServiceThr(UserType ut, int CID, double thr)
{
	double ServiceThr;  //LTE用户获得的吞吐量
	double ServiceRw;  //WiFi用户吞吐量
	switch (ut)
	{
	case U1:
		ServiceThr = thr;  //LTE类型1用户吞吐量
		ServiceRw = (1 - (thr / CCstat[CID].GetRl()) - (CCstat[CID].GetSumThrU1() / CCstat[CID].GetRl()))*(CCstat[CID].GetRw()) / (CCstat[CID].GetNw() + CCstat[CID].GetSizeU2());  //WiFi用户吞吐量
		Rl1_A.push_back(ServiceThr);  //存放到Rl1_A容器中进行统计
		Rl1_R.push_back(thr);  //存放到Rl1_R容器中进行统计
		Rw.push_back(ServiceRw);  //存放到Rw容器中进行统计
		break;
	case U2:
		ServiceThr = (1 - (CCstat[CID].GetSumThrU1() / CCstat[CID].GetRl()))*(CCstat[CID].GetRl()) / (CCstat[CID].GetNw() + CCstat[CID].GetSizeU2() + 1);
		ServiceRw = (1 - (CCstat[CID].GetSumThrU1() / CCstat[CID].GetRl()))*(CCstat[CID].GetRw()) / (CCstat[CID].GetNw() + CCstat[CID].GetSizeU2() + 1);
		Rl2.push_back(ServiceThr);
		Rw.push_back(ServiceRw);
		break;
	default:
		break;
	}
	return ServiceThr;
}

void Simulator::UpdateCarrierInfor(EventType et, UserType ut, int UID, int CID, double thrR, double thrA)  //更新载波信息
{
	if (et == Arrival)  //用户到达时刻更新，添加用户至相应载波
	{
		User NewUser;  //新用户信息
		LU	LUtemp;  //构建临时list
		switch (ut)
		{
		case U1:
			NewUser = { U1, UID, thrR, thrA };  //构造新的用户
			LUtemp = CCstat[CID].GetLU1();  //获取该载波上的用户信息list
			LUtemp.push_back(NewUser);  //放入新用户信息
			CCstat[CID].SetLU1(LUtemp);  //更新该载波上的用户信息list
			break;
		case U2:
			NewUser = { U2, UID, 0, thrA };
			LUtemp = CCstat[CID].GetLU2();
			LUtemp.push_back(NewUser);
			CCstat[CID].SetLU2(LUtemp);
			break;
		default:
			break;
		}
	}
	else  //用户离去时刻更新，在相应载波上删除特定用户信息
	{
		LU LUtemp;  //构建临时LU
		LU::iterator iter;  //list迭代器
		switch (ut)
		{
		case U1:
			LUtemp = CCstat[CID].GetLU1();  //拷贝该载波上的LU1
			iter = LUtemp.begin();  //迭代器初始位置
			while (iter != LUtemp.end())
			{
				if (iter->UserID == UID)				
					iter = LUtemp.erase(iter);
				else			
					iter++;
			}
			CCstat[CID].SetLU1(LUtemp);  //重置该载波上的LU1
			break;
		case U2:
			LUtemp = CCstat[CID].GetLU2();  //拷贝该载波上的LU2
			iter = LUtemp.begin();  //迭代器初始位置
			while (iter != LUtemp.end())
			{
				if (iter->UserID == UID)
					iter = LUtemp.erase(iter);
				else
					iter++;
			}
			CCstat[CID].SetLU2(LUtemp);  //重置该载波上的LU1
			break;
		default:
			break;
		}
	}
}

Simulator::Simulator()  //构造函数，初始化调度器
{
	cout << "请输入模拟的起始和终止时间" << endl;  //输入模拟所用到的参数，如仿真起始时间、用户到达均值和数据包大小范围等
	cin >> StartTime >> EndTime;
	cout << "输入用户到达的均值" << endl;
	cin >> lamda;
	cout << "输入数据包大小的分布范围，如(10 20):" << endl;
	cin >> PacketLow >> PacketHigh;

	int i;
	for (i = 0; i<NumCarriers; i++)  //初始化创建载波
	{
		LU L1, L2;  //初始化创建用户类型1和用户类型2的list，默认构造函数，初始为空
		Carrier CC(L1, L2, i, Nw[i], RlTot[i], RwTot[i]);
		CCstat.push_back(CC);
	}
	int ran = r.random(4);  //获取0-3之间的随机整数
	Event FirstEvent(StartTime, Arrival, U1, 1, 0, U1Thr[ran], 0, GetPacketLength());	//初始化第一个事件
	Queue.push(FirstEvent);  //将第一个事件加入事件队列
}

void Simulator::RunSimulation()  //执行模拟过程
{
	Event e, NewEvent;
	int NextUser;  //下一个用户号
	int NextTime;  //下一个用户的到达时间
	int SelectedCarrierIndex;  //所选择的成分载波号
	double ServiceThr;  //当前获得的吞吐量
	int ServiceTime;  //当前用户的服务时间		

	cout << "  时间  " << "事件类型 " << " 用户ID " << " 用户类型 " << "数据包长度" << "     服务状态 " << "     载波ID " << "   吞吐量需求大小" << "  实际吞吐量大小" << "     服务时间" << endl;  //输出相关信息

	while (!Queue.empty())
	{
		e = Queue.top();  //获取队列首部事件
		Queue.pop();  //弹出队列首部事件

		if (e.GetEventType() == Arrival)  //如果为到达事件
		{			
			EndTime = (EndTime < e.GetTime()) ? e.GetTime() : EndTime;  //判断仿真时间是否结束，若结束，则加上此次的超时			
			NextTime = e.GetTime() + NextArrivalTime(lamda);  //计算下次用户到达事件发生的时间
			if (EndTime < NextTime)  //若下一事件到达时间超过结束时间，则只处理当前事件，但不再产生新事件				
				continue;
			else
			{
				NumArrived++;  //更新到达的用户数目
				NextUser = e.GetUserID() + 1;  //为下一个用户产生到达事件并放入队列中
				double ran1 = r.frandom();  //生成一个随机数，用于判断下一次到达的用户类型
				if (ran1 <= Portion)  //下一个用户为类型1用户
				{
					int ran2 = r.random(4);  //获取0-3之间的随机整数					
					NewEvent = Event(NextTime, Arrival, U1, NextUser, 0, U1Thr[ran2], 0, GetPacketLength());  //生成新的LTE类型1用户到达事件
				}
				else  //下一个用户为类型2用户
				{
					NewEvent = Event(NextTime, Arrival, U2, NextUser, 0, 0, 0, GetPacketLength());  //生成新的LTE类型2用户到达事件
				}
				Queue.push(NewEvent);  //将新事件加入事件队列
			}
			SelectedCarrierIndex = SelectCarrier(e.GetUserType(), e.GetThrRequirment());  //获取给新到达用户所分配的载波
			if (SelectedCarrierIndex == NumCarriers + 1)  //若不存在合适的载波，则不更新载波操作和产生离去事件
			{
				NumRejected++;  //更新被拒绝的用户数目
				double Rw_temp = Rw.back();  //获取Rw尾部元素
				switch (e.GetUserType())
				{
				case U1:
					NumRejectedl1++;  //更新被拒绝的LTE类型1用户数目
					Rl1_A.push_back(0);  //存放到Rl1_A容器中进行统计
					Rl1_R.push_back(e.GetThrRequirment());  //存放到Rl1_R容器中进行统计				
					Rw.push_back(Rw_temp);  //复制上一次的数据放入容器
					break;
				case U2:
					NumRejectedl2++;  //更新被拒绝的LTE类型2用户数目
					Rl2.push_back(0);  
					Rw.push_back(Rw_temp);  
					break;
				default:
					break;
				}
				cout << setw(5) << e.GetTime() << "       " << e.GetEventType() << "       " << e.GetUserID() << "       " << e.GetUserType() << "       " << e.GetPacketLength() << "         " << "  拒绝接入  " << endl;  //输出相关信息
			}
			else  //若存在合适载波，则更新载波信息，计算服务时间，并产生离去事件
			{
				NumAccepted++;  //更新接受服务的用户数目
				ServiceThr = GetServiceThr(e.GetUserType(), SelectedCarrierIndex, e.GetThrRequirment());  //用户所能获得的实际吞吐量大小
				ServiceTime = int(e.GetPacketLength() / ServiceThr);  //用户的服务时间
				UpdateCarrierInfor(e.GetEventType(), e.GetUserType(), e.GetUserID(), SelectedCarrierIndex, e.GetThrRequirment(), ServiceThr);  //更新所分配的载波信息
				cout << setw(5) << e.GetTime() << "       " << e.GetEventType() << "       " << e.GetUserID() << "       " << e.GetUserType() << "       " << e.GetPacketLength() << "         " << "  接受接入  " << "      " << SelectedCarrierIndex << "             " << e.GetThrRequirment() << "             " << ServiceThr << "             " << ServiceTime << endl;  //输出相关信息
				NextTime = e.GetTime() + ServiceTime;  //计算用户离去事件发生的时间
				NewEvent = Event(NextTime, Departure, e.GetUserType(), e.GetUserID(), SelectedCarrierIndex, e.GetThrRequirment(), ServiceThr, e.GetPacketLength());  //生成用户离去事件
				Queue.push(NewEvent);  //将新事件加入事件队列
			}
		}

		else  //如果为离开事件
		{
			EndTime = (EndTime<e.GetTime()) ? e.GetTime() : EndTime;  //判断仿真时间是否结束，若结束，则加上此次的超时
			cout << setw(5) << e.GetTime() << "       " << e.GetEventType() << "       " << e.GetUserID() << "       " << e.GetUserType() << "       " << e.GetPacketLength() << "         " << "  服务结束  " << "      " << e.GetCarrierID() << "             " << e.GetThrRequirment() << "             " << e.GetThrActural() << "             " << ServiceTime << endl;  //输出相关信息
			UpdateCarrierInfor(e.GetEventType(), e.GetUserType(), e.GetUserID(), e.GetCarrierID(), e.GetThrRequirment(), 0);  //更新载波信息
		}
	}
}

void Simulator::PrintSimulation()  //输出仿真结果
{
	char filename[MAX];  //构建输出文件名字符串
	sprintf_s(filename, "E:\\C++项目\\CCS\\OutFile\\Result lamda %d - PacketLength %f-%f - Rwmin %f - Portion %f.txt", lamda, PacketLow, PacketHigh, Rwmin, Portion);  //设置输出文件文件名
	ofstream OutFile;  //创建输出文件流
	OutFile.open(filename);  //打开文件

	cout << endl;
	cout << "**************模拟结果***************" << endl;
	cout << "仿真起始时间:  " << StartTime << " " << EndTime << endl;
	cout << "用户到达均值:  " << lamda << endl;
	cout << "数据包大小范围:  " << PacketLow << " " << PacketHigh << endl;
	cout << "到达系统的总用户数:  " << NumArrived << endl;
	cout << "接受服务的总用户数:  " << NumAccepted << endl;
	cout << "拒绝服务的总用户数:  " << NumRejected << endl;
	cout << "接受服务的LTE类型1总用户数:  " << Rl1_A.size() << endl;
	cout << "拒绝接入的LTE类型1总用户数:  " << NumRejectedl1 << endl;
	cout << "接受服务的LTE类型2总用户数:  " << Rl2.size() << endl;	
	cout << "拒绝接入的LTE类型2总用户数:  " << NumRejectedl2 << endl;
	cout << endl;

	OutFile << endl;
	OutFile << "**************模拟结果***************" << endl;
	OutFile << "仿真起始时间:  " << StartTime << " " << EndTime << endl;
	OutFile << "用户到达均值:  " << lamda << endl;
	OutFile << "数据包大小范围:  " << PacketLow << " " << PacketHigh << endl;
	OutFile << "到达系统的总用户数:  " << NumArrived << endl;
	OutFile << "接受服务的总用户数:  " << NumAccepted << endl;
	OutFile << "拒绝服务的总用户数:  " << NumRejected << endl;
	OutFile << "接受服务的LTE类型1总用户数:  " << Rl1_A.size() << endl;
	OutFile << "拒绝接入的LTE类型1总用户数:  " << NumRejectedl1 << endl;
	OutFile << "接受服务的LTE类型2总用户数:  " << Rl2.size() << endl;
	OutFile << "拒绝接入的LTE类型2总用户数:  " << NumRejectedl2 << endl;
	OutFile << endl;

	double c1 = 1.0, a1 = 1.6, b1 = 0.0;
	double c2 = 1.0, a2 = 0.4, b2 = 0.6;
	double c3 = 0.2, a3 = 1.2, b3 = -0.6;  //Utility函数参数
	double Sum_CCS_UR1 = 0, Sum_CCS_UR2 = 0,Sum_CCS_UW = 0;  //Utility的和
	double UR1_temp = 0, UR2_temp = 0, UW_temp = 0;
	int i;
	for (i = 0; i < Rl1_A.size(); i++)
	{
		if (Rl1_A[i] <= Rl1_R[i])
			UR1_temp = a1 / (1 + exp(-c1*(Rl1_A[i] - Rl1_R[i]))) + b1;  //计算每一个类型1用户的utility值大小
		else
			UR1_temp = a2 / (1 + exp(-c2*(Rl1_A[i] - Rl1_R[i]))) + b2;
		Sum_CCS_UR1 += UR1_temp;
	}
	for (i = 0; i < Rl2.size(); i++)
	{
		UR2_temp = a3 / (1 + exp(-c3*Rl2[i])) + b3;  //计算每一个类型2用户的utility值大小
		Sum_CCS_UR2 += UR2_temp;
	}
	for ( i = 0; i < Rw.size(); i++)
	{
		if (Rw[i] >= Rwmin)
			UW_temp = 1.0;
		else
			UW_temp = 0.0;  //计算每一个WiFi用户的utility值大小
		Sum_CCS_UW += UW_temp;
	}
	double AvgUtility = (Sum_CCS_UR1 + Sum_CCS_UR2 + Sum_CCS_UW) / Rw.size();  //计算用户的平均utility值
	cout << "共存系统的平均Utility:  " << AvgUtility << endl;
	OutFile << "共存系统的平均Utility:  " << AvgUtility << endl;  //输出用户的平均utility
	cout << endl;
	OutFile << endl;

	vector<double>::iterator iter;  //定义vector迭代器

	cout << "LTE类型1用户实际获得的吞吐量:  " << endl;
	OutFile << "LTE类型1用户实际获得的吞吐量:  " << endl;
	for (iter = Rl1_A.begin();iter != Rl1_A.end(); iter++)
	{
		cout << *iter << endl;  //输出类型1用户实际吞吐量
		OutFile << *iter << endl;
	}
	cout << endl;
	OutFile << endl;

	cout << "LTE类型1用户需要获得的吞吐量:  " << endl;
	OutFile << "LTE类型1用户需要获得的吞吐量:  " << endl;
	for (iter = Rl1_R.begin(); iter != Rl1_R.end(); iter++)
	{
		cout << *iter << endl;  //输出类型1用户实际吞吐量
		OutFile << *iter << endl;
	}
	cout << endl;
	OutFile << endl;

	cout << "LTE类型2用户实际获得的吞吐量:  "  << endl;
	OutFile << "LTE类型2用户实际获得的吞吐量:  " << endl;
	for (iter = Rl2.begin(); iter != Rl2.end(); iter++)
	{
		cout << *iter << endl;  //输出类型2用户实际吞吐量
		OutFile << *iter << endl;
	}
	cout << endl;
	OutFile << endl;

	cout << "WiFi用户实际获得的吞吐量:  " << endl;
	OutFile << "WiFi用户实际获得的吞吐量:  " << endl;
	for (iter = Rw.begin(); iter != Rw.end(); iter++)
	{
		cout << *iter << endl;  //输出类型2用户实际吞吐量
		OutFile << *iter << endl;
	}	
}

#endif