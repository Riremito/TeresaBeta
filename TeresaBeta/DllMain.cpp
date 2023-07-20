#include"Teresa.h"

// for JMS v415, some codes are from JMS v404
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		LoadLibraryW(L"LocalHost64.dll");
		RemoveBC();
		HideProcesses(); // unchanged v404 to v415
		MemoryMap(); // unchanged v404 to v415
		HWIDRandomizer(); // unchanged v404 to v415
		MultiClient(); // unchanged v404 to v415
	}
	return TRUE;
}