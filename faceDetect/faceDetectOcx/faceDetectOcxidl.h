

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __faceDetectOcxidl_h__
#define __faceDetectOcxidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DfaceDetectOcx_FWD_DEFINED__
#define ___DfaceDetectOcx_FWD_DEFINED__
typedef interface _DfaceDetectOcx _DfaceDetectOcx;
#endif 	/* ___DfaceDetectOcx_FWD_DEFINED__ */


#ifndef ___DfaceDetectOcxEvents_FWD_DEFINED__
#define ___DfaceDetectOcxEvents_FWD_DEFINED__
typedef interface _DfaceDetectOcxEvents _DfaceDetectOcxEvents;
#endif 	/* ___DfaceDetectOcxEvents_FWD_DEFINED__ */


#ifndef __faceDetectOcx_FWD_DEFINED__
#define __faceDetectOcx_FWD_DEFINED__

#ifdef __cplusplus
typedef class faceDetectOcx faceDetectOcx;
#else
typedef struct faceDetectOcx faceDetectOcx;
#endif /* __cplusplus */

#endif 	/* __faceDetectOcx_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __faceDetectOcxLib_LIBRARY_DEFINED__
#define __faceDetectOcxLib_LIBRARY_DEFINED__

/* library faceDetectOcxLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_faceDetectOcxLib;

#ifndef ___DfaceDetectOcx_DISPINTERFACE_DEFINED__
#define ___DfaceDetectOcx_DISPINTERFACE_DEFINED__

/* dispinterface _DfaceDetectOcx */
/* [uuid] */ 


EXTERN_C const IID DIID__DfaceDetectOcx;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B42C8159-7C6C-42CF-8B9E-AE8EEE7E8E1D")
    _DfaceDetectOcx : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DfaceDetectOcxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DfaceDetectOcx * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DfaceDetectOcx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DfaceDetectOcx * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DfaceDetectOcx * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DfaceDetectOcx * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DfaceDetectOcx * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DfaceDetectOcx * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DfaceDetectOcxVtbl;

    interface _DfaceDetectOcx
    {
        CONST_VTBL struct _DfaceDetectOcxVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DfaceDetectOcx_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DfaceDetectOcx_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DfaceDetectOcx_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DfaceDetectOcx_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DfaceDetectOcx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DfaceDetectOcx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DfaceDetectOcx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DfaceDetectOcx_DISPINTERFACE_DEFINED__ */


#ifndef ___DfaceDetectOcxEvents_DISPINTERFACE_DEFINED__
#define ___DfaceDetectOcxEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DfaceDetectOcxEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DfaceDetectOcxEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BAAEA834-E078-4CC0-A2CF-3786C6699FAA")
    _DfaceDetectOcxEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DfaceDetectOcxEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DfaceDetectOcxEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DfaceDetectOcxEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DfaceDetectOcxEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DfaceDetectOcxEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DfaceDetectOcxEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DfaceDetectOcxEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DfaceDetectOcxEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DfaceDetectOcxEventsVtbl;

    interface _DfaceDetectOcxEvents
    {
        CONST_VTBL struct _DfaceDetectOcxEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DfaceDetectOcxEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DfaceDetectOcxEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DfaceDetectOcxEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DfaceDetectOcxEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DfaceDetectOcxEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DfaceDetectOcxEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DfaceDetectOcxEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DfaceDetectOcxEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_faceDetectOcx;

#ifdef __cplusplus

class DECLSPEC_UUID("F620116E-E336-4039-B505-953ACC371363")
faceDetectOcx;
#endif
#endif /* __faceDetectOcxLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


