#ifndef RANDOMNUMBER_CLASS
#define RANDOMNUMBER_CLASS

#include <iostream>
#include <time.h>

using namespace std;

class RandomNumber  //定义随机数类
{
	public:
		RandomNumber(long s = 0);  //构造函数，初始化随机数类		
		long random();  //返回一个32-bit随机整数m, 1 <= m <= 2^31-2		
		long random(long n);  //返回一个32-bit随机整数m, 0 <= m <= n-1, 其中 n <= 2^31-1		
		double frandom();  //返回实数x, 0 <= x < 1
		int possion(int lamda);  //返回以lamda为均值的泊松分布随机数

	private:
		static const long A;
		static const long M;
		static const long Q;
		static const long R;
		long seed;  //随机数种子
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
		long t_time = time(NULL);  //获取系统时间
		t_time *= t_time;  //平方化，结果可能溢出，进行处理，> 0、 < 0、 = 0
		if (t_time > 0)
			s = t_time ^ 0x5EECE66DL;
		else if (t_time < 0)
			s = (t_time & 0x7fffffff) ^ 0x5EECE66DL;
		else
			s = 0x5EECE66DL;
	}
	seed = s;  //获得随机数种子
}

long RandomNumber::random()
{
	long tmpSeed = A * ( seed % Q ) - R * ( seed / Q );
	if( tmpSeed >= 0 )
		 seed = tmpSeed;
	else
		 seed = tmpSeed + M;
	return seed;  //获取随机数种子
}

long RandomNumber::random(long n)
{
	double fraction = double(random())/double(M);
	return int(fraction * n);  //获取0-n之间的随机数
}

double RandomNumber::frandom()
{
	return double(random())/double(M);  //获取0-1之间的随机实数，double型
}

int RandomNumber::possion(int lamda)  //返回以lamda为均值的泊松分布随机数
{
	int Lambda = lamda;
	int k = 0;
	long double p = 1.0;
	long double l = exp(double(-Lambda));  //为了精度，定义l为long double的，exp(-Lambda)是接近0的小数。对-Lambda强制类型转换为double，防止过重载无法识别
	while (p >= l)
	{
		double u = frandom();
		p *= u;
		k++;
	}
	return k - 1;
}

#endif