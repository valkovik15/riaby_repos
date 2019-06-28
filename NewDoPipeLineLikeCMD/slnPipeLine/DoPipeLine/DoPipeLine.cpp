// DoPipeLine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//Verify argc==2
	if(argc!=2)
	{
		printf("Error: argc!=2\n");
		printf("Press the Key Enter to Exit...\n");
		getchar();
		return 0;
	}
	printf("argc:%d\n",argc);
	//Verify argv[1]="process0<streamIn 2>streamErr|process1>streamOut"
	wprintf(L"argv[1]=:%s\n",argv[1]);
	////wchar_t *wcschr(const wchar_t *str, wchar_t c )
	//_TCHAR* pSpace = wcschr(argv[1], ' ' );
	// 
	//wprintf(L"*(pSpace+2)=:%c\n",*(pSpace+2));
	////
	_TCHAR pr0[100];
	_TCHAR streamIn[100];
	_TCHAR streamErr[100];
	_TCHAR pr1[100];
	_TCHAR streamOut[100];
	_TCHAR less=0,two=0,op=0,greater1=0,greater2=0;
	int res = swscanf(argv[1],L"%[a-zA-Z0-9]%c%[a-zA-Z0-9] %c%c%[a-zA-Z0-9]%c%[a-zA-Z0-9]%c%[a-zA-Z0-9]",
		pr0,&less,streamIn,&two,&greater1,streamErr,&op,pr1,&greater2,streamOut);
	if(res == EOF)
	{
		printf("Error: res == EOF, an error or if the end of the string is reached before the first conversion\n");
		printf("Press the Key Enter to Exit...\n");
		getchar();
		return 0;

	}
	else if(res == 0)
	{
		printf("Error: res == 0, no fields were assigned\n");
		printf("Press the Key Enter to Exit...\n");
		getchar();
		return 0;

	}
	else if(res != 10)
	{

		printf("Error: res != 10, the number of fields successfully converted and assigned;res:%d\n",res);
		printf("Press the Key Enter to Exit...\n");
		getchar();
		return 0;

	}
	//res == 10, all fields. OK!
	
	if(less!='<'||two!='2'||greater1!='>'||greater2!='>')
	{
		wprintf(L"Error: see pipeline:\n%s\n",argv[1]);
		printf("Press the Key Enter to Exit...\n");
		getchar();
		return 0;
	}
	wprintf(L"PipeLine is correct:\n");
	wprintf(L"%s%c%s %c%c%s%c%s%c%s\n",pr0,less,streamIn,two,greater1,streamErr,op,pr1,greater2,streamOut);
	//
	
	_tcscat(streamIn,L".txt");
	_tcscat(streamErr,L".txt");
	_tcscat(streamOut,L".txt");
	wprintf(L"PipeLine is correct:\n");
	wprintf(L"%s%c%s %c%c%s%c%s%c%s\n",pr0,less,streamIn,two,greater1,streamErr,op,pr1,greater2,streamOut);
	printf("Press the Key Enter to Exit...\n");
		getchar();
		//return 0;
	
	//Create PipeLine
	//For Pipe
	HANDLE hWritePipe,hReadPipe;
	SECURITY_ATTRIBUTES saPipe=//For Inherited Descriptors
	{sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	//
	//For File "streamIn"
	SECURITY_ATTRIBUTES saFileIn=//For Inherited Descriptors
	{sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	//
	HANDLE hFileIn = CreateFile(streamIn,GENERIC_READ,0,
		&saFileIn,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFileIn == INVALID_HANDLE_VALUE) 
    { 
        wprintf(L"Could not open file <%s> (error %d)\n",streamIn, GetLastError());
		getchar();
        return 0;
    }
	//
	//For File "streamErr"
	SECURITY_ATTRIBUTES saFileErr=//For Inherited Descriptors
	{sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	//
	HANDLE hFileErr = CreateFile(streamErr,GENERIC_WRITE,0,
		&saFileErr,
		 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFileErr == INVALID_HANDLE_VALUE) 
    { 
        wprintf(L"Could not open file <%s> (error %d)\n",streamErr, GetLastError());
		getchar();
        return 0;
    }
	//
	//For File "streamOut"
	SECURITY_ATTRIBUTES saFileOut=//For Inherited Descriptors
	{sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
	//
	HANDLE hFileOut = CreateFile(streamOut,GENERIC_WRITE,0,
		&saFileOut,
		 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFileOut == INVALID_HANDLE_VALUE) 
    { 
        wprintf(L"Could not open file <%s> (error %d)\n",streamOut, GetLastError());
		getchar();
        return 0;
    }
	//
	STARTUPINFO startInfoProcess0, startInfoProcess1;

	//1)For Pipe
	
	BOOL bPipe = CreatePipe(&hReadPipe, &hWritePipe, &saPipe, 0);
	if (!bPipe) 
    { 
        wprintf(L"Could not create Pipe (error %d)\n", GetLastError());
		getchar();
        return 0;
    }

	//2)Get startInfoProcess0 from parent DoPipeLine.exe and replace STD_..._HANDLEs 
	GetStartupInfo(&startInfoProcess0);
	startInfoProcess0.hStdInput = hFileIn; //GetStdHandle(STD_INPUT_HANDLE);
	startInfoProcess0.hStdError = hFileErr;//GetStdHandle(STD_ERROR_HANDLE);
	startInfoProcess0.hStdOutput = hWritePipe;//GetStdHandle(STD_OUTPUT_HANDLE);
	startInfoProcess0.dwFlags=STARTF_USESTDHANDLES;

	//3)

	PROCESS_INFORMATION process0_Info, process1_Info;
	BOOL bPr0 = CreateProcess(NULL,(LPTSTR)pr0,
		NULL,NULL,
		TRUE, //pr0 inherits descriptors:hFileIn,hFileErr,hWritePipe
		0,//pr0 with NORMAL_PRIORITY_CLASS 
		NULL,//pr0 gets the environment of the calling process (parent DoPipeLine)
		NULL,//the new process will have the same current drive and directory as the calling process(parent DoPipeLine)
		&startInfoProcess0,
		&process0_Info);
	if (!bPr0) 
    { 
        wprintf(L"Could not create process <%s>(error %d)\n",pr0, GetLastError());
		getchar();
        return 0;
    }
	CloseHandle(hWritePipe);
	CloseHandle(hFileIn);
	CloseHandle(hFileErr);

	//4)Get startInfoProcess1 from parent DoPipeLine.exe and replace STD_..._HANDLEs 
	GetStartupInfo(&startInfoProcess1);
	startInfoProcess1.hStdInput = hReadPipe; //GetStdHandle(STD_INPUT_HANDLE);
	startInfoProcess1.hStdError = GetStdHandle(STD_ERROR_HANDLE);  //hFileErr
	startInfoProcess1.hStdOutput = hFileOut;//GetStdHandle(STD_OUTPUT_HANDLE);
	startInfoProcess1.dwFlags=STARTF_USESTDHANDLES;

	//5)
	BOOL bPr1 = CreateProcess(NULL,(LPTSTR)pr1,
		NULL,NULL,
		TRUE, //pr1 inherits descriptors::hFileOut,hReadPipe
		0,
		NULL,
		NULL,
		&startInfoProcess1,
		&process1_Info);
	if (!bPr1) 
    { 
        wprintf(L"Could not create process <%s>(error %d)\n",pr1, GetLastError());
		getchar();
        return 0;
    }
	CloseHandle(hReadPipe);
	CloseHandle(hFileOut);
	
	//6)
	WaitForSingleObject(process0_Info.hProcess,INFINITE);
	CloseHandle(process0_Info.hProcess);

	WaitForSingleObject(process1_Info.hProcess,INFINITE);
	CloseHandle(process1_Info.hProcess);

	printf("PipeLine complited its job!\n");
	printf("Press the Key Enter to Exit...\n");
	getchar();
	return 0;
}

