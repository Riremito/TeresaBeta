#include"Teresa.h"

#ifndef _WIN64
#define FILE_NAME_NGCLIENT L"NGClient.aes"
#define FILE_NAME_NGCLIENT_BACKUP L"_NGClient.aes"
#else
#define FILE_NAME_NGCLIENT L"NGClient64.aes"
#define FILE_NAME_NGCLIENT_BACKUP L"_NGClient64.aes"
#endif

decltype(CreateFileW) *_CreateFileW = NULL;
HANDLE WINAPI CreateFileW_Hook(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
	if (SimpleHook::IsCallerEXE(_ReturnAddress())) {
		if (lpFileName) {
			if (wcsstr(lpFileName, L"\\" FILE_NAME_NGCLIENT)) {
				std::wstring wFakePath;
				GetDir(wFakePath);
				wFakePath += FILE_NAME_NGCLIENT_BACKUP;
				std::wstring wText = lpFileName;
				wText += L" -> " + wFakePath;
				return _CreateFileW(wFakePath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
			}
		}
	}
	return _CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

bool RemoveBC() {
	Rosemary r;

	ULONG_PTR uRemoveBC_MSCRC_EnterField = r.Scan(L"48 89 54 24 10 48 89 4C 24 08 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B 8C 24 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 83 BC 24 ?? ?? ?? ?? 00 74");
	ULONG_PTR uRemoveBC_HeartBeat_Recv = r.Scan(L"48 89 54 24 10 48 89 4C 24 08 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? E9");
	ULONG_PTR uRemoveBC_HeartBeat_Init = r.Scan(L"48 89 4C 24 08 48 83 EC 68 E9", 5); // not good
	ULONG_PTR uRemoveBC_InGame_Check = r.Scan(L"48 89 54 24 10 48 89 4C 24 08 B8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 2B E0 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? E9");
	ULONG_PTR uRemoveBC_3minutes_DC = r.Scan(L"48 89 54 24 10 48 89 4C 24 08 48 83 EC ?? BA ?? ?? ?? ?? 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 90 48 8D 54 24 ?? 48 8B 4C 24 ?? E8 ?? ?? ?? ?? 90 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 48 83 C4 ?? C3");

	SCANRES(uRemoveBC_MSCRC_EnterField);
	SCANRES(uRemoveBC_HeartBeat_Recv);
	SCANRES(uRemoveBC_HeartBeat_Init);
	SCANRES(uRemoveBC_InGame_Check);
	SCANRES(uRemoveBC_3minutes_DC);

	if (!uRemoveBC_MSCRC_EnterField || !uRemoveBC_HeartBeat_Recv || !uRemoveBC_HeartBeat_Init || !uRemoveBC_InGame_Check) {
		KILLFORSAFETY(L"Error: RemoveBC Initialization");
		return false;
	}

	r.Patch(uRemoveBC_MSCRC_EnterField, L"48 31 C0 C3");
	r.Patch(uRemoveBC_HeartBeat_Recv, L"48 31 C0 C3");
	r.Patch(uRemoveBC_HeartBeat_Init, L"48 31 C0 C3");
	r.Patch(uRemoveBC_InGame_Check, L"48 31 C0 C3");
	r.Patch(uRemoveBC_3minutes_DC, L"48 31 C0 C3");

	SHook(CreateFileW); // file scan bypass recv @002A list
	return true;
}