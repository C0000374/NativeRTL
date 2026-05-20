#include <NRTLp.h>

BOOL
STDCALL
NrtlpDllEntryPoint(
    IN PVOID Dll,
    IN UINT32 CallReason,
    IN PVOID Reserved
    )
{
    if (CallReason == DLL_PROCESS_ATTACH) {
        NrtlpInitializeHeap();
    }

    return TRUE;
}