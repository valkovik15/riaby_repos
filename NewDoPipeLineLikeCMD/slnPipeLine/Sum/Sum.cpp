// Sum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
		float x,Sum;
	int i=0;
	char sTail[100];
	ZeroMemory(sTail,sizeof(sTail));

	Sum=0;
	while(!cin.eof())
	{
		
		i++;
		sTail[0]=0;
		cin >> x;

		if(cin.fail())
		{
			
			if(cin.eof())
				break;			
			cin.clear(0);

			cin.getline(sTail,100,'\n');
			//cerr << i <<':' << "Error" <<':'<<(int)(sTail[0])<< endl;
			cerr << i <<':' << "Error" <<':'<< sTail<< endl;


			continue;
		}

		cin.getline(sTail,100,'\n');
		if(sTail[0]!=0)
		{
			//cerr << i <<':' << "Error" <<':'<<(int)(sTail[0])<< endl;
			cerr << i <<':' << "Error" <<':'<< sTail<< endl;

			continue;
		}


		Sum=Sum+x;
		//cout << i<<':'<<y << endl;
		//cout <<y << endl;
		
	};
	cout <<"Sum=" << Sum<<endl;
	return 0;
}

