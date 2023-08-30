#include<Windows.h>
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

// Getting Return Address
// 140D89F10 (v414.1) -> 140DDF350 (v415.1) -> 140DDF350 (v415.2)
// 140DEE7B0 (v416.2)
// 48 89 54 24 10 48 89 4C 24 08 56 57 48 83 EC 78 E9 1st result
// @003C blocked = cannot be login
// 14ACA130A
// ULONG_PTR uReturnAddress = 0x14B0EC509; // v415.1
// ULONG_PTR uReturnAddress = 0x14ACA130A; // v415.2
ULONG_PTR uReturnAddress = 0x14AF8075D; // v416.2

// NGClient64.aes+153A0
#ifndef _WIN64
void __declspec(naked) Teresa1() {
	__asm {
		sub esp,0x08
		mov eax,0x05219A21 // GMS v224.2, not working ???
		mov dword ptr [esp],eax
		mov eax,1
		ret
	}
}
#else
ULONG_PTR Teresa1(void *v1, void *v2, void *v3, void *v4) {
	*(ULONG_PTR *)_AddressOfReturnAddress() = uReturnAddress;
	return 0;
}
#endif

ULONG_PTR Teresa2(void *v1, void *v2, void *v3, void *v4) {
	return 0;
}

ULONG_PTR Teresa3(void *v1, void *v2, void *v3, void *v4) {
	return 0;
}

ULONG_PTR Teresa4(void *v1, void *v2, void *v3, void *v4) {
	return 0;
}

ULONG_PTR Teresa5(void *v1, void *v2, void *v3, void *v4) {
	return 0;
}

ULONG_PTR Teresa6(void *v1, void *v2, void *v3, void *v4) {
	return 0;
}