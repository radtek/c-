

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Aug 22 13:03:01 2018
 */
/* Compiler settings for imgTcp.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_imgTcpLib,0x65AC894F,0x4D99,0x4D16,0xBE,0xEC,0x5B,0x44,0x08,0xE5,0x62,0x8D);


MIDL_DEFINE_GUID(IID, DIID__DimgTcp,0x61F94D57,0x91C1,0x4332,0x8F,0x4F,0xE1,0x3C,0x90,0xE2,0x0D,0x5D);


MIDL_DEFINE_GUID(IID, DIID__DimgTcpEvents,0xBFD2A219,0x2DB9,0x4C4D,0x9D,0x37,0x5D,0xC0,0x2D,0xE0,0x2C,0x7B);


MIDL_DEFINE_GUID(CLSID, CLSID_imgTcp,0x285D5807,0x9271,0x4783,0xB7,0x37,0x75,0xF1,0xC5,0x51,0xAD,0xF2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



