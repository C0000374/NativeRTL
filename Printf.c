#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <NRTLp.h>
#include <StdIO.h>

INT32
STDCALL
Nrtl_vsnprintf(
    IN PSTR Buffer,
    IN UINT32 BufferLength,
    IN PCSTR Format,
    IN va_list ArgList
    )
{
    INT32 Result;

    Result = _vsnprintf(Buffer, BufferLength, Format, ArgList);
    Buffer[BufferLength - 1] = 0;
    return Result;
}

INT32
STDCALL
Nrtl_vsnwprintf(
    IN PWSTR Buffer,
    IN UINT32 BufferLength,
    IN PCWSTR Format,
    IN va_list ArgList
    )
{
    INT32 Result;

    Result = _vsnwprintf(Buffer, BufferLength / 2, Format, ArgList);
    Buffer[BufferLength / 2 - 1] = 0;
    return Result;
}

INT32
CDECL
Nrtl_snprintf(
    IN PSTR Buffer,
    IN UINT32 BufferLength,
    IN PCSTR Format,
    IN ...
    )
{
    va_list ArgList;

    va_start(ArgList, Format);
    return Nrtl_vsnprintf(Buffer, BufferLength, Format, ArgList);
}

INT32
CDECL
Nrtl_snwprintf(
    IN PWSTR Buffer,
    IN UINT32 BufferLength,
    IN PCWSTR Format,
    IN ...
    )
{
    va_list ArgList;

    va_start(ArgList, Format);
    return Nrtl_vsnwprintf(Buffer, BufferLength, Format, ArgList);
}