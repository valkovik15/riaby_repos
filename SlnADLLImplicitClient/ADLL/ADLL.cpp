// ADLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ADLL.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// This is an example of an exported variable
ADLL_API int nADLL=0;

// This is an example of an exported function.
ADLL_API int fnADLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ADLL.h for the class definition
CADLL::CADLL(int loc)
{
	location=loc;
	switch (location) {
		case 0:
			printf("\nCADLL Creating on DATA\n");
			break;
		case 1:
			printf("\nCADLL Creating on STACK\n");
			break;
		
		case 2:
		default:
			printf("\nCADLL Creating on HEAP\n");
			break;
		
	}
	
	return;
}
CADLL::~CADLL()
{
	switch (location) {
		case 0:
			printf("\nCADLL Destroying on DATA\n");
			location=20;
			break;
		case 1:
			printf("\nCADLL Destroying on STACK\n");
			location=21;
			break;
		case 2:
			printf("\nCADLL Destroying on HEAP\n");
			location=22;
			break;
		case 20:
			printf("\nCADLL Destroying finally on DATA\n");	

			break;
		case 21:
			printf("\nCADLL Destroying finally on STACK\n");			 
			break;
		default:
			printf("\nCADLL Destroying finally\n");
			break;
	}
//	printf("\nCADLL Destroying...\n");
//	location = 3;
	return;
}
int CADLL::MyMethod(void){
	return 13;
}
