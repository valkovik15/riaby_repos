// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ADLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ADLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "Interface.h"

#ifdef ADLL_EXPORTS
#define ADLL_API __declspec(dllexport)
#else
#define ADLL_API __declspec(dllimport)
#endif


// This class is exported from the ADLL.dll
class CMyObject : IMyObject {
	double data;
public:
	CMyObject(void);
	// TODO: add your methods here.
	virtual void ReadProperty(double&);
	virtual void WriteProperty(double);
	virtual double MySin(double);
	virtual void Destroy();
	virtual ~CMyObject();
};

extern "C" ADLL_API int nADLL;

extern "C" ADLL_API int fnADLL(void);

extern "C" ADLL_API int CreateMyObject(IMyObject*&);
