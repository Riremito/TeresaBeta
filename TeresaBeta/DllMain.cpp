#include"Teresa.h"

#define FAST_LOAD L"FastLoad"
std::vector<std::wstring> vFastLoadDlls;

bool FastLoad() {
	for (size_t i = 0; i < vFastLoadDlls.size(); i++) {
		if (LoadLibraryW(vFastLoadDlls[i].c_str())) {
			DEBUG(L"FastLoad:" + vFastLoadDlls[i]);
		}
	}
	return true;
}

// for JMS v415, some codes are from JMS v404
#define INI_FILE_NAME L"TeresaBeta.ini"

bool LoadConfig(HMODULE hDll) {
	Config conf(INI_FILE_NAME, hDll);
	std::wstring wLoaderDir;

	if (!GetDir(wLoaderDir, hDll)) {
		return false;
	}

	for (size_t i = 1; i <= 10; i++) {
		std::wstring wDllName;
		conf.Read(FAST_LOAD, L"DLL_" + std::to_wstring(i), wDllName);

		if (wDllName.length()) {
			if (wDllName.find('\\') == std::wstring::npos) {
				vFastLoadDlls.push_back(wLoaderDir + L"\\" + wDllName);
			}
			else {
				vFastLoadDlls.push_back(wDllName);
			}
		}
	}

	return true;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		LoadConfig(hinstDLL);
		FastLoad();
		RemoveBC();
		Extra();
	}
	return TRUE;
}