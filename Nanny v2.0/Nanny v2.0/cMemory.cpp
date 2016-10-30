#include "cMemory.h"

void cracking_hook_function(int from, int to)
{
	DWORD dwback;
	const int len = 5;

	VirtualProtect((LPVOID)from, len, PAGE_READWRITE, &dwback);

	int relative = to - (from+5); // +5 is the position of next opcode
	memset((void *)from, 0xE9, 1); // JMP-OPCODE
	memcpy((void *)(from+1), &relative, 4); // set relative address with endian

	VirtualProtect((LPVOID)from, len, dwback, &dwback);
}

void cracking_hook_call(int from, const int to)
{
	DWORD dwback;
	const int len = 5;

	VirtualProtect((LPVOID)from, len, PAGE_READWRITE, &dwback);

	int relative = to - (from+5); // +5 is the position of next opcode
	memcpy((void *)(from+1), &relative, 4); // set relative address with endian

	VirtualProtect((LPVOID)from, len, dwback, &dwback);
}

void cracking_write_text(int from, char* text, int len)
{
	DWORD dwback;
	int strln = len;

	if (strln == 0) {
		strln = strlen(text);
	}

	VirtualProtect((LPVOID)from, strln, PAGE_READWRITE, &dwback);

	memcpy((void *)from, text, strln);

	VirtualProtect((LPVOID)from, strln, dwback, &dwback);
}

void* DetourCreate(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;

	VirtualProtect(src, len, PAGE_READWRITE, &dwback);

	memcpy(jmp, src, len); jmp += len;

	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;

	VirtualProtect(src, len, dwback, &dwback);

	return (jmp - len);
}