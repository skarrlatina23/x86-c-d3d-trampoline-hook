#pragma once
#include "includes.h"

using tEndScene = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);

static HWND window;

extern int windowHeight, windowWidth;
extern LPDIRECT3DDEVICE9 pDevice;

bool GetD3D9Device(void** pTable, size_t size);