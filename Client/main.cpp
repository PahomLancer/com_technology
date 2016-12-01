#include <objbase.h>

#include <stdio.h>

#include "Interfaces.h"

//{91A42CAA-2577-4E80-934E-07DE64502FD6}
const CLSID CLSID_CA = {0x91A42CAA,0x2577,0x4E80,{0x93,0x4E,0x07,0xDE,0x64,0x50,0x2F,0xD6}};

//Manager emulator method
typedef HRESULT __stdcall (*GetClassObjectType) (const CLSID& clsid, const IID& iid, void** ppv);


int main()
{
    printf("Main::Start\n");

    int var = 0;
    var = 1; //Using manager emulator
    //var = 2; //Using COM library (factory)
    //var = 3; //Using COM library (instance)


    //Initializing COM library (Begin)
    CoInitialize(NULL);
    //Initializing COM library (End)


    try
    {

     IX* pX = NULL;
     IClassFactory* pCF = NULL;

     if (var!=3)
     {

      HRESULT resFactory;

      if (var==1)
      {
        //Getting manager method (Begin)
        GetClassObjectType GetClassObject;
        HINSTANCE h;
        h=LoadLibrary("../../../Manager/bin/Release/Manager.dll");
        if (!h)
        {
          throw "No manager";
        }
        GetClassObject=(GetClassObjectType) GetProcAddress(h,"GetClassObject");
        if (!GetClassObject)
        {
          throw "No manager method";
        }
        //Getting manager method (End)

        //Getting factory (Begin)
        resFactory = GetClassObject(CLSID_CA,IID_IClassFactory,(void**)&pCF);
        //Getting factory (End)
      }
      else
      {
        //Getting factory (Begin)
        resFactory = CoGetClassObject(CLSID_CA,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,(void**)&pCF);
        //Getting factory (End)
      }

      if (!(SUCCEEDED(resFactory)))
      {
         //printf("%X\n",(unsigned int)resFactory);
         throw "No factoty";
      }

     } // if need factory


     HRESULT resInstance;

     //Getting instance (Begin)
     if (var!=3)
     {
       resInstance = pCF->CreateInstance(NULL,IID_IX,(void**)&pX);
     }
     else
     {
       resInstance = CoCreateInstance(CLSID_CA,NULL,CLSCTX_INPROC_SERVER,IID_IX,(void**)&pX);
     }

     if (!(SUCCEEDED(resInstance)))
     {
          //printf("%X\n",(unsigned int)resInstance);
          throw "No instance";
     }
     //Getting instance (End)


     //Work (Begin)
     pX->Fx1();
     //Work (End)

     //Query (Begin)
     IY* pY = NULL;
     HRESULT resQuery = pX->QueryInterface(IID_IY,(void**)&pY);
     if (!(SUCCEEDED(resQuery)))
     {
          //printf("%X\n",(unsigned int)resQuery);
          throw "No query";
     }
     //Query (End)
    //Query (Begin)
     IZ* pZ = NULL;
     resQuery = pX->QueryInterface(IID_IZ,(void**)&pZ);
     if (!(SUCCEEDED(resQuery)))
     {
          //printf("%X\n",(unsigned int)resQuery);
          throw "No query";
     }

     //Work (Begin)
     pY->Fy1();
     pZ->Fz1();
     //Work (End)


     //Ref (Begin)
     IY* pY2 = pY;
     pY2->AddRef();
     pY2->Fy1();
     pY2->Release();
     IZ* pZ2 = pZ;
     pZ2->AddRef();
     pZ2->Fz1();
     pZ2->Release();
     //Ref (End)



     //Deleting (Begin)
     pY->Release();
     pZ->Release();
     pX->Release();
     if (var!=3)
     {
       pCF->Release();
     }
     //Deleting (End)

    }
    catch (const char* str)
    {
        printf("Main::Error: ");
        printf(str);
        printf("\n");
    }
    catch (...)
    {
        printf("Main::Error: Unknown\n");
    }


    //Uninitializing COM library (Begin)
    CoUninitialize();
    //Uninitializing COM library (End)

    printf("Main::Finish\n");
    return 0;
}
