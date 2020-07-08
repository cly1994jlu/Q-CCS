#include <iostream>
#include "Simulator.h"

using namespace std;

void new_line()
{
	char c;
	do{
		cin.get(c);
	} while (c != '\n');
}

int main(void)
{
	char c;
//	cout.setf(ios::fixed, ios::floatfield);
//	cout.precision(1);
	do{
		system("cls");
		Simulator temp;
		temp.RunSimulation();
		temp.PrintSimulation();
		cout<<"ÊÇ·ñ¼ÌÐøÖ´ÐÐ (Y/N)"<<endl;
		cin>>c;
		new_line();
	}while(c=='y'||c=='Y');
	return 0;
}