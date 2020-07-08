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

#define MAX 1000  //�궨��һ�����ֵΪ1000
#define NumCarriers 5  //�궨���ز�����Ϊ5
#define Portion 0.8  //�궨������1�û�����Ϊ0.5
#define Rwmin 8.0  //�궨��WiFi������С����������Ϊ8.0
int Nw[NumCarriers] = { 5, 5, 5, 5, 5 };  //Ԥ���ظ��ز��ϵ�WiFi�����û������������õĽ���������������в������仯
double RlTot[NumCarriers] = { 175, 165, 155, 149, 154 };  //Ԥ���ظ��ز��ϵ�LTEϵͳ������������matlab����õ�
double RwTot[NumCarriers] = { 49, 55, 50, 59, 49 };  //Ԥ���ظ��ز��ϵ�WiFi����������������matlab����õ�
double U1Thr[4] = { 10, 20, 30, 40 };  //����LTE�û�����1�����������������4������ѡ��

using namespace std;

class Simulator
{
private:
	int StartTime, EndTime;  //ģ����ֹʱ��
	double PacketLow, PacketHigh;  //���ݰ���С�ķֲ���Χ�����ȷֲ�
	int lamda;  //�û������ֵ�����ɷֲ�
	int NumArrived = 0;  //ͳ�Ƶ�����û���	
	int NumAccepted = 0;  //ͳ�ƽ��ܷ�����û���
	int NumRejected = 0;  //ͳ�Ʊ��ܾ����û���
	int NumRejectedl1 = 0;  //ͳ�Ʊ��ܾ���LTE����1�û���
	int NumRejectedl2 = 0;  //ͳ�Ʊ��ܾ���LTE����2�û���

	vector<Carrier> CCstat;  //���ظ��ز���Ϣ������
	priority_queue<Event> Queue;  //���ظ��¼������ȼ�����
	vector<double> Rl1_A;  //����Rl1_A���ݵ�������LTE����1�û�ʵ�ʻ�õ�������ͳ��
	vector<double> Rl1_R;  //����Rl1_R���ݵ�������LTE����1�û���Ҫ��õ�������ͳ��
	vector<double> Rl2;  //����Rl2���ݵ�����
	vector<double> Rw;  //����Rw���ݵ�����
	RandomNumber r;  //�����

	int NextArrivalTime(int lamda);  //��ȡ��һ���û��ĵ���ʱ��		
	double GetPacketLength();  //��ȡ���ݰ���С
	int SelectCarrier(UserType ut, double thr);  //�ز�ѡ�����
	double GetServiceThr(UserType ut, int CID, double thr);  //�����û���õ�������
	void UpdateCarrierInfor(EventType et, UserType ut, int UID, int CID, double thrR, double thrA);  //�����ز���Ϣ			

public:
	Simulator();  //���캯������ʼ��������
	void RunSimulation();  //ִ�з������
	void PrintSimulation();  //���������
};

int Simulator::NextArrivalTime(int lamda)  //������һ���û��ĵ���ʱ���������Ӳ��ɷֲ�
{
	return (r.possion(lamda));
}

double Simulator::GetPacketLength()  //�������ݰ��Ĵ�С
{
	return (PacketLow + r.random(PacketHigh - PacketLow + 1));
}

int Simulator::SelectCarrier(UserType ut, double thr)  //�ز�ѡ�����
{
	int i = 0;
	int SelectedCarrierIndex = 0;  //��ѡ����ز��ţ�Ĭ��Ϊ0
	double Rl2ThrSelected = 0;  //��ѡ����ز�������2�û�ƽ��������
	double RwThrSelected = 0;  //��ѡ����ز���WiFi�û�ƽ��������
	vector<Carrier>::iterator iter;  //����һ�������ز���Ϣ��vector������
	switch (ut)
	{
	case U1:
		Rl2ThrSelected = (1 - (thr / CCstat[SelectedCarrierIndex].GetRl()) - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRl()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2());  //��������ѡ�е��ز�������2�û����ܻ�õĵ�ƽ��������
		RwThrSelected = (1 - (thr / CCstat[SelectedCarrierIndex].GetRl()) - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRw()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2());  //��������ѡ�е��ز���WiFi�û����ܻ�õĵ�ƽ��������
		if (RwThrSelected < Rwmin)
		{
			Rl2ThrSelected = 0;  //WiFi�û����ܱ���
		}
		for (iter = CCstat.begin(); iter != CCstat.end(); iter++)
		{

			double Rl2ThrCurrent = (1 - (thr / iter->GetRl()) - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRl()) / (iter->GetNw() + iter->GetSizeU2());  //�����ڴ��ز�������2�û����ܻ�õĵ�ƽ��������
			double RwThrCurrent = (1 - (thr / iter->GetRl()) - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRw()) / (iter->GetNw() + iter->GetSizeU2());  //�����ڴ��ز���WiFi�û����ܻ�õ�ƽ��������
			if (RwThrCurrent < Rwmin)
			{
				Rl2ThrCurrent = 0;  //WiFi�û����ܱ���
			}
			if (Rl2ThrCurrent > Rl2ThrSelected)
			{
				SelectedCarrierIndex = iter->GetCarrierID();
				Rl2ThrSelected = Rl2ThrCurrent;  //����ѡ���ز���Ϣ
			}
		}
		if (Rl2ThrSelected == 0)
		{
			SelectedCarrierIndex = NumCarriers + 1;  //�������ں��ʵ��ز����򷵻��ز���Ŀ��1
		}
		break;
	case U2:
		Rl2ThrSelected = (1 - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRl()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2() + 1);  //��������ѡ�е��ز�������2�û����ܻ�õĵ�ƽ��������
		RwThrSelected = (1 - (CCstat[SelectedCarrierIndex].GetSumThrU1() / CCstat[SelectedCarrierIndex].GetRl()))*(CCstat[SelectedCarrierIndex].GetRw()) / (CCstat[SelectedCarrierIndex].GetNw() + CCstat[SelectedCarrierIndex].GetSizeU2() + 1);  //��������ѡ�е��ز���WiFi�û����ܻ�õĵ�ƽ��������
		if (RwThrSelected < Rwmin)
		{
			Rl2ThrSelected = 0;  //WiFi�û����ܱ���
		}
		for (iter = CCstat.begin(); iter != CCstat.end(); iter++)
		{

			double Rl2ThrCurrent = (1 - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRl()) / (iter->GetNw() + iter->GetSizeU2() + 1);  //�����ڴ��ز�������2�û����ܻ�õĵ�ƽ��������
			double RwThrCurrent = (1 - (iter->GetSumThrU1() / iter->GetRl()))*(iter->GetRw()) / (iter->GetNw() + iter->GetSizeU2() + 1);  //�����ڴ��ز���WiFi�û����ܻ�õ�ƽ��������
			if (RwThrCurrent < Rwmin)
			{
				Rl2ThrCurrent = 0;  //WiFi�û����ܱ���
			}
			if (Rl2ThrCurrent > Rl2ThrSelected)
			{
				SelectedCarrierIndex = iter->GetCarrierID();
				Rl2ThrSelected = Rl2ThrCurrent;  //����ѡ���ز���Ϣ
			}
		}
		if (Rl2ThrSelected == 0)
		{
			SelectedCarrierIndex = NumCarriers + 1;  //�������ں��ʵ��ز����򷵻��ز���Ŀ��1
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
	double ServiceThr;  //LTE�û���õ�������
	double ServiceRw;  //WiFi�û�������
	switch (ut)
	{
	case U1:
		ServiceThr = thr;  //LTE����1�û�������
		ServiceRw = (1 - (thr / CCstat[CID].GetRl()) - (CCstat[CID].GetSumThrU1() / CCstat[CID].GetRl()))*(CCstat[CID].GetRw()) / (CCstat[CID].GetNw() + CCstat[CID].GetSizeU2());  //WiFi�û�������
		Rl1_A.push_back(ServiceThr);  //��ŵ�Rl1_A�����н���ͳ��
		Rl1_R.push_back(thr);  //��ŵ�Rl1_R�����н���ͳ��
		Rw.push_back(ServiceRw);  //��ŵ�Rw�����н���ͳ��
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

void Simulator::UpdateCarrierInfor(EventType et, UserType ut, int UID, int CID, double thrR, double thrA)  //�����ز���Ϣ
{
	if (et == Arrival)  //�û�����ʱ�̸��£�����û�����Ӧ�ز�
	{
		User NewUser;  //���û���Ϣ
		LU	LUtemp;  //������ʱlist
		switch (ut)
		{
		case U1:
			NewUser = { U1, UID, thrR, thrA };  //�����µ��û�
			LUtemp = CCstat[CID].GetLU1();  //��ȡ���ز��ϵ��û���Ϣlist
			LUtemp.push_back(NewUser);  //�������û���Ϣ
			CCstat[CID].SetLU1(LUtemp);  //���¸��ز��ϵ��û���Ϣlist
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
	else  //�û���ȥʱ�̸��£�����Ӧ�ز���ɾ���ض��û���Ϣ
	{
		LU LUtemp;  //������ʱLU
		LU::iterator iter;  //list������
		switch (ut)
		{
		case U1:
			LUtemp = CCstat[CID].GetLU1();  //�������ز��ϵ�LU1
			iter = LUtemp.begin();  //��������ʼλ��
			while (iter != LUtemp.end())
			{
				if (iter->UserID == UID)				
					iter = LUtemp.erase(iter);
				else			
					iter++;
			}
			CCstat[CID].SetLU1(LUtemp);  //���ø��ز��ϵ�LU1
			break;
		case U2:
			LUtemp = CCstat[CID].GetLU2();  //�������ز��ϵ�LU2
			iter = LUtemp.begin();  //��������ʼλ��
			while (iter != LUtemp.end())
			{
				if (iter->UserID == UID)
					iter = LUtemp.erase(iter);
				else
					iter++;
			}
			CCstat[CID].SetLU2(LUtemp);  //���ø��ز��ϵ�LU1
			break;
		default:
			break;
		}
	}
}

Simulator::Simulator()  //���캯������ʼ��������
{
	cout << "������ģ�����ʼ����ֹʱ��" << endl;  //����ģ�����õ��Ĳ������������ʼʱ�䡢�û������ֵ�����ݰ���С��Χ��
	cin >> StartTime >> EndTime;
	cout << "�����û�����ľ�ֵ" << endl;
	cin >> lamda;
	cout << "�������ݰ���С�ķֲ���Χ����(10 20):" << endl;
	cin >> PacketLow >> PacketHigh;

	int i;
	for (i = 0; i<NumCarriers; i++)  //��ʼ�������ز�
	{
		LU L1, L2;  //��ʼ�������û�����1���û�����2��list��Ĭ�Ϲ��캯������ʼΪ��
		Carrier CC(L1, L2, i, Nw[i], RlTot[i], RwTot[i]);
		CCstat.push_back(CC);
	}
	int ran = r.random(4);  //��ȡ0-3֮����������
	Event FirstEvent(StartTime, Arrival, U1, 1, 0, U1Thr[ran], 0, GetPacketLength());	//��ʼ����һ���¼�
	Queue.push(FirstEvent);  //����һ���¼������¼�����
}

void Simulator::RunSimulation()  //ִ��ģ�����
{
	Event e, NewEvent;
	int NextUser;  //��һ���û���
	int NextTime;  //��һ���û��ĵ���ʱ��
	int SelectedCarrierIndex;  //��ѡ��ĳɷ��ز���
	double ServiceThr;  //��ǰ��õ�������
	int ServiceTime;  //��ǰ�û��ķ���ʱ��		

	cout << "  ʱ��  " << "�¼����� " << " �û�ID " << " �û����� " << "���ݰ�����" << "     ����״̬ " << "     �ز�ID " << "   �����������С" << "  ʵ����������С" << "     ����ʱ��" << endl;  //��������Ϣ

	while (!Queue.empty())
	{
		e = Queue.top();  //��ȡ�����ײ��¼�
		Queue.pop();  //���������ײ��¼�

		if (e.GetEventType() == Arrival)  //���Ϊ�����¼�
		{			
			EndTime = (EndTime < e.GetTime()) ? e.GetTime() : EndTime;  //�жϷ���ʱ���Ƿ������������������ϴ˴εĳ�ʱ			
			NextTime = e.GetTime() + NextArrivalTime(lamda);  //�����´��û������¼�������ʱ��
			if (EndTime < NextTime)  //����һ�¼�����ʱ�䳬������ʱ�䣬��ֻ����ǰ�¼��������ٲ������¼�				
				continue;
			else
			{
				NumArrived++;  //���µ�����û���Ŀ
				NextUser = e.GetUserID() + 1;  //Ϊ��һ���û����������¼������������
				double ran1 = r.frandom();  //����һ��������������ж���һ�ε�����û�����
				if (ran1 <= Portion)  //��һ���û�Ϊ����1�û�
				{
					int ran2 = r.random(4);  //��ȡ0-3֮����������					
					NewEvent = Event(NextTime, Arrival, U1, NextUser, 0, U1Thr[ran2], 0, GetPacketLength());  //�����µ�LTE����1�û������¼�
				}
				else  //��һ���û�Ϊ����2�û�
				{
					NewEvent = Event(NextTime, Arrival, U2, NextUser, 0, 0, 0, GetPacketLength());  //�����µ�LTE����2�û������¼�
				}
				Queue.push(NewEvent);  //�����¼������¼�����
			}
			SelectedCarrierIndex = SelectCarrier(e.GetUserType(), e.GetThrRequirment());  //��ȡ���µ����û���������ز�
			if (SelectedCarrierIndex == NumCarriers + 1)  //�������ں��ʵ��ز����򲻸����ز������Ͳ�����ȥ�¼�
			{
				NumRejected++;  //���±��ܾ����û���Ŀ
				double Rw_temp = Rw.back();  //��ȡRwβ��Ԫ��
				switch (e.GetUserType())
				{
				case U1:
					NumRejectedl1++;  //���±��ܾ���LTE����1�û���Ŀ
					Rl1_A.push_back(0);  //��ŵ�Rl1_A�����н���ͳ��
					Rl1_R.push_back(e.GetThrRequirment());  //��ŵ�Rl1_R�����н���ͳ��				
					Rw.push_back(Rw_temp);  //������һ�ε����ݷ�������
					break;
				case U2:
					NumRejectedl2++;  //���±��ܾ���LTE����2�û���Ŀ
					Rl2.push_back(0);  
					Rw.push_back(Rw_temp);  
					break;
				default:
					break;
				}
				cout << setw(5) << e.GetTime() << "       " << e.GetEventType() << "       " << e.GetUserID() << "       " << e.GetUserType() << "       " << e.GetPacketLength() << "         " << "  �ܾ�����  " << endl;  //��������Ϣ
			}
			else  //�����ں����ز���������ز���Ϣ���������ʱ�䣬��������ȥ�¼�
			{
				NumAccepted++;  //���½��ܷ�����û���Ŀ
				ServiceThr = GetServiceThr(e.GetUserType(), SelectedCarrierIndex, e.GetThrRequirment());  //�û����ܻ�õ�ʵ����������С
				ServiceTime = int(e.GetPacketLength() / ServiceThr);  //�û��ķ���ʱ��
				UpdateCarrierInfor(e.GetEventType(), e.GetUserType(), e.GetUserID(), SelectedCarrierIndex, e.GetThrRequirment(), ServiceThr);  //������������ز���Ϣ
				cout << setw(5) << e.GetTime() << "       " << e.GetEventType() << "       " << e.GetUserID() << "       " << e.GetUserType() << "       " << e.GetPacketLength() << "         " << "  ���ܽ���  " << "      " << SelectedCarrierIndex << "             " << e.GetThrRequirment() << "             " << ServiceThr << "             " << ServiceTime << endl;  //��������Ϣ
				NextTime = e.GetTime() + ServiceTime;  //�����û���ȥ�¼�������ʱ��
				NewEvent = Event(NextTime, Departure, e.GetUserType(), e.GetUserID(), SelectedCarrierIndex, e.GetThrRequirment(), ServiceThr, e.GetPacketLength());  //�����û���ȥ�¼�
				Queue.push(NewEvent);  //�����¼������¼�����
			}
		}

		else  //���Ϊ�뿪�¼�
		{
			EndTime = (EndTime<e.GetTime()) ? e.GetTime() : EndTime;  //�жϷ���ʱ���Ƿ������������������ϴ˴εĳ�ʱ
			cout << setw(5) << e.GetTime() << "       " << e.GetEventType() << "       " << e.GetUserID() << "       " << e.GetUserType() << "       " << e.GetPacketLength() << "         " << "  �������  " << "      " << e.GetCarrierID() << "             " << e.GetThrRequirment() << "             " << e.GetThrActural() << "             " << ServiceTime << endl;  //��������Ϣ
			UpdateCarrierInfor(e.GetEventType(), e.GetUserType(), e.GetUserID(), e.GetCarrierID(), e.GetThrRequirment(), 0);  //�����ز���Ϣ
		}
	}
}

void Simulator::PrintSimulation()  //���������
{
	char filename[MAX];  //��������ļ����ַ���
	sprintf_s(filename, "E:\\C++��Ŀ\\CCS\\OutFile\\Result lamda %d - PacketLength %f-%f - Rwmin %f - Portion %f.txt", lamda, PacketLow, PacketHigh, Rwmin, Portion);  //��������ļ��ļ���
	ofstream OutFile;  //��������ļ���
	OutFile.open(filename);  //���ļ�

	cout << endl;
	cout << "**************ģ����***************" << endl;
	cout << "������ʼʱ��:  " << StartTime << " " << EndTime << endl;
	cout << "�û������ֵ:  " << lamda << endl;
	cout << "���ݰ���С��Χ:  " << PacketLow << " " << PacketHigh << endl;
	cout << "����ϵͳ�����û���:  " << NumArrived << endl;
	cout << "���ܷ�������û���:  " << NumAccepted << endl;
	cout << "�ܾ���������û���:  " << NumRejected << endl;
	cout << "���ܷ����LTE����1���û���:  " << Rl1_A.size() << endl;
	cout << "�ܾ������LTE����1���û���:  " << NumRejectedl1 << endl;
	cout << "���ܷ����LTE����2���û���:  " << Rl2.size() << endl;	
	cout << "�ܾ������LTE����2���û���:  " << NumRejectedl2 << endl;
	cout << endl;

	OutFile << endl;
	OutFile << "**************ģ����***************" << endl;
	OutFile << "������ʼʱ��:  " << StartTime << " " << EndTime << endl;
	OutFile << "�û������ֵ:  " << lamda << endl;
	OutFile << "���ݰ���С��Χ:  " << PacketLow << " " << PacketHigh << endl;
	OutFile << "����ϵͳ�����û���:  " << NumArrived << endl;
	OutFile << "���ܷ�������û���:  " << NumAccepted << endl;
	OutFile << "�ܾ���������û���:  " << NumRejected << endl;
	OutFile << "���ܷ����LTE����1���û���:  " << Rl1_A.size() << endl;
	OutFile << "�ܾ������LTE����1���û���:  " << NumRejectedl1 << endl;
	OutFile << "���ܷ����LTE����2���û���:  " << Rl2.size() << endl;
	OutFile << "�ܾ������LTE����2���û���:  " << NumRejectedl2 << endl;
	OutFile << endl;

	double c1 = 1.0, a1 = 1.6, b1 = 0.0;
	double c2 = 1.0, a2 = 0.4, b2 = 0.6;
	double c3 = 0.2, a3 = 1.2, b3 = -0.6;  //Utility��������
	double Sum_CCS_UR1 = 0, Sum_CCS_UR2 = 0,Sum_CCS_UW = 0;  //Utility�ĺ�
	double UR1_temp = 0, UR2_temp = 0, UW_temp = 0;
	int i;
	for (i = 0; i < Rl1_A.size(); i++)
	{
		if (Rl1_A[i] <= Rl1_R[i])
			UR1_temp = a1 / (1 + exp(-c1*(Rl1_A[i] - Rl1_R[i]))) + b1;  //����ÿһ������1�û���utilityֵ��С
		else
			UR1_temp = a2 / (1 + exp(-c2*(Rl1_A[i] - Rl1_R[i]))) + b2;
		Sum_CCS_UR1 += UR1_temp;
	}
	for (i = 0; i < Rl2.size(); i++)
	{
		UR2_temp = a3 / (1 + exp(-c3*Rl2[i])) + b3;  //����ÿһ������2�û���utilityֵ��С
		Sum_CCS_UR2 += UR2_temp;
	}
	for ( i = 0; i < Rw.size(); i++)
	{
		if (Rw[i] >= Rwmin)
			UW_temp = 1.0;
		else
			UW_temp = 0.0;  //����ÿһ��WiFi�û���utilityֵ��С
		Sum_CCS_UW += UW_temp;
	}
	double AvgUtility = (Sum_CCS_UR1 + Sum_CCS_UR2 + Sum_CCS_UW) / Rw.size();  //�����û���ƽ��utilityֵ
	cout << "����ϵͳ��ƽ��Utility:  " << AvgUtility << endl;
	OutFile << "����ϵͳ��ƽ��Utility:  " << AvgUtility << endl;  //����û���ƽ��utility
	cout << endl;
	OutFile << endl;

	vector<double>::iterator iter;  //����vector������

	cout << "LTE����1�û�ʵ�ʻ�õ�������:  " << endl;
	OutFile << "LTE����1�û�ʵ�ʻ�õ�������:  " << endl;
	for (iter = Rl1_A.begin();iter != Rl1_A.end(); iter++)
	{
		cout << *iter << endl;  //�������1�û�ʵ��������
		OutFile << *iter << endl;
	}
	cout << endl;
	OutFile << endl;

	cout << "LTE����1�û���Ҫ��õ�������:  " << endl;
	OutFile << "LTE����1�û���Ҫ��õ�������:  " << endl;
	for (iter = Rl1_R.begin(); iter != Rl1_R.end(); iter++)
	{
		cout << *iter << endl;  //�������1�û�ʵ��������
		OutFile << *iter << endl;
	}
	cout << endl;
	OutFile << endl;

	cout << "LTE����2�û�ʵ�ʻ�õ�������:  "  << endl;
	OutFile << "LTE����2�û�ʵ�ʻ�õ�������:  " << endl;
	for (iter = Rl2.begin(); iter != Rl2.end(); iter++)
	{
		cout << *iter << endl;  //�������2�û�ʵ��������
		OutFile << *iter << endl;
	}
	cout << endl;
	OutFile << endl;

	cout << "WiFi�û�ʵ�ʻ�õ�������:  " << endl;
	OutFile << "WiFi�û�ʵ�ʻ�õ�������:  " << endl;
	for (iter = Rw.begin(); iter != Rw.end(); iter++)
	{
		cout << *iter << endl;  //�������2�û�ʵ��������
		OutFile << *iter << endl;
	}	
}

#endif