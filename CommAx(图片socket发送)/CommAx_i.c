

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jun 15 17:35:35 2018
 */
/* Compiler settings for CommAx.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_CommAxLib,0x174AB775,0x4743,0x4A1E,0xBF,0x7B,0xD5,0x70,0xBA,0xC9,0xB0,0xF3);


MIDL_DEFINE_GUID(IID, DIID__DCommAx,0x4F628536,0xE5DD,0x4DA0,0x83,0x41,0xE8,0xE4,0xB7,0xA2,0x2D,0x95);


MIDL_DEFINE_GUID(IID, DIID__DCommAxEvents,0x27131751,0x8E45,0x499F,0x90,0x3E,0x83,0x61,0xF2,0x66,0x7F,0x1A);


MIDL_DEFINE_GUID(CLSID, CLSID_CommAx,0x23BCC422,0x10AB,0x479A,0xB4,0xC7,0xC7,0x25,0x87,0x4B,0xB1,0x07);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



