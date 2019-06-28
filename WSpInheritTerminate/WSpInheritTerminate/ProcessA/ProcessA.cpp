// ProcessA.cpp : Defines the entry point for the application.
//
// Use of the C,s handle inheritance flag because B's handle is  inheritable.	 
// Use of command line to transmit both B,s handle and B,s PID.
// Use of OpenProcess to get B,s handle by B,s PID.
//
// So C need not use DuplicateHandle to get B's handle.
// C should use the first argument of command line
// because the B,s handle object is inheritable and
// the C,s handle inheritance flag is set to TRUE .
//

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	
   // Prepare a STARTUPINFO structure for spawning processes.
   STARTUPINFO         si = { sizeof(si) };
   SECURITY_ATTRIBUTES saProcess,
	                   saThread;
   PROCESS_INFORMATION piProcessB,   
	                   piProcessC;
   TCHAR szPath[MAX_PATH];

   // Prepare to spawn Process B from Process A.
// The handle identifying the new process object is to be INHERITABLE.

   saProcess.nLength              = sizeof(saProcess);
   saProcess.lpSecurityDescriptor = NULL;
   saProcess.bInheritHandle       = TRUE;  // TRUE means INHERITABLE //FALSE NOT INHERITABLE   

// The handle identifying the new thread object is NOT to be INHERITABLE.

   saThread.nLength              = sizeof(saThread);
   saThread.lpSecurityDescriptor = NULL;
   saThread.bInheritHandle       =FALSE;    //NOT INHERITABLE    //TRUE  means INHERITABLE

   // Spawn Process B.
//The handle returned by the CreateProcess function
// always  has PROCESS_ALL_ACCESS access to the process object.
//The current directory specified by the lpcurrentDirectory parameter
//    is the current directory for the child process.
//The current directory specified in item 2 under the lpCommandLine parameter
//    is the current directory for the parent process.

   lstrcpy(szPath, TEXT("ProcessB"));

// CREATE ProcessB
   if(!CreateProcess(NULL, szPath, //Command  line
	   &saProcess,  &saThread,
	   FALSE, //B inherits nothing from A, TRUE is suitable too.
	   0,	  //Creation Flags: priority , suspended, er. mode, detached console ...
	          //0 means NORMAL_PRIORITY_CLASS and not suspendid ... by default.
	   NULL,  //Env. block from A
	   NULL,  //Current dir. from A. So it is  defined by the command line in this call
	   &si,   // Specifies the main window appearence(by default in this call)
	   &piProcessB) )
   {
	MessageBox(NULL,TEXT("ProcessB failed"),TEXT("1:ProcessA"),MB_OK);
	return(0);
   };

//?????  CloseHandle(piProcessB.hProcess);
   CloseHandle(piProcessB.hThread);

// The pi structure contains two handles  relative to Process A:
   // pi.hProcess, which identifies Process B's process object and is  inheritable; 
   // pi.hThread,  which identifies Process B's primary thread object and is NOT inheritable.

// Prepare to spawn Process C from Process A.
   // Since NULL is passed for the " psaProcess and psaThread "
   // parameters, the handles to Process C's process and 
   // primary thread objects default to "noninheritable."

   // If Process A were to spawn another process, this new 
   // process would NOT inherit handles to Process C's process 
   // and thread objects.

   // Because TRUE is passed for the bInheritHandles parameter,
   // Process C will inherit all inheritable handles from A that is B's process handle.

    //A has piProcessB.hProcess and B's pid be handed in C 
   ///through a command line. The second argument is an extra information!

 
   sprintf( szPath, TEXT("ProcessC %i %i"), piProcessB.hProcess,
											piProcessB.dwProcessId );
 // CREATE ProcessC
   DWORD dwPID = piProcessB.dwProcessId;
   char szPID[100];
   sprintf(szPID, "%d", dwPID);
   HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwPID);
   char szHANDLE[100];
   sprintf(szHANDLE, "%d", piProcessB.hProcess);
   if (!SetEnvironmentVariable("PID_B", szPID))
   {
	   perror("!SetEnvironmentVariable(""PID_B"",szPID)");
	   return 1;
   };
   if (!SetEnvironmentVariable("HANDLE_B", szHANDLE))
   {
	   perror("!SetEnvironmentVariable(""HANDLE_B"",szHANDLE)");
	   return 1;
   };
   if(!CreateProcess(
	   NULL, szPath,

	   NULL, NULL,//BOTH C's HANDLES are NOT INHERITABLE
	   TRUE,// FALSE would mean that C has NOTHING inherit  from A 
	        // TRUE means C inherits B's handle!

	   NORMAL_PRIORITY_CLASS|CREATE_SUSPENDED,
	   NULL, //env not dividable
	   NULL,
	   &si, &piProcessC))
   { 
        MessageBox(NULL,TEXT("ProcessC failed"),TEXT("1:ProcessA"),MB_OK);
	    return(0);
   };
 	// TODO: Place code here.
   Sleep(5000);

//???? CloseHandle(piProcessB.hProcess); Here it is possible to close
//???? CloseHandle(piProcessB.hThread); Here it is possible to close

   if(1==ResumeThread(piProcessC.hThread))

   {
	Sleep(5000);
   
	MessageBox(NULL,TEXT("ProcessC resumed"),TEXT("1:ProcessA"),MB_OK);
   }

//Wait for Terminating Process B

    if(WAIT_FAILED==WaitForSingleObject(piProcessB.hProcess,INFINITE))
	{MessageBox(NULL,TEXT("WAIT_FAILED for ProcessB"),TEXT("2:ProcessA"),MB_OK);
    };
//Wait for Finishing Process C

    if(WAIT_FAILED==WaitForSingleObject(piProcessC.hProcess,INFINITE))
	{MessageBox(NULL,TEXT("WAIT_FAILED for ProcessC"),TEXT("2:ProcessA"),MB_OK);
    };

	CloseHandle(piProcessB.hProcess);
//	CloseHandle(piProcessB.hThread);

	CloseHandle(piProcessC.hProcess);
	CloseHandle(piProcessC.hThread);
	MessageBox(NULL,TEXT("ProcessA finished"),TEXT("2:ProcessA"),MB_OK);
	 
	return 0;
}



