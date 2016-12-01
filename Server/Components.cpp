#include "interfaces.h"
#include <stdio.h>
#include "Components.h"
#include <iostream>
#include "math.h"
#include <conio.h>

//*************************************************
//y'=e^x*y^2 - 2*y; y(0)=1/2; x=[0,2];
//h=0.1; yт=e^-2x/(1+e^-x)
//Функция f(x,y)
double funcd(double x, double y)
{
	double e = 2.7182;
	e = pow(e, x)*pow(y, 2) - 2 * y;
	return (e);
}
//Функция для расчета погрешости
double funct(double x)
{
	double e = 2.7182;
	e = pow(e, -2 * x) / (1 + pow(e, -x));
	return (e);
}

class CA: public IX,IY,IZ,IQ
{
   private:
    int counter;
   public:
    CA();
    virtual ~CA();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual void __stdcall Eyler(double a, double b);
	virtual void __stdcall RungeCut2(double a, double b);
	virtual void __stdcall RungeCut4(double a, double b);
	virtual void __stdcall Milna(double a, double b);
};

CA::CA()
{
  println("CA::CA constructor");
  counter = 0;
}

CA::~CA()
{
  println("CA::~CA destructor");
}


HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{
   println("CA::QueryInterface");

   if (iid==IID_IUnknown)
   {
      *ppv = (IUnknown*)(IX*)this;
   }
   else if (iid==IID_IX)
   {
      *ppv = (IX*)this;
   }
   else if (iid==IID_IY)
   {
      *ppv = (IY*)this;
   }
   else if (iid==IID_IZ)
   {
      *ppv = (IZ*)this;
   }
   else if (iid==IID_IQ)
   {
      *ppv = (IQ*)this;
   }
   else
   {
     *ppv = NULL;
     return E_NOINTERFACE;
   }

   this->AddRef();
   return S_OK;
}

ULONG __stdcall CA::AddRef()
{
   println("CA::AddRef");
   counter = counter + 1;
   return counter;
}

ULONG __stdcall CA::Release()
{
   println("CA::Release");
   counter = counter - 1;
   if (counter==0) {delete this;}
   return counter;
}

void __stdcall CA::Eyler(double a, double b)
{
    //y'=e^x*y^2 - 2*y; y(0)=1/2; x=[0,2];
    //h=0.1; yт=e^-2x/(1+e^-x)
    setlocale(LC_ALL, "Rus");
    println("CA:Eyler(a, b)");
    double h = 0.1;
	int n = (b - a) / h;
	//n = 20
	double *x = new double[n + 1]; // Выделение памяти для массива
	double *r = new double[n + 1]; // Выделение памяти для массива
	double *y = new double[n + 1]; // Выделение памяти для массива
	double *yt = new double[n + 1]; // Выделение памяти для массива
	//Расчет x
	x[0] = 0;
	for (int i = 1; i <= n + 1; i++){
		x[i] = x[0] + i*h;
	}
	for (int i = 0; i <= n + 1; i++){
		yt[i] = funct(x[i]);
	}
	//Расчет погрешности r
	//for (int i = 0; i <= n; i++){
	//	r[i] = 0.5*h*h*funct(x[i]);
	//}
	//Расчет y
	y[0] = 0.5;
	for (int i = 1; i <= n; i++){
		y[i] = y[i - 1] + h*funcd(x[i - 1], y[i - 1]);
	}
	printf("Метод Эйлера:\ndy = e^x*y^2 - 2*y\ny(0)=1/2; x=[%lf,%lf]; h = 0.1; n = 20;\n", a, b);
	for (int i = 0; i <= n; i++){
		printf("i=%d, x=%lf, yt=%lf, y=%lf\n", i, x[i], yt[i], y[i]);//, abs(yt[i] - y[i]));
	}
	delete[] x; //Удаление массива
	delete[] r; //Удаление массива
	delete[] y;	//Удаление массива
}

void __stdcall CA::RungeCut2(double a, double b)
{
    setlocale(LC_ALL, "Rus");
    println("CA:RungeCut2(a, b)");
	double h = 0.1;
	int n = (b - a) / h;
	//n = 20
	double *x = new double[n + 1]; // Выделение памяти для массива
	double *r = new double[n + 1]; // Выделение памяти для массива
	double *y = new double[n + 1]; // Выделение памяти для массива
	double *yt = new double[n + 1]; // Выделение памяти для массива
	//Расчет x
	x[0] = 0;
	for (int i = 1; i <= n + 1; i++){
		x[i] = x[0] + i*h;
	}
	for (int i = 0; i <= n + 1; i++){
		yt[i] = funct(x[i]);
	}
	//Расчет y
	y[0] = 0.5;
	for (int i = 1; i <= n; i++){
		y[i] = y[i - 1] + h*funcd(x[i - 1], y[i - 1]);
	}
    float k1, k2, k3, k4;
	for (int i = 0; i<n; ++i)
	{
		k1 = h * funcd(x[i], y[i]);
		k2 = h * funcd(x[i] + h, y[i] + k1);
		y[i + 1] = y[i] + 0.5*(k1 + k2);
	}
	printf("Метод Рунге-Кутта 2-порядка:\ndy = e^x*y^2 - 2*y\ny(0)=1/2; x=[%lf,%lf]; h = 0.1; n = 20;\n", a, b);
	for (int i = 0; i <= n; i++){
		printf("i=%d, x=%lf, yt=%lf, y=%lf\n", i, x[i], yt[i], y[i]);//, abs(yt[i] - y[i]));
	}
	delete[] x; //Удаление массива
	delete[] r; //Удаление массива
	delete[] y;	//Удаление массива
}

void __stdcall CA::RungeCut4(double a, double b)
{
    setlocale(LC_ALL, "Rus");
    println("CA:RungeCut4(a, b)");
	double h = 0.1;
	int n = (b - a) / h;
	//n = 20
	double *x = new double[n + 1]; // Выделение памяти для массива
	double *r = new double[n + 1]; // Выделение памяти для массива
	double *y = new double[n + 1]; // Выделение памяти для массива
	double *yt = new double[n + 1]; // Выделение памяти для массива
	//Расчет x
	x[0] = 0;
	for (int i = 1; i <= n + 1; i++){
		x[i] = x[0] + i*h;
	}
	for (int i = 0; i <= n + 1; i++){
		yt[i] = funct(x[i]);
	}
	//Расчет y
	y[0] = 0.5;
	for (int i = 1; i <= n; i++){
		y[i] = y[i - 1] + h*funcd(x[i - 1], y[i - 1]);
	}
    float k1, k2, k3, k4;
	for (int i = 0; i<n; ++i)
	{
		k1 = h * funcd(x[i], y[i]);
		k2 = h * funcd(x[i] + h, y[i] + k1);
		y[i + 1] = y[i] + 0.5*(k1 + k2);
	}
	for (int i = 0; i<=n; ++i)
	{
		k1 = h * funcd(x[i], y[i]);
		k2 = h * funcd(x[i] + h / 2, y[i] + k1 / 2);
		k3 = h * funcd(x[i] + h / 2, y[i] + k2 / 2);
		k4 = h * funcd(x[i] + h, y[i] + k3);
		y[i + 1] = y[i] + 0.16666*(k1 + 2 * k2 + 2 * k3 + k4);
	}
	printf("Метод Рунге-Кутта 4-порядка:\ndy = e^x*y^2 - 2*y\ny(0)=1/2; x=[%lf,%lf]; h = 0.1; n = 20;\n", a, b);
	for (int i = 0; i <= n; i++){
		printf("i=%d, x=%lf, yt=%lf, y=%lf\n", i, x[i], yt[i], y[i]);//, abs(yt[i]-y[i]));
	}
	delete[] x; //Удаление массива
	delete[] r; //Удаление массива
	delete[] y;	//Удаление массива
}

void __stdcall CA::Milna(double a, double b)
{
    setlocale(LC_ALL, "Rus");
    println("CA:Milna(a, b)");
	double h = 0.1;
	int n = (b - a) / h;
	//n = 20
	double *x = new double[n + 1]; // Выделение памяти для массива
	double *r = new double[n + 1]; // Выделение памяти для массива
	double *y = new double[n + 1]; // Выделение памяти для массива
	double *yt = new double[n + 1]; // Выделение памяти для массива
	//Расчет x
	x[0] = 0;
	for (int i = 1; i <= n + 1; i++){
		x[i] = x[0] + i*h;
	}
	for (int i = 0; i <= n + 1; i++){
		yt[i] = funct(x[i]);
	}
	//Расчет y
	y[0] = 0.5;
	for (int i = 1; i <= n; i++){
		y[i] = y[i - 1] + h*funcd(x[i - 1], y[i - 1]);
	}
    float k1, k2, k3, k4;
	for (int i = 0; i<n; ++i)
	{
		k1 = h * funcd(x[i], y[i]);
		k2 = h * funcd(x[i] + h, y[i] + k1);
		y[i + 1] = y[i] + 0.5*(k1 + k2);
	}
	for (int i = 0; i<=n; ++i)
	{
		k1 = h * funcd(x[i], y[i]);
		k2 = h * funcd(x[i] + h / 2, y[i] + k1 / 2);
		k3 = h * funcd(x[i] + h / 2, y[i] + k2 / 2);
		k4 = h * funcd(x[i] + h, y[i] + k3);
		y[i + 1] = y[i] + 0.16666*(k1 + 2 * k2 + 2 * k3 + k4);
	}
    //Метод Милна
	for (int i = 4; i <= n; i++){
		y[i] = y[i - 4] + 4 * h / 3 * (2 * funcd(x[i - 3], y[i - 3]) - funcd(x[i - 2], y[i - 2]) + 2 * funcd(x[i - 1], y[i - 1]));
		//y[i] = y[i - 2] + 4 * h / 3 * (funcd(x[i - 2], y[i - 2]) - 4 * funcd(x[i - 1], y[i - 1]) + funcd(x[i], y[i]));
	}
	printf("Метод Милна:\ndy = e^x*y^2 - 2*y\ny(0)=1/2; x=[%lf,%lf]; h = 0.1; n = 20;\n", a, b);
	for (int i = 0; i <= n; i++){
		printf("i=%d, x=%lf, yt=%lf, y=%lf\n", i, x[i], yt[i], y[i]);//, abs(yt[i] - y[i]));
	}
	delete[] x; //Удаление массива
	delete[] r; //Удаление массива
	delete[] y;	//Удаление массива
}
//*************************************************

class CFA: public IClassFactory
{
    private:
     int counter;
    public:
      CFA();
      virtual ~CFA();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);
};

CFA::CFA()
{
  println("CFA::CFA");
  counter = 0;
}

CFA::~CFA()
{
  println("CFA::~CFA");
}


HRESULT __stdcall CFA::QueryInterface(const IID& iid, void** ppv)
{
   println("CFA::QueryInterface");

   if (iid==IID_IUnknown)
   {
     *ppv = (IUnknown*)(IClassFactory*)this;
   }
   else if (iid==IID_IClassFactory)
   {
     *ppv = (IClassFactory*)this;
   }
   else
   {
     *ppv = NULL;
     return E_NOINTERFACE;
   }
   this->AddRef();
   return S_OK;
}

ULONG __stdcall CFA::AddRef()
{
   println("CFA::AddRef");
   counter = counter + 1;
   return counter;
}

ULONG __stdcall CFA::Release()
{
   println("CFA::Release");
   counter = counter - 1;
   if (counter==0) {delete this;}
   return counter;
}


HRESULT __stdcall CFA::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv)
{
   println("CFA::CreateInstance");
   if (pUnknownOuter!=NULL)
   {
      return E_NOTIMPL;
   }

   CA* a = new CA();
   return a->QueryInterface(iid,ppv);
}

HRESULT __stdcall CFA::LockServer(BOOL bLock)
{
  println("CFA::LockServer");
  return S_OK;
}

//*************************************************


void println(const char* str)
{
    printf(str);
    printf("\n");
}


//{91A42CAA-2577-4E80-934E-07DE64502FD6}
const CLSID CLSID_CA = {0x91A42CAA,0x2577,0x4E80,{0x93,0x4E,0x07,0xDE,0x64,0x50,0x2F,0xD6}};

HRESULT __stdcall GetClassObject(const CLSID& clsid, const IID& iid, void** ppv)
{
   println("Component::GetClassObject");
   if (clsid==CLSID_CA)
   {
      CFA* fa  = new CFA();
      return fa->QueryInterface(iid,ppv);
   }
   else
   {
     *ppv = NULL;
     return E_NOTIMPL;
   }

}

//*************************************************
