#include "device.h"

int windowHeight, windowWidth;

BOOL CALLBACK enumWind(HWND handle, LPARAM lp)
{
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);

	if (GetCurrentProcessId() != procId)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(enumWind, NULL);

	RECT size;
	GetWindowRect(window, &size);
	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	return window;
}

bool GetD3D9Device(void** pTable, size_t size)
{
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D) 
		return false;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	IDirect3DDevice9* pDummyDevice = nullptr;
	HRESULT dummyDevCreated = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (FAILED(dummyDevCreated) || !pDummyDevice) 
	{
		pD3D->Release();
		return false;
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);

	pDummyDevice->Release();
	pD3D->Release();
	return true;
}