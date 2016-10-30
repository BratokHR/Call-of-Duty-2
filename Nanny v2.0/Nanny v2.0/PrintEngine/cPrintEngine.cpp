/**
 * by BratokHR for Call of Duty 2
 *
 * cPrintEngine.cpp
 */

#include "cPrintEngine.h"

bool __PRINT__::print::init = false;
int __PRINT__::print::version = 0;
DWORD __PRINT__::print::d_sayline = 0x0;
DWORD __PRINT__::print::d_saybold = 0x0;
DWORD __PRINT__::print::d_say = 0x0;
DWORD __PRINT__::print::d_printinconsole = 0x0;
DWORD __PRINT__::print::d_cmdconsole = 0x0;

__PRINT__::g_CG_Printf __PRINT__::print::PrintConsole = (g_CG_Printf)0x0;
__PRINT__::g_WritetoConsole __PRINT__::print::WritetoConsole = (g_WritetoConsole)0x0;

char *__PRINT__::print::ConsoleText = 0x0;
char *__PRINT__::print::IpPort = 0x0;
char *__PRINT__::print::UserInfo = 0x0;

/**
* Инициализирует переменные, вызывается только 1 раз
*/
void __PRINT__::print::PrintInit() {
	if (strcmp((char*)0x561B1C, "1.0") == 0)
	{
		version = 0;
		d_cmdconsole = CMDCONSOLE_0;
		d_printinconsole = PRINTINCONSOLE_0;
		d_say = SAY_0;
		d_saybold = SAYBOLD_0;
		d_sayline = SAYLINE_0;

		ConsoleText = (char*)CONSOLETEXT_0;
		IpPort = (char*)IPPORT_0;
		UserInfo = (char*)USERINFO_0;
	}
	else if (strcmp((char*)0x5A1D54, "1.2") == 0)
	{
		version = 2;
		d_cmdconsole = CMDCONSOLE_2;
		d_printinconsole = PRINTINCONSOLE_2;
		d_say = SAY_2;
		d_saybold = SAYBOLD_2;
		d_sayline = SAYLINE_2;

		ConsoleText = (char*)CONSOLETEXT_2;
		IpPort = (char*)IPPORT_2;
		UserInfo = (char*)USERINFO_2;
	}
	else
	{
		version = 3;
		d_cmdconsole = CMDCONSOLE_3;
		d_printinconsole = PRINTINCONSOLE_3;
		d_say = SAY_3;
		d_saybold = SAYBOLD_3;
		d_sayline = SAYLINE_3;

		ConsoleText = (char*)CONSOLETEXT_3;
		IpPort= (char*)IPPORT_3;
		UserInfo = (char*)USERINFO_3;
	}

	PrintConsole = (g_CG_Printf)d_printinconsole;
	WritetoConsole = (g_WritetoConsole)d_cmdconsole;

	init = true;
}

/**
* Возвращает версию игры (по-умолчанию вернет 1.3)
*/
char* __PRINT__::print::CheckVersionGame() {
	if (version == 0)
		return "1.0";
	else if (version == 2)
		return "1.2";

	return "1.3";
}

/**
* Выводит сообщения в sayline (слева снизу)
*/
void __PRINT__::print::Sayline( const char *msg, ...) {
	va_list		argptr;
	char		text[MAX_SAY_TEXT];

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

/**
* Выводит сообщения в saybold (по центру экрана)
*/
void __PRINT__::print::Saybold( const char *msg, ...) {
	va_list		argptr;
	char		text[MAX_SAY_TEXT];

	va_start (argptr, msg);
	_vsnprintf_s<150> (text, 150, msg, argptr);
	va_end (argptr);

	__asm
	{
		lea ESI, text
		mov ECX, ESI
		call d_saybold
	}
}

/**
* Выводит сообщения в чат
*/
void __PRINT__::print::Say( const char *msg, ...) {
	va_list		argptr;
	char		text[MAX_SAY_TEXT];

	va_start (argptr, msg);
	_vsnprintf_s<150> (text, 150, msg, argptr);
	va_end (argptr);

	__asm
	{
		lea ESI, text
		mov ECX, ESI
		call d_say
	}
}