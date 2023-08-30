#ifndef __TERESA_H__
#define __TERESA_H__

#include"../Share/Simple/Simple.h"
#include"../Share/Hook/SimpleHook.h"
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

bool RemoveBC();
void Extra();
bool MemoryMap();
bool MultiClient();
bool HWIDRandomizer(Rosemary &r);
bool HideProcesses(Rosemary &r);
bool SkipLauncher(Rosemary &r);

#define KILLFORSAFETY(msg) \
{\
	MessageBoxW(NULL, msg, L"Teresa", MB_OK);\
	ExitProcess(1337);\
}

#endif