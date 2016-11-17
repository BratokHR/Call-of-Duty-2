void cracking_hook_call(int from, const int to)
{
	DWORD dwback;
	const int len = 5;

	VirtualProtect((LPVOID)from, len, PAGE_READWRITE, &dwback);

	int relative = to - (from+5); // +5 is the position of next opcode
	memcpy((void *)(from+1), &relative, 4); // set relative address with endian

	VirtualProtect((LPVOID)from, len, dwback, &dwback);
}

void cracking_hook_push(int from, const int to)
{
	DWORD dwback;
	const int len = 5;

	VirtualProtect((LPVOID)from, len, PAGE_READWRITE, &dwback);

	memcpy((void *)(from+1), &to, 4);

	VirtualProtect((LPVOID)from, len, dwback, &dwback);
}

void cracking_write_text(int from, char* text, int len = 0)
{
	DWORD dwback;
	size_t strln = len;

	if (strln == 0) {
		strln =  strlen(text)+1;
	}

	VirtualProtect((LPVOID)from, strln, PAGE_READWRITE, &dwback);

	memcpy((void *)from, text, strln);

	VirtualProtect((LPVOID)from, strln, dwback, &dwback);
}