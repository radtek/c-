

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __CommAxidl_h__
#define __CommAxidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DCommAx_FWD_DEFINED__
#define ___DCommAx_FWD_DEFINED__
typedef interface _DCommAx _DCommAx;
#endif 	/* ___DCommAx_FWD_DEFINED__ */


#ifndef ___DCommAxEvents_FWD_DEFINED__
#define ___DCommAxEvents_FWD_DEFINED__
typedef interface _DCommAxEvents _DCommAxEvents;
#endif 	/* ___DCommAxEvents_FWD_DEFINED__ */


#ifndef __CommAx_FWD_DEFINED__
#define __CommAx_FWD_DEFINED__

#ifdef __cplusplus
typedef class CommAx CommAx;
#else
typedef struct CommAx CommAx;
#endif /* __cplusplus */

#endif 	/* __CommAx_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CommAxLib_LIBRARY_DEFINED__
#define __CommAxLib_LIBRARY_DEFINED__

/* library CommAxLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_CommAxLib;

#ifndef ___DCommAx_DISPINTERFACE_DEFINED__
#define ___DCommAx_DISPINTERFACE_DEFINED__

/* dispinterface _DCommAx */
/* [uuid] */ 


EXTERN_C const IID DIID__DCommAx;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4F628536-E5DD-4DA0-8341-E8E4B7A22D95")
    _DCommAx : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCommAxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCommAx * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCommAx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCommAx * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCommAx * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCommAx * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCommAx * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCommAx * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DCommAxVtbl;

    interface _DCommAx
    {
        CONST_VTBL struct _DCommAxVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCommAx_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCommAx_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCommAx_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCommAx_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCommAx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCommAx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCommAx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCommAx_DISPINTERFACE_DEFINED__ */


#ifndef ___DCommAxEvents_DISPINTERFACE_DEFINED__
#define ___DCommAxEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DCommAxEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DCommAxEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("27131751-8E45-499F-903E-8361F2667F1A")
    _DCommAxEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCommAxEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCommAxEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCommAxEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCommAxEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCommAxEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCommAxEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCommAxEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCommAxEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DCommAxEventsVtbl;

    interface _DCommAxEvents
    {
        CONST_VTBL struct _DCommAxEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCommAxEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCommAxEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCommAxEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCommAxEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCommAxEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCommAxEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCommAxEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCommAxEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CommAx;

#ifdef __cplusplus

class DECLSPEC_UUID("23BCC422-10AB-479A-B4C7-C725874BB107")
CommAx;
#endif
#endif /* __CommAxLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


