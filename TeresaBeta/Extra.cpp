#include"Teresa.h"

void Extra() {
	Rosemary r;
	HideProcesses(r); // unchanged v404 to v415
	MemoryMap(); // unchanged v404 to v415
	HWIDRandomizer(r); // unchanged v404 to v415
	MultiClient(); // unchanged v404 to v415
	SkipLauncher(r); // Launcher is updated in v415 (it had not changed for 15 years)
}

// Multi Client Part
#define MAPLE_MUTEX L"WvsClientMtx"
decltype(CreateMutexExW) *_CreateMutexExW = NULL;
HANDLE WINAPI CreateMutexExW_Hook(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess) {
	HANDLE hMutex = _CreateMutexExW(lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);

	if (lpName && wcsstr(lpName, MAPLE_MUTEX)) {
		HANDLE hDuplicatedMutex = NULL;
		if (DuplicateHandle(GetCurrentProcess(), hMutex, 0, &hDuplicatedMutex, 0, FALSE, DUPLICATE_CLOSE_SOURCE)) {
			CloseHandle(hDuplicatedMutex);
			DEBUG(L"Multi Client OK");
		}
	}

	return hMutex;
}

bool MultiClient() {
	SHook(CreateMutexExW);
	return true;
}
// ==============================
// Memory Map Part
bool MemoryMap() {
	HMODULE hNameSpace = LoadLibraryW(L"NameSpace.dll");
	Rosemary rn(L"NameSpace.dll");
	ULONG_PTR uMemoryMap = rn.Scan(L"48 83 E9 38 E9 ?? ?? ?? ?? CC CC CC 48 81 E9 98 00 00 00 E9"); // 1st result

	SCANRES(uMemoryMap);
	if (!uMemoryMap) {
		return false;
	}

	rn.Patch(uMemoryMap, L"48 31 C0 C3");
	DEBUG(L"MemoryMap OK");
	return true;
}
// ==============================
// HWID Part
std::string MapleID; // HWID用

void GenarateHWIDChecker(BYTE *HWID) {
	// 元から値がオーバーフローしている仕様なので、その通りに値を合わせる必要がある
	WORD Checker = 0;

	for (size_t i = 0; i < 6; i++) {
		WORD x = i + 3;
		Checker += *(WORD *)&HWID[i * 2] * x;
	}

	// 末尾に書き込む
	memcpy_s(&HWID[14], 2, &Checker, 2);
}

bool HWIDRandomizer(BYTE *HWID) {
	if (MapleID.length() == 0) {
		KILLFORSAFETY(L"Error: HWID is used before you login");
		return false;
	}

	DWORD seed = 0, seed2 = 0;

	for (size_t i = 0; i < MapleID.length(); i++) {
		seed += MapleID.at(i);
		seed2 ^= MapleID.at(i);
	}

	for (size_t i = 0; i < 10; i++) {
		HWID[i] ^= MapleID.length();
		HWID[i] ^= seed;
	}

	*(DWORD *)&HWID[0x06] ^= seed2;
	*(DWORD *)&HWID[0x02] ^= *(DWORD *)&HWID[0x06];
	return true;
}

void (*_Login)(void *, void *, char **, char **) = NULL;
void Login_Hook(void *rcx, void *rdx, char **cMapleID, char **cPassword) {
	MapleID = *cMapleID;
	return _Login(rcx, rdx, cMapleID, cPassword);
}

void (*_MIDLib_GetMachineId)(BYTE *) = NULL;
void MIDLib_GetMachineId_Hook(BYTE *HWID) {
	_MIDLib_GetMachineId(HWID);
	HWIDRandomizer(HWID);
	GenarateHWIDChecker(HWID);
}

bool HWIDRandomizer(Rosemary &r) {
	ULONG_PTR uLogin = r.Scan(L"4C 89 4C 24 20 4C 89 44 24 18 89 54 24 10 48 89 4C 24 08 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B 84 24 ?? ?? ?? ?? 83 B8 ?? ?? ?? ?? 00 74");
	if (!uLogin) {
		KILLFORSAFETY(L"Error: HWIDRandomizer Initialization");
		return false;
	}

	LoadLibraryW(L"MachineIdLib.dll");

	SHookFunction(Login, uLogin);
	SHookNT(MachineIdLib.dll, MIDLib_GetMachineId);
	return true;
}
// ==============================
// Process List Stealer Blocker
bool HideProcesses(Rosemary &r) {
	ULONG_PTR uProcessListPacketMaker = r.Scan(L"48 83 EC 48 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F B6 C0 85 C0 75 ?? 48 8D 15 ?? ?? ?? ?? 48 8D 4C 24 28 E8");

	if (!uProcessListPacketMaker) {
		KILLFORSAFETY(L"Error: HideProcesses Initialization");
		return false;
	}

	r.Patch(uProcessListPacketMaker, L"48 31 C0 C3");

	SCANRES(uProcessListPacketMaker);
	return true;
}
// ==============================
// Launcher Skip
bool SkipLauncher(Rosemary &r) {
	ULONG_PTR uSkipLauncher = r.Scan(L"48 89 5C 24 20 56 41 56 41 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B F1 33 C9 FF 15");
	SCANRES(uSkipLauncher);

	if (!uSkipLauncher) {
		return false;
	}

	r.Patch(uSkipLauncher, L"48 31 C0 48 FF C0 C3");
	return true;
}
// ==============================
// ==============================
// ==============================
// ==============================