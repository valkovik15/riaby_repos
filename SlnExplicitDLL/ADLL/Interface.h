#include <objbase.h>
typedef int* pMyInt;
typedef int (*pMyFunc)();

interface IMyObject{
 
	virtual void ReadProperty(double&) PURE;
	virtual void WriteProperty(double)PURE ;
	virtual double MySin(double) PURE;
	virtual void Destroy() = 0;
};

/*
class  IMyObject{
public:
	virtual void ReadProperty(double&)=0;
	virtual void WriteProperty(double)=0;
	virtual double MySin(double)=0;
	virtual void Destroy()=0;
};
*/
typedef int (*pCreateObject)(IMyObject*&);
