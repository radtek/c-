

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jun 21 17:00:34 2018
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __imgTcpidl_h__
#define __imgTcpidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DimgTcp_FWD_DEFINED__
#define ___DimgTcp_FWD_DEFINED__
typedef interface _DimgTcp _DimgTcp;
#endif 	/* ___DimgTcp_FWD_DEFINED__ */


#ifndef ___DimgTcpEvents_FWD_DEFINED__
#define ___DimgTcpEvents_FWD_DEFINED__
typedef interface _DimgTcpEvents _DimgTcpEvents;
#endif 	/* ___DimgTcpEvents_FWD_DEFINED__ */


#ifndef __imgTcp_FWD_DEFINED__
#define __imgTcp_FWD_DEFINED__

#ifdef __cplusplus
typedef class imgTcp imgTcp;
#else
typedef struct imgTcp imgTcp;
#endif /* __cplusplus */

#endif 	/* __imgTcp_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __imgTcpLib_LIBRARY_DEFINED__
#define __imgTcpLib_LIBRARY_DEFINED__

/* library imgTcpLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_imgTcpLib;

#ifndef ___DimgTcp_DISPINTERFACE_DEFINED__
#define ___DimgTcp_DISPINTERFACE_DEFINED__

/* dispinterface _DimgTcp */
/* [uuid] */ 


EXTERN_C const IID DIID__DimgTcp;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("61F94D57-91C1-4332-8F4F-E13C90E20D5D")
    _DimgTcp : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DimgTcpVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DimgTcp * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DimgTcp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DimgTcp * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DimgTcp * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DimgTcp * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DimgTcp * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DimgTcp * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DimgTcpVtbl;

    interface _DimgTcp
    {
        CONST_VTBL struct _DimgTcpVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DimgTcp_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DimgTcp_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DimgTcp_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DimgTcp_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DimgTcp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DimgTcp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DimgTcp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DimgTcp_DISPINTERFACE_DEFINED__ */


#ifndef ___DimgTcpEvents_DISPINTERFACE_DEFINED__
#define ___DimgTcpEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DimgTcpEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DimgTcpEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BFD2A219-2DB9-4C4D-9D37-5DC02DE02C7B")
    _DimgTcpEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DimgTcpEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DimgTcpEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DimgTcpEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DimgTcpEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DimgTcpEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DimgTcpEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DimgTcpEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DimgTcpEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DimgTcpEventsVtbl;

    interface _DimgTcpEvents
    {
        CONST_VTBL struct _DimgTcpEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DimgTcpEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DimgTcpEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DimgTcpEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DimgTcpEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DimgTcpEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DimgTcpEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DimgTcpEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DimgTcpEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_imgTcp;

#ifdef __cplusplus

class DECLSPEC_UUID("285D5807-9271-4783-B737-75F1C551ADF2")
imgTcp;
#endif
#endif /* __imgTcpLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


