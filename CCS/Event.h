#ifndef EVENT_CLASS
#define EVENT_CLASS

#include <iostream>

using namespace std;

enum EventType{Arrival,Departure};  //�����¼����ͣ���������ȥ
enum UserType{U1,U2};  //�����û����ͣ�LTE-U����1�û�������2�û�

class Event  //�����¼���
{
    private:
	    int Time;  //ʱ���
		EventType Etype;  //�¼�����
		UserType Utype;  //�û�����
		int UserID;  //�û�ID
		int CarrierID;  //�ز�ID
		double ThrRequirment;  //�û�����������
		double ThrActural;  //�û�ʵ����������С
		double PacketLength;  //���ݰ���С

    public:
		Event();
		Event(int t, EventType et, UserType ut, int UID, int CID, double thrR, double thrA, double pkt);
		int GetTime() ;
		EventType GetEventType() ;
		UserType GetUserType();
		int GetUserID() ;
		int GetCarrierID() ;
		double GetThrRequirment();
		double GetThrActural();
		double GetPacketLength();
		friend bool operator < (const Event& a, const Event& b);  //�����¼���ıȽϷ��ţ�����ʱ������¼���������
};

Event::Event()  //�¼��Ĺ��캯����Ĭ�ϳ�ʼ��
{
	Time = 0;
	Etype = Arrival;
	Utype = U1;
	UserID = 1;
	CarrierID = 0;
	ThrRequirment = 10;
	ThrActural = 0;
	PacketLength = 10;
}

Event::Event(int t, EventType et, UserType ut, int UID, int CID, double thrR, double thrA, double pkt)  //�¼��Ĺ��캯������ֵ��ʼ��
{
	Time = t;
	Etype = et;
	Utype = ut;
	UserID = UID;
	CarrierID = CID;
	ThrRequirment = thrR;
	ThrActural = thrA;
	PacketLength = pkt;
}

int Event::GetTime()  //�ⲿ��������ȡ�¼���ʱ���
{
	return Time;
}

EventType Event::GetEventType()  //�ⲿ��������ȡ�¼����¼�����
{
	return Etype;
}

UserType Event::GetUserType()  //�ⲿ��������ȡ�¼����û�����
{
	return Utype;
}

int Event::GetUserID() //�ⲿ��������ȡ�¼����û�ID
{
	return UserID;
}

int Event::GetCarrierID()   //�ⲿ��������ȡ�¼����ز�ID
{
	return CarrierID;
}

double Event::GetThrRequirment()  //�ⲿ��������ȡ�¼��е�����������
{
	return ThrRequirment;
}

double Event::GetThrActural()  ////�ⲿ��������ȡ�¼��е�����������
{
	return ThrActural;
}

double Event::GetPacketLength()  //�ⲿ��������ȡ�¼������ݰ���С
{
	return PacketLength;
}

bool operator< (const Event& a, const  Event& b)  //�����¼���ıȽϷ��ţ��Ƚ���Ҫ���������еıȽϷ��Ŷ���Ϊ��������¼����ʱ���������ʱ����ȣ���Ƚ��û�ID
{
	if( a.Time < b.Time||(a.Time == b.Time&&a.UserID<b.UserID))
		return false;
	else return true;
}

#endif