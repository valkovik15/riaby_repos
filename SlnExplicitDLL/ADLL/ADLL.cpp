// ADLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ADLL.h"
#include <windows.h>
#include <iostream>
#include <assert.h>
#include <math.h>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

CMyObject* pMyObject=NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					   )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			pMyObject=NULL;
			std::cout << "DLL_PROCESS_ATTACH" << std::endl;
			break;
		}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			std::cout << "DLL_PROCESS_DETACH" << std::endl;
			if (pMyObject!=NULL)
			{
				std::cout << "pMyObject!=NULL" << std::endl;
				delete[] pMyObject; //It is for other HEAP!
				pMyObject->~CMyObject();
				pMyObject=NULL;
				assert(pMyObject==NULL);
				
				break;
			}
			else
			{
				std::cout << "pMyObject=NULL" << std::endl;
				break;
			}
		}
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// This is an example of an exported variable
ADLL_API int nADLL=13;

// This is an example of an exported function.
ADLL_API int fnADLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ADLL.h for the class definition
CMyObject::CMyObject()
{
	data = 0.0;
	return;
}
CMyObject::~CMyObject()
{
}
void CMyObject::ReadProperty(double& d)
{
	d = data;
}
void CMyObject::WriteProperty(double d)
{
	data = d;
}
double CMyObject::MySin(double d)
{
	return data*sin(d);
}

void CMyObject::Destroy()
{

	this->~CMyObject();
	pMyObject=NULL;
	std::cout << "CMyObject::Destroy()" << std::endl;
}



ADLL_API int CreateMyObject(IMyObject*& MyObject)
{
	if (pMyObject!=NULL)
	{
		MyObject = (IMyObject*)pMyObject;
		std::cout << "CreateMyObject::Exists" << std::endl;
		return 0;
	}
	else
	{
		pMyObject = new CMyObject();
		MyObject = (IMyObject*)pMyObject;
		std::cout << "CreateMyObject::New" << std::endl;
		return 1;
	}
}
