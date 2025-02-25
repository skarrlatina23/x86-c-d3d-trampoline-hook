#include "includes.h"
#include "device.h"
#include "hook.h"

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
	if (!pDevice)
		pDevice = o_pDevice;

	oEndScene(pDevice);
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
		oEndScene = (tEndScene)TrampolineHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	while (!GetAsyncKeyState(VK_END))
		Sleep(5);

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);

		const HANDLE hMainThread = CreateThread(
			nullptr,
			0,
			MainThread,
			hinstDLL,
			0,
			nullptr
		);

		if (hMainThread)
			CloseHandle(hMainThread);
	}
	return TRUE;
}