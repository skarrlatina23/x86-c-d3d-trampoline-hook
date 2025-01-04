#pragma once
typedef unsigned char BYTE;

void Patch(BYTE* dst, BYTE* src, unsigned int size);

bool Hook(char* src, char* dst, unsigned int len);

char* TrampolineHook(char* src, char* dst, unsigned int len);