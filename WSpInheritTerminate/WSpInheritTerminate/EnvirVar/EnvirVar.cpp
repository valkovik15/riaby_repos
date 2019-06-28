// EnvirVar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	printf("EnvirVar: Hello World!\n");

	DWORD dwPID = GetCurrentProcessId();
	char szPID[100];
	sprintf(szPID,"%d",dwPID);
	printf(szPID);
	printf("\n\n");
 

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,TRUE,dwPID);
	char szHANDLE[100];
	sprintf(szHANDLE,"%d",hProcess);
	printf(szHANDLE);
	printf("\n\n");

 
//===Set=up=two=variables================
    if(!SetEnvironmentVariable("PID_PARENT",szPID))
	{
		perror("!SetEnvironmentVariable(""PID_PARENT"",szPID)");
		return 1;
	};
	if(!SetEnvironmentVariable("HANDLE_PARENT",szHANDLE))
	{
		perror("!SetEnvironmentVariable(""HANDLE_PARENT"",szHANDLE)");
		return 1;
	};
//====Get=two=variables============================================
	char szValue[200];
	DWORD lenValue = GetEnvironmentVariable("PID_PARENT",szValue,sizeof(szValue));

	if(!lenValue){
		perror("The variable PID_PARENT was not found!");
		return 1;
	};
	printf("PID_PARENT = %s\n",szValue);
//-----------------------------------------------------------------------------
  lenValue = GetEnvironmentVariable("HANDLE_PARENT",szValue,sizeof(szValue));

	if(!lenValue){
		perror("The variable HANDLE_PARENT was not found!");
		return 1;
	};
	printf("HANDLE_PARENT = %s\n",szValue);

//======Delete the variable by name PID_PARENT==================================================
	 
	if(!SetEnvironmentVariable("PID_PARENT",NULL))
	{
		perror("!SetEnvironmentVariable(""PID_PARENT"",NULL)");
		return 1;
	};
// Try to get the value of PID_PARENT that it does not exist!
	 lenValue = GetEnvironmentVariable("PID_PARENT",szValue,sizeof(szValue));

	if(!lenValue){
		perror("The variable PID_PARENT was not found!");
		return 1;
	};
	printf("PID_PARENT = %s\n",szValue);

	return 0;
}

