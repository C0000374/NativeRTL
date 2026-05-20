#ifndef __NRTL_h__
#define __NRTL_h__
#include <NTExp.h>

#pragma warning(push)
#pragma warning(disable:4005)

#define STDCALL	__stdcall
#define CDECL   __cdecl

#pragma warning(pop)

#define NRTL_RESOURCE_TYPE_FILE         0x80000000
#define NRTL_RESOURCE_TYPE_HEAP_MEMORY  0x80000001

typedef
BOOLEAN
(STDCALL* PNRTL_ALLOCFAIL_HANDLER) (
    IN UINT32 BlockSize,
    IN PVOID Block OPTIONAL
    );

typedef struct __NRTL_RESOURCES* PNRTL_RESOURCES;

typedef struct __NRTL_ILinkedList NRTL_ILinkedList, * PNRTL_ILinkedList;
struct __NRTL_ILinkedList {
    PNRTL_ILinkedList Previous;
    PNRTL_ILinkedList Next;
};

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

BOOLEAN
STDCALL
NrtlDosPathNameToNtPathName(
    IN PCWSTR DosPathName,
    OUT PUNICODE_STRING NtPathName
    );

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

#endif