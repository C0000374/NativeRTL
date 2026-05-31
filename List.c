//
// Copyright (C) C0000374
//

#include <NRTLp.h>

typedef struct __LIST_BLOCK LIST_BLOCK, * PLIST_BLOCK;
struct __LIST_BLOCK {
    PLIST_BLOCK Previous;
    PLIST_BLOCK Next;
    PVOID Block;
    UINT32 Count;
};

typedef struct __NRTL_LIST NRTL_LIST; // PNRTL_LIST определён в NRTL.h
struct __NRTL_LIST {
    LIST_BLOCK FirstBlock;
    UINT32 ElementSize;
    UINT32 ElementsCount;
    UINT32 BlockCount;
    UINT32 BlockPtrArraySize;
    PLIST_BLOCK* BlockPtrArray;
};

BOOLEAN
STDCALL
NrtlCreateList(
    IN UINT32 ElementSize,
    OUT PNRTL_LIST* List_
    )
/*
 * --- NrtlCreateList ---
 * Создает блочный список
 */
{
    PNRTL_LIST List;

    List = NrtlAllocateHeap(0, sizeof(NRTL_LIST));
    if (List == NULL) return FALSE;

    NrtlInitializeLinkedList((PNRTL_ILinkedList)&List->FirstBlock);

    List->FirstBlock.Block = NrtlAllocateHeap(HEAP_ZERO_MEMORY, ElementSize * 256);
    if (List->FirstBlock.Block == NULL) {

        NrtlFreeHeap(List);
        return FALSE;
    }

    List->FirstBlock.Count = 0;
    List->ElementSize = ElementSize;
    List->ElementsCount = 0;
    List->BlockCount = 1;
    List->BlockPtrArraySize = 256;

    List->BlockPtrArray = NrtlAllocateHeap(0, sizeof(PLIST_BLOCK) * 256);
    if (List->BlockPtrArray == NULL) {

        NrtlFreeHeap(List->FirstBlock.Block);
        NrtlFreeHeap(List);
        return FALSE;
    }

    List->BlockPtrArray[0] = &List->FirstBlock;

    *List_ = List;
    return TRUE;
}

BOOLEAN
STDCALL
NrtlGetPtrToListElement(
    IN PNRTL_LIST List,
    IN UINT32 ElementIndex,
    OUT PVOID* Ptr_
    )
/*
 * --- NrtlGetPtrToListElement ---
 * Возвращает указатель на элемент блочного списка
 */
{
    PLIST_BLOCK Block;

    if ((ElementIndex >> 8) >= List->BlockCount) return FALSE;

    Block = List->BlockPtrArray[ElementIndex >> 8];
    if ((ElementIndex & 255) >= Block->Count) return FALSE;
    
    *Ptr_ = ((PBYTE)Block->Block) + List->ElementSize * (ElementIndex & 255);
    return TRUE;
}

BOOLEAN
STDCALL
NrtlpCreateListBlock(
    IN PNRTL_LIST List
    )
{
    PLIST_BLOCK Block;
    PLIST_BLOCK* BlockPtrArray;

    if (List->BlockCount == List->BlockPtrArraySize) {

        if (List->BlockPtrArraySize * 2 > 0x10000) return FALSE; // В списке не может быть больше 2^24 элементов

        List->BlockPtrArraySize <<= 1;

        BlockPtrArray = NrtlReAllocateHeap(0, List->BlockPtrArray, sizeof(PLIST_BLOCK) * List->BlockPtrArraySize);
        if (BlockPtrArray == NULL) return FALSE;

        List->BlockPtrArray = BlockPtrArray;
    }

    Block = NrtlAllocateHeap(0, sizeof(LIST_BLOCK));
    if (Block == NULL) return FALSE;

    Block->Block = NrtlAllocateHeap(HEAP_ZERO_MEMORY, List->ElementSize * 256);
    if (Block->Block == NULL) {

        NrtlFreeHeap(Block);
        return FALSE;
    }

    NrtlInsertElementToLinkedList((PNRTL_ILinkedList)List->FirstBlock.Previous, (PNRTL_ILinkedList)Block);
    Block->Count = 0;

    List->BlockPtrArray[List->BlockCount] = Block;
    List->BlockCount++;
    return TRUE;
}

BOOLEAN
STDCALL
NrtlAddElementToList(
    IN PNRTL_LIST List,
    IN PVOID Element OPTIONAL
    )
/*
 * --- NrtlAddElementToList ---
 * Добавляет элемент в конец блочного списка
 */
{
    PLIST_BLOCK Block;
    BOOLEAN bStatus;
    PVOID ElementInList;

    if (List->BlockPtrArray[List->BlockCount - 1]->Count == 256) {

        bStatus = NrtlpCreateListBlock(List);
        if (!bStatus) return FALSE;
    }

    Block = List->BlockPtrArray[List->BlockCount - 1];
    if (Element != NULL) {
        ElementInList = ((PBYTE)Block->Block) + List->ElementSize * Block->Count;
        x86CopyMemory(ElementInList, Element, List->ElementSize);
    }
    Block->Count++;
    List->ElementsCount++;
    return TRUE;
}

BOOLEAN
STDCALL
NrtlGetListSize(
    IN PNRTL_LIST List,
    OUT PUINT32 Size
    )
/*
 * --- NrtlGetListSize ---
 * Возвращает длину блочного списка
 */
{
    *Size = List->ElementsCount;
    return TRUE;
}

BOOLEAN
STDCALL
NrtlDestroyList(
    IN PNRTL_LIST List
    )
/*
 * --- NrtlDestroyList ---
 * Уничтожает блочный список
 */
{
    PLIST_BLOCK Block;
    PLIST_BLOCK NextBlock;

    Block = List->FirstBlock.Next;

    while (Block != &List->FirstBlock) {
        NextBlock = Block->Next;

        NrtlFreeHeap(Block->Block);
        NrtlFreeHeap(Block);

        Block = NextBlock;
    }

    NrtlFreeHeap(List->FirstBlock.Block);
    NrtlFreeHeap(List->BlockPtrArray);
    NrtlFreeHeap(List);
    return TRUE;
}