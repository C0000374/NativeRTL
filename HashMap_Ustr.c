//
// Copyright (C) C0000374
//

#include <NRTLp.h>

UINT32
STDCALL
NrtlpComputeStringHash(
    PUNICODE_STRING String
    )
{
    return RtlComputeCrc32(0, String->Buffer, String->Length);
}

BOOLEAN
STDCALL
NrtlCreateKeyForHashMapUstr(
    IN PUNICODE_STRING KeyStr,
    OUT PHASHMAP_USTR_KEY Key
    )
{
    BOOLEAN bStatus;

    bStatus = RtlCreateUnicodeString(&Key->Key, KeyStr->Buffer);
    if (!bStatus) return FALSE;

    Key->KeyHash = NrtlpComputeStringHash(&Key->Key);
    return TRUE;
}

BOOLEAN
STDCALL
NrtlCreateHashMapUstr(
    IN UINT32 ElementSize,
    OUT PNRTL_LIST* HashMap
    )
{
    return NrtlCreateList(ElementSize + sizeof(HASHMAP_USTR_KEY), HashMap);
}

BOOLEAN
STDCALL
NrtlAddElementToHashMapUstr(
    IN PNRTL_LIST HashMap,
    IN PUNICODE_STRING Key,
    IN PVOID Element OPTIONAL
    )
{
    BOOLEAN bStatus;
    UINT32 ElementSize;
    PHASHMAP_USTR_KEY El;

    bStatus = NrtlGetListElementSize(HashMap, &ElementSize);
    if (!bStatus) return FALSE;

    El = NrtlAllocateHeap(HEAP_ZERO_MEMORY, ElementSize);
    if (!bStatus) return FALSE;

    bStatus = NrtlCreateKeyForHashMapUstr(Key, El);
    if (!bStatus) goto Cleanup;

    if (Element != NULL) x86CopyMemory(El + 1, Element, ElementSize - sizeof(HASHMAP_USTR_KEY));

    bStatus = NrtlAddElementToList(HashMap, El);
    if (!bStatus) RtlFreeUnicodeString(&El->Key);

Cleanup:
    NrtlFreeHeap(El);
    return bStatus;
}

BOOLEAN
STDCALL
NrtlGetPtrToHashMapUstrElementByIndex(
    IN PNRTL_LIST HashMap,
    IN UINT32 Index,
    OUT PVOID* Ptr
    )
{
    BOOLEAN bStatus;
    PHASHMAP_USTR_KEY Element;

    bStatus = NrtlGetPtrToListElement(HashMap, Index, &Element);
    if (!bStatus) return FALSE;

    *Ptr = Element + 1;
    return TRUE;
}

BOOLEAN
STDCALL
NrtlGetPtrToHashMapUstrElementByKey(
    IN PNRTL_LIST HashMap,
    IN PHASHMAP_USTR_KEY Key,
    OUT PVOID* Ptr
    )
{
    UINT32 Count;
    UINT32 i;
    PHASHMAP_USTR_KEY Element;

    NrtlGetListSize(HashMap, &Count);

    for (i = 0; i < Count; i++) {

        NrtlGetPtrToListElement(HashMap, i, &Element);
        if (Key->KeyHash == Element->KeyHash && RtlCompareUnicodeString(&Key->Key, &Element->Key, FALSE) == 0) {

            *Ptr = Element + 1;
            return TRUE;
        }
    }
    return FALSE;
}

BOOLEAN
STDCALL
NrtlGetPtrToHashMapUstrElementByUstr(
    IN PNRTL_LIST HashMap,
    IN PUNICODE_STRING String,
    OUT PVOID* Ptr
    )
{
    BOOLEAN bStatus;
    HASHMAP_USTR_KEY Key;

    bStatus = NrtlCreateKeyForHashMapUstr(String, &Key);
    if (!bStatus) return FALSE;

    bStatus = NrtlGetPtrToHashMapUstrElementByKey(HashMap, &Key, Ptr);
    RtlFreeUnicodeString(&Key.Key);
    return bStatus;
}

BOOLEAN
STDCALL
NrtlDestroyHashMapUstr(
    IN PNRTL_LIST HashMap
    )
{
    UINT32 Count;
    UINT32 i;
    PHASHMAP_USTR_KEY Element;

    NrtlGetListSize(HashMap, &Count);

    for (i = 0; i < Count; i++) {

        NrtlGetPtrToListElement(HashMap, i, &Element);
        RtlFreeUnicodeString(&Element->Key);
    }

    NrtlDestroyList(HashMap);
    return TRUE;
}