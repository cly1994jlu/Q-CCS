#ifndef EVENT_CLASS
#define EVENT_CLASS

#include <iostream>

using namespace std;

enum EventType{Arrival,Departure};  //定义事件类型：到达与离去
enum UserType{U1,U2};  //定义用户类型：LTE-U类型1用户和类型2用户

class Event  //定义事件类
{
    private:
	    int Time;  //时间戳
		EventType Etype;  //事件类型
		UserType Utype;  //用户类型
		int UserID;  //用户ID
		int CarrierID;  //载波ID
		double ThrRequirment;  //用户吞吐量需求
		double ThrActural;  //用户实际吞吐量大小
		double PacketLength;  //数据包大小

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
		friend bool operator < (const Event& a, const Event& b);  //重载事件类的比较符号，依照时间戳对事件进行排序
};

Event::Event()  //事件的构造函数，默认初始化
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

Event::Event(int t, EventType et, UserType ut, int UID, int CID, double thrR, double thrA, double pkt)  //事件的构造函数，赋值初始化
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

int Event::GetTime()  //外部函数，获取事件的时间戳
{
	return Time;
}

EventType Event::GetEventType()  //外部函数，获取事件的事件类型
{
	return Etype;
}

UserType Event::GetUserType()  //外部函数，获取事件的用户类型
{
	return Utype;
}

int Event::GetUserID() //外部函数，获取事件的用户ID
{
	return UserID;
}

int Event::GetCarrierID()   //外部函数，获取事件的载波ID
{
	return CarrierID;
}

double Event::GetThrRequirment()  //外部函数，获取事件中的吞吐量需求
{
	return ThrRequirment;
}

double Event::GetThrActural()  ////外部函数，获取事件中的吞吐量需求
{
	return ThrActural;
}

double Event::GetPacketLength()  //外部函数，获取事件的数据包大小
{
	return PacketLength;
}

bool operator< (const Event& a, const  Event& b)  //重载事件类的比较符号，比较重要，将排序中的比较符号定义为按照类的事件类的时间戳排序，若时间相等，则比较用户ID
{
	if( a.Time < b.Time||(a.Time == b.Time&&a.UserID<b.UserID))
		return false;
	else return true;
}

#endif