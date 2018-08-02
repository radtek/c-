

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 02 10:33:37 2018
 */
/* Compiler settings for faceDetectOcx.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_faceDetectOcxLib,0x0E73220C,0x0A9F,0x41B5,0xB0,0x63,0x90,0x36,0x93,0x6A,0x27,0x9B);


MIDL_DEFINE_GUID(IID, DIID__DfaceDetectOcx,0xB42C8159,0x7C6C,0x42CF,0x8B,0x9E,0xAE,0x8E,0xEE,0x7E,0x8E,0x1D);


MIDL_DEFINE_GUID(IID, DIID__DfaceDetectOcxEvents,0xBAAEA834,0xE078,0x4CC0,0xA2,0xCF,0x37,0x86,0xC6,0x69,0x9F,0xAA);


MIDL_DEFINE_GUID(CLSID, CLSID_faceDetectOcx,0xF620116E,0xE336,0x4039,0xB5,0x05,0x95,0x3A,0xCC,0x37,0x13,0x63);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



