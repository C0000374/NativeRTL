//
// Copyright (C) C0000374
//

#ifndef __NRTL_h__
#define __NRTL_h__
#include <NTExp.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(push)
#pragma warning(disable:4005)
#define STDCALL	    __stdcall
#define CDECL       __cdecl
#define NORETURN    __declspec(noreturn) VOID
#pragma warning(pop)

typedef float FLOAT32, * PFLOAT32;
typedef double FLOAT64, * PFLOAT64;
typedef const VOID* PCVOID;
typedef struct __NRTL_LIST* PNRTL_LIST;

typedef
BOOLEAN
(STDCALL* PNRTL_ALLOCFAIL_HANDLER) (
    IN UINT32 BlockSize,
    IN PVOID Block OPTIONAL
    );

typedef struct __NRTL_ILinkedList NRTL_ILinkedList, * PNRTL_ILinkedList;
struct __NRTL_ILinkedList {
    PNRTL_ILinkedList Previous;
    PNRTL_ILinkedList Next;
};

typedef struct __HASHMAP_USTR_KEY HASHMAP_USTR_KEY, * PHASHMAP_USTR_KEY;
struct __HASHMAP_USTR_KEY {
    UNICODE_STRING Key;
    UINT32 KeyHash; // crc32
};

//
// Heap.c
//

PNRTL_ALLOCFAIL_HANDLER
STDCALL
NrtlSetAllocFailHandler(
    IN PNRTL_ALLOCFAIL_HANDLER Handler
    );

PVOID
STDCALL
NrtlAllocateHeap(
    IN UINT32 Flags,
    IN UINT32 BlockSize
    );

PVOID
STDCALL
NrtlReAllocateHeap(
    IN UINT32 Flags,
    IN PVOID Block,
    IN UINT32 BlockSize
    );

VOID
STDCALL
NrtlFreeHeap(
    IN PVOID Block
    );

VOID
STDCALL
NrtlFreeLinkedList(
    IN PNRTL_ILinkedList FirstElement
    );

//
// PathConv.c
//

BOOLEAN
STDCALL
NrtlDosPathNameToNtPathName(
    IN PCWSTR DosPathName,
    OUT PUNICODE_STRING NtPathName
    );

//
// Printf.c
//

INT32
STDCALL
Nrtl_vsnprintf(
    IN PSTR Buffer,
    IN UINT32 BufferLength,
    IN PCSTR Format,
    IN va_list ArgList
    );

INT32
STDCALL
Nrtl_vsnwprintf(
    IN PWSTR Buffer,
    IN UINT32 BufferLength,
    IN PCWSTR Format,
    IN va_list ArgList
    );

INT32
CDECL
Nrtl_snprintf(
    IN PSTR Buffer,
    IN UINT32 BufferLength,
    IN PCSTR Format,
    IN ...
    );

INT32
CDECL
Nrtl_snwprintf(
    IN PWSTR Buffer,
    IN UINT32 BufferLength,
    IN PCWSTR Format,
    IN ...
    );

//
// List.c
//

BOOLEAN
STDCALL
NrtlCreateList(
    IN UINT32 ElementSize,
    OUT PNRTL_LIST* List_
    );

BOOLEAN
STDCALL
NrtlDestroyList(
    IN PNRTL_LIST List
    );

BOOLEAN
STDCALL
NrtlAddElementToList(
    IN PNRTL_LIST List,
    IN PVOID Element OPTIONAL
    );

BOOLEAN
STDCALL
NrtlGetListElementSize(
    IN PNRTL_LIST List,
    OUT PUINT32 ElementSize
    );

BOOLEAN
STDCALL
NrtlGetListSize(
    IN PNRTL_LIST List,
    OUT PUINT32 Size
    );

BOOLEAN
STDCALL
NrtlGetPtrToListElement(
    IN PNRTL_LIST List,
    IN UINT32 ElementIndex,
    OUT PVOID* Ptr_
    );

//
// HashMap_Ustr
//

BOOLEAN
STDCALL
NrtlCreateHashMapUstr(
    IN UINT32 ElementSize,
    OUT PNRTL_LIST* HashMap
    );

BOOLEAN
STDCALL
NrtlDestroyHashMapUstr(
    IN PNRTL_LIST HashMap
    );

BOOLEAN
STDCALL
NrtlAddElementToHashMapUstr(
    IN PNRTL_LIST HashMap,
    IN PUNICODE_STRING Key,
    IN PVOID Element OPTIONAL
    );

BOOLEAN
STDCALL
NrtlGetPtrToHashMapUstrElementByIndex(
    IN PNRTL_LIST HashMap,
    IN UINT32 Index,
    OUT PVOID* Ptr
    );

BOOLEAN
STDCALL
NrtlGetPtrToHashMapUstrElementByKey(
    IN PNRTL_LIST HashMap,
    IN PHASHMAP_USTR_KEY Key,
    OUT PVOID* Ptr
    );

BOOLEAN
STDCALL
NrtlGetPtrToHashMapUstrElementByUstr(
    IN PNRTL_LIST HashMap,
    IN PUNICODE_STRING String,
    OUT PVOID* Ptr
    );

BOOLEAN
STDCALL
NrtlCreateKeyForHashMapUstr(
    IN PUNICODE_STRING KeyStr,
    OUT PHASHMAP_USTR_KEY Key
    );

//
// inline
//

FORCEINLINE
VOID
NrtlInitializeLinkedList(
    OUT PNRTL_ILinkedList FirstListElement
    )
{
    FirstListElement->Previous = FirstListElement->Next = FirstListElement;
}

FORCEINLINE
VOID
NrtlInsertElementToLinkedList(
    IN PNRTL_ILinkedList PreviousElement,
    IN PNRTL_ILinkedList ElementToAdd
    )
{
    PreviousElement->Next->Previous = ElementToAdd;
    ElementToAdd->Next = PreviousElement->Next;
    ElementToAdd->Previous = PreviousElement;
    PreviousElement->Next = ElementToAdd;
}

FORCEINLINE
VOID
NrtlRemoveElementFromLinkedList(
    IN PNRTL_ILinkedList Element
    )
{
    Element->Previous->Next = Element->Next;
    Element->Next->Previous = Element->Previous;
}

FORCEINLINE
VOID
x86ZeroMemory(
    OUT PVOID Block,
    IN UINT32 Size_
    )
{
    __asm {
              push  edi
              mov   edi, Block
              mov   ecx, Size_
              xor   al, al
        rep   stosb
              pop   edi
    }
}

FORCEINLINE
VOID
x86FillMemory(
    OUT PVOID Block,
    IN UINT32 Size_,
    IN UINT8 Byte_
    )
{
    __asm {
              push  edi
              mov   edi, Block
              mov   ecx, Size_
              mov   al, Byte_
        rep   stosb
              pop   edi
    }
}

FORCEINLINE
VOID
x86CopyMemory(
    OUT PVOID Dest,
    IN PCVOID Src,
    IN UINT32 Size_
    )
{
    __asm {
              push  edi
              push  esi
              mov   edi, Dest
              mov   esi, Src
              mov   ecx, Size_
        rep   movsb
              pop   esi
              pop   edi
    }
}

#ifdef __cplusplus
}

PNRTL_ALLOCFAIL_HANDLER
STDCALL
NrtlSetNewHandler(
    IN PNRTL_ALLOCFAIL_HANDLER Handler
    );

PVOID
CDECL
operator new(
    IN UINT32 BlockSize
    );

PVOID
CDECL
operator new[](
    IN UINT32 BlockSize
    );

VOID
CDECL
operator delete(
    IN PVOID Block
    );

VOID
CDECL
operator delete[](
    IN PVOID Block
    );

VOID
CDECL
operator delete(
    IN PVOID Block,
    IN UINT32 BlockSize
    );

VOID
CDECL
operator delete[](
    IN PVOID Block,
    IN UINT32 BlockSize
    );
#endif

#endif