// Square.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>


using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	float x,y;
	int i=0;
	char sTail[100];
	
	ZeroMemory(sTail,sizeof(sTail));
	//cerr <<':' << "sizeof(sTail)" <<':'<< sizeof(sTail)<<sizeof(char)<< endl;
	
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

		y=x*x;
		
		cout <<y << endl;
		
	};
	
	return 0;
}

