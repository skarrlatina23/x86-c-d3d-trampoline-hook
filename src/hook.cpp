#include "includes.h"

const char* REL_JMP = "\xE9"; // Opcode for a relative JMP instruction
// 1 byte for "jmp" + 4 bytes address
const unsigned int SIZE_OF_REL_JMP = 5;

void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oProc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oProc, &oProc);
}

bool Hook(char* src, char* dst, unsigned int len)
{
	if (len < 5)
		return false;

	DWORD oProc;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
	memset(src, 0x90, len); // Fill the region with NOP instructions
	uintptr_t dstAddy = (uintptr_t)(dst - src - SIZE_OF_REL_JMP);
	*src = *REL_JMP;
	*(uintptr_t*)(src + 1) = dstAddy;
	VirtualProtect(src, len, oProc, &oProc);
}

char* TrampolineHook(char* src, char* dst, unsigned int len)
{
	if (len < SIZE_OF_REL_JMP)
		return 0;

	char* gateway = (char*)VirtualAlloc(0, len + SIZE_OF_REL_JMP, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len); // Copy original instructions to the trampoline

	uintptr_t srcAddy = (uintptr_t)(src - gateway - SIZE_OF_REL_JMP);
	*(gateway + len) = *REL_JMP;
	*(uintptr_t*)(gateway + len + 1) = srcAddy;

	if (Hook(src, dst, len))
		return gateway;

	return nullptr;
}
