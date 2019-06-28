// ProcessC.cpp : Defines the entry point for the console application.
//
// It gets B's handle value through the first argument. 
// B's handle is inheritable. So there is its descriptor in C's table.
// It does not need to be duplicated but OpenProcess does that.
// The new B's handle and the inheritable one are different.
//
// C terminates B and closes A's and B's handles!
//

#include "stdafx.h"
#include <stdlib.h>
#include <conio.h>
#include <assert.h>
int main(int argc, char* argv[])
{
	if ( argc < 3 )// is equal to assert(argc >= 3);
	{
	 printf("\nHello World! from ProcessC :Error argc < 3\nPress any key to exit!\n");
	 getch();
	 return 0;
	}
	 
	char s[ 256 ];
	 sprintf( s, "ProcessC resumed by ProcessA, hProcessB = %s, B's PID = %s",
				argv[1], argv[2] );

	 MessageBox(NULL,TEXT(s),TEXT("1:ProcessC"),MB_OK);
	 //ENV variables extract
	 //====Get=two=variables============================================
	char szValue[200];
	DWORD lenValue = GetEnvironmentVariable("PID_B",szValue,sizeof(szValue));

	if(!lenValue){
		perror("The variable PID_PARENT was not found!");
		return 1;
	};
	printf("PID_PARENT = %s\n",szValue);
	DWORD  dwBProcessId = atoi(szValue);
//-----------------------------------------------------------------------------
  lenValue = GetEnvironmentVariable("HANDLE_B",szValue,sizeof(szValue));

	if(!lenValue){
		perror("The variable HANDLE_PARENT was not found!");
		return 1;
	};
	printf("HANDLE_PARENT = %s\n",szValue);
	 
	 HANDLE hargB        =(HANDLE)atoi( szValue );

	 HANDLE hProcessB = OpenProcess( PROCESS_DUP_HANDLE,
		                             FALSE,//FALSE means hProcessB is noninheritable
									dwBProcessId );
	 assert(hProcessB);
	 assert(hProcessB!=hargB);

	if( TerminateProcess(hargB,2)) //exit code 2	 
	 MessageBox(NULL,TEXT("Inherited ProcessB terminated"),TEXT("2:ProcessC"),MB_OK);	 
	else
	 MessageBox(NULL,TEXT("ProcessB failed to be terminated"),TEXT("2:ProcessC"),MB_OK);

	CloseHandle(hargB);
	CloseHandle(hProcessB);

	printf("\nHello World! from ProcessC\nPress any key to exit!\n");
	getch();
	return 0;	
}



