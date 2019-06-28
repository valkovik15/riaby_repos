// Instances.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <conio.h>

//Начало определения разделяемой секции данных(здесь только одна перем.)
#pragma data_seg("SharedSection")
	int ApplicationInstances=0;
#pragma data_seg()

#pragma comment(linker,"/Section:SharedSection,RWS")
//Конец определения разделяемой секции данных

	int CurrentNumber;
 

int main(int argc, char* argv[])
{
 CurrentNumber= InterlockedExchangeAdd (
                   (PLONG) &ApplicationInstances,  // pointer to the addend PLONG
                    1 // increment value
                     );
 bool t = false;

	while(!t){	 
        CurrentNumber= InterlockedExchangeAdd ((PLONG) &ApplicationInstances, 0 ); 
		printf("\nApplicationInstances==%d\n",CurrentNumber);
     
	  printf("\nPress Key 'x' to exit or \n one different from 'x' to continue\n");
        t=int('x') == getch() ;
    }
 InterlockedExchangeAdd ((PLONG) &ApplicationInstances, -1  
                     );
	return 0;
}

