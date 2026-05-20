#include <NRTLp.h>

BOOLEAN
STDCALL
NrtlDosPathNameToNtPathName(
    IN PCWSTR DosPathName,
    OUT PUNICODE_STRING NtPathName
    )
{
    PWSTR Buffer;
    UINT32 u32Status;
    BOOLEAN bStatus;

    Buffer = NrtlAllocateHeap(0, 2048);
    if (Buffer == NULL) return FALSE;

    u32Status = RtlGetFullPathName_U(DosPathName, 2048, Buffer, NULL);
    if (u32Status == 0) {
        NrtlFreeHeap(Buffer);
        return FALSE;
    }

    bStatus = RtlDosPathNameToNtPathName_U(Buffer, NtPathName, NULL, NULL);
    NrtlFreeHeap(Buffer);
    if (!bStatus) return FALSE;
    return TRUE;
}