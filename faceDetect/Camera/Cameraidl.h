

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Jul 25 11:54:06 2018
 */
/* Compiler settings for Camera.idl:
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


#ifndef __Cameraidl_h__
#define __Cameraidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DCamera_FWD_DEFINED__
#define ___DCamera_FWD_DEFINED__
typedef interface _DCamera _DCamera;
#endif 	/* ___DCamera_FWD_DEFINED__ */


#ifndef ___DCameraEvents_FWD_DEFINED__
#define ___DCameraEvents_FWD_DEFINED__
typedef interface _DCameraEvents _DCameraEvents;
#endif 	/* ___DCameraEvents_FWD_DEFINED__ */


#ifndef __Camera_FWD_DEFINED__
#define __Camera_FWD_DEFINED__

#ifdef __cplusplus
typedef class Camera Camera;
#else
typedef struct Camera Camera;
#endif /* __cplusplus */

#endif 	/* __Camera_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CameraLib_LIBRARY_DEFINED__
#define __CameraLib_LIBRARY_DEFINED__

/* library CameraLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_CameraLib;

#ifndef ___DCamera_DISPINTERFACE_DEFINED__
#define ___DCamera_DISPINTERFACE_DEFINED__

/* dispinterface _DCamera */
/* [uuid] */ 


EXTERN_C const IID DIID__DCamera;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B4F6D8B7-84AB-4759-A69C-872E1857A859")
    _DCamera : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCameraVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCamera * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCamera * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCamera * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCamera * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCamera * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCamera * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCamera * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DCameraVtbl;

    interface _DCamera
    {
        CONST_VTBL struct _DCameraVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCamera_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCamera_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCamera_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCamera_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCamera_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCamera_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCamera_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCamera_DISPINTERFACE_DEFINED__ */


#ifndef ___DCameraEvents_DISPINTERFACE_DEFINED__
#define ___DCameraEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DCameraEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DCameraEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BB502913-9A6D-4702-8A1D-8E2EEDD258DF")
    _DCameraEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCameraEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCameraEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCameraEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCameraEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCameraEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCameraEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCameraEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCameraEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DCameraEventsVtbl;

    interface _DCameraEvents
    {
        CONST_VTBL struct _DCameraEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCameraEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCameraEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCameraEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCameraEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCameraEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCameraEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCameraEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCameraEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Camera;

#ifdef __cplusplus

class DECLSPEC_UUID("942C47F3-62F8-454F-BC2C-CA6C5AE1CCD1")
Camera;
#endif
#endif /* __CameraLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


