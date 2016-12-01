#include "interfaces.h"
#include <stdio.h>
#include "Components.h"

//*************************************************

class CA: public IX,IY,IZ
{
   private:
    int counter;
   public:
    CA();
    virtual ~CA();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual void __stdcall Fx1();
	virtual void __stdcall Fy1();
	virtual void __stdcall Fz1();
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

void __stdcall CA::Fx1()
{
   println("CA:Fx1");
}

void __stdcall CA::Fy1()
{
   println("CA:Fy1");
}

void __stdcall CA::Fz1()
{
   println("CA:Fz1");
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
