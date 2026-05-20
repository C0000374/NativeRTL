#include <NRTLp.h>

HANDLE NrtlpProcessHeap;
PNRTL_ALLOCFAIL_HANDLER NrtlpAllocFailHandler = NULL;

VOID
STDCALL
NrtlpInitializeHeap(
    VOID
    )
{
    NrtlpProcessHeap = RtlProcessHeap();
}

PNRTL_ALLOCFAIL_HANDLER
STDCALL
NrtlSetAllocFailHandler(
    IN PNRTL_ALLOCFAIL_HANDLER Handler
    )
{
    PNRTL_ALLOCFAIL_HANDLER OldHandler;

    OldHandler = NrtlpAllocFailHandler;
    NrtlpAllocFailHandler = Handler;
    return OldHandler;
}

PVOID
STDCALL
NrtlAllocateHeap(
    IN UINT32 Flags,
    IN UINT32 BlockSize
    )
{
    PVOID Block;
    BOOLEAN bStatus;

    for (;;) {

        Block = RtlAllocateHeap(NrtlpProcessHeap, Flags, BlockSize);
        if (Block != NULL) return Block;

        if (NrtlpAllocFailHandler != NULL) {

            bStatus = NrtlpAllocFailHandler(BlockSize, NULL);
            if (!bStatus) return NULL;
        }
        else return NULL;
    }
}

PVOID
STDCALL
NrtlReAllocateHeap(
    IN UINT32 Flags,
    IN PVOID Block,
    IN UINT32 BlockSize
    )
{
    PVOID NewBlock;
    BOOLEAN bStatus;

    for (;;) {

        NewBlock = RtlReAllocateHeap(NrtlpProcessHeap, Flags, Block, BlockSize);
        if (NewBlock != NULL) return NewBlock;

        if (NrtlpAllocFailHandler != NULL) {

            bStatus = NrtlpAllocFailHandler(BlockSize, Block);
            if (!bStatus) return NULL;
        }
        else return NULL;
    }
}

VOID
STDCALL
NrtlFreeHeap(
    IN PVOID Block
    )
{
    RtlFreeHeap(NrtlpProcessHeap, 0, Block);
}

VOID
STDCALL
NrtlFreeLinkedList(
    IN PNRTL_ILinkedList FirstElement
    )
{
    PNRTL_ILinkedList Element;

    Element = FirstElement;

    do {
        Element = Element->Next;
        NrtlFreeHeap(Element->Previous);
    } while (Element != FirstElement);
}