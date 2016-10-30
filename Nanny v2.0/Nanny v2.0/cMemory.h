#pragma once
#include <Windows.h>

void cracking_hook_function(int from, int to);
void cracking_hook_call(int from, const int to);
void cracking_write_text(int from, char* text, int len = 0);
void* DetourCreate(BYTE *src, const BYTE *dst, const int len);