#ifndef RANDOMNUMBER_CLASS
#define RANDOMNUMBER_CLASS

#include <iostream>
#include <time.h>

using namespace std;

class RandomNumber  //�����������
{
	public:
		RandomNumber(long s = 0);  //���캯������ʼ���������		
		long random();  //����һ��32-bit�������m, 1 <= m <= 2^31-2		
		long random(long n);  //����һ��32-bit�������m, 0 <= m <= n-1, ���� n <= 2^31-1		
		double frandom();  //����ʵ��x, 0 <= x < 1
		int possion(int lamda);  //������lamdaΪ��ֵ�Ĳ��ɷֲ������

	private:
		static const long A;
		static const long M;
		static const long Q;
		static const long R;
		long seed;  //���������
};

const long RandomNumber::A = 48271;
const long RandomNumber::M = 2147483647;
const long RandomNumber::Q = M / A;
const long RandomNumber::R = M % A;

RandomNumber::RandomNumber(long s)
{
	if (s < 0)
		s = 0;
	if (s == 0)
	{
		long t_time = time(NULL);  //��ȡϵͳʱ��
		t_time *= t_time;  //ƽ���������������������д���> 0�� < 0�� = 0
		if (t_time > 0)
			s = t_time ^ 0x5EECE66DL;
		else if (t_time < 0)
			s = (t_time & 0x7fffffff) ^ 0x5EECE66DL;
		else
			s = 0x5EECE66DL;
	}
	seed = s;  //������������
}

long RandomNumber::random()
{
	long tmpSeed = A * ( seed % Q ) - R * ( seed / Q );
	if( tmpSeed >= 0 )
		 seed = tmpSeed;
	else
		 seed = tmpSeed + M;
	return seed;  //��ȡ���������
}

long RandomNumber::random(long n)
{
	double fraction = double(random())/double(M);
	return int(fraction * n);  //��ȡ0-n֮��������
}

double RandomNumber::frandom()
{
	return double(random())/double(M);  //��ȡ0-1֮������ʵ����double��
}

int RandomNumber::possion(int lamda)  //������lamdaΪ��ֵ�Ĳ��ɷֲ������
{
	int Lambda = lamda;
	int k = 0;
	long double p = 1.0;
	long double l = exp(double(-Lambda));  //Ϊ�˾��ȣ�����lΪlong double�ģ�exp(-Lambda)�ǽӽ�0��С������-Lambdaǿ������ת��Ϊdouble����ֹ�������޷�ʶ��
	while (p >= l)
	{
		double u = frandom();
		p *= u;
		k++;
	}
	return k - 1;
}

#endif