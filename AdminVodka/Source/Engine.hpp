typedef float vec_t;
typedef vec_t vec4_t[4];

typedef void (*g_WriteToConsole) ( const char *msg );
g_WriteToConsole WriteToConsole = (g_WriteToConsole)0x0041FD10;

typedef void (*g_PrintInConsole) ( int a1, char *text );
g_PrintInConsole PrintInConsole = (g_PrintInConsole)0x400303A0;

typedef void (*g_SendCommand) ( char *text );
g_SendCommand SendCommand = (g_SendCommand)0x0040B650;

void DrawConsoleBox(int height, char *text)
{
	float *f_color = (float*)(*(DWORD**)(*(DWORD**)(0x59DB84) + 0x2));
	vec4_t text_color = {1, 1, 1, 1};

	DWORD d_funcbox = 0x00405230;
	DWORD d_functext = 0x00404FF0;
	DWORD *d_text_color = (DWORD*)text_color; //0x0055C05C;

	float *offset_y = (float*)0x59DB9C;
	offset_y[0] += 15.0f;

	__asm
	{
		PUSH	f_color
		PUSH	height
		CALL	d_funcbox
		PUSH	text
		MOV		EAX, d_text_color
		CALL	d_functext
		ADD		ESP, 0x1C
	}
}

void Sayline( const char *msg, ...) {
	DWORD		d_sayline = 0x004B9230;
	va_list		argptr;
	char		text[150];

	va_start (argptr, msg);
	_vsnprintf_s<150> (text, 150, msg, argptr);
	va_end (argptr);

	__asm
	{
		lea ESI, text
		mov ECX, ESI
		call d_sayline
	}
}