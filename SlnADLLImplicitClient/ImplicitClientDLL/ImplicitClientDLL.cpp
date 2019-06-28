// ImplicitClientDLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//On DATA
CADLL cStatic = CADLL(0);
int _tmain(int argc, _TCHAR* argv[])
{
	printf("\nStarting...\n");
	
	nADLL=123;
	printf("\nnADLL==%d\n",nADLL);

	printf("\nfnADLL()==%d\n",fnADLL());
// On HEAP
	CADLL* pCADLL = new CADLL(2);

	printf("\nOn HEAP: MyMethod()==%d\n",pCADLL->MyMethod() );

	delete [] pCADLL;
// On STACK
	CADLL cADLL = CADLL(1);
	printf("\nOn STACK: MyMethod()==%d\n",cADLL.MyMethod() );
//	cADLL.~CADLL();

//On DATA
	printf("\nOn DATA: MyMethod()==%d\n",cStatic.MyMethod() );
	
//	cStatic.~CADLL ();
	printf("\nFinishing...\n");
	getchar();
	return 0;
}

