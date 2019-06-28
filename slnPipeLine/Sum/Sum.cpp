// Sum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	float x,Sum;
	int i=0;
	char sTail[100];
	Sum=0;
	while(!cin.eof())
	{
		i++;
		cin >> x;
		if(cin.fail())
		{
			cerr << i <<':' << "Error" << endl;
			//cin.clear(0);
			if(cin.eof())
				break;
			cin.clear(0);
			cin.getline(sTail,100,'\n');
			continue;
		}
		cin.getline(sTail,100,'\n');
		Sum=Sum+x;
		//cout << i<<':'<<y << endl;
		//cout <<y << endl;
		
	};
	cout <<"Sum=" << sqrt(Sum)<<endl;
	return 0;
}

