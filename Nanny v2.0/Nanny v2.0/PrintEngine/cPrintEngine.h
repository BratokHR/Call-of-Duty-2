/**
 * by BratokHR for Call of Duty 2
 *
 * cPrintEngine.h
 */

#pragma once

#ifndef _CPRINTENGINE_H_
#define _CPRINTENGINE_H_

#include <Windows.h>
#include <stdio.h>

#define MAX_SAY_TEXT 150

/**
 * Адреса функций
 */
#define SAYLINE_0				0x004B9230
#define SAYLINE_2				0x004BF5E0
#define SAYLINE_3				0x004BF650
#define SAYBOLD_0				0x004B9340
#define SAYBOLD_2				0x004BF700
#define SAYBOLD_3				0x004BF770
#define SAY_0					0x004CABB0
#define SAY_2					0x004D0DA0
#define SAY_3					0x004D0E10
#define PRINTINCONSOLE_0		0x00430540
#define PRINTINCONSOLE_2		0x00431E90
#define PRINTINCONSOLE_3		0x00431EE0
#define CMDCONSOLE_0			0x0041FD10
#define CMDCONSOLE_2			0x00421470
#define CMDCONSOLE_3			0x004214C0
#define CONSOLETEXT_0			0x018312F0
#define CONSOLETEXT_2			0x018D2390
#define CONSOLETEXT_3			0x00652410
#define IPPORT_0				0x0060F2A8
#define IPPORT_2				0x00652410
#define IPPORT_3				0x0068A410
#define USERINFO_0				0x00B3C800
#define USERINFO_2				0x0
#define USERINFO_3				0x0

namespace __PRINT__
{
	typedef struct print_cuRect_s print_cuRect_t;
	typedef enum {
		print_qfalse,
		print_qtrue
	} print_qbool;

	/**
	 * Функции движка
	 */
	typedef void (*g_CG_Printf)( const char *msg, ... ); 
	typedef void (*g_WritetoConsole) ( const char *msg );

	class print {
	private:
		static int		version;			// Версия игры 0 / 2 / 3
		static bool		init;				// Иниц. переменных
		static DWORD	d_sayline;			// Адрес функции вывода сообщения в sayline (слева снизу)
		static DWORD	d_saybold;			// Адрес функции вывода сообщения в saybold (по центру экрана)
		static DWORD	d_say;				// Адрес функции вывода текста в чат
		static DWORD	d_printinconsole;	// Адрес функции написания текста в консоль игры
		static DWORD	d_cmdconsole;		// Адрес функции отправки команды в консоль игры

	public:
		// Инициализирует переменные, вызывается только 1 раз
		static void PrintInit();
		// Возвращает версию игры (по-умолчанию вернет 1.3)
		static char* CheckVersionGame();
		// Функция вывода сообщения в sayline (слева снизу)
		static void Sayline( const char *msg, ...);
		// Функция вывода сообщения в saybold (по центру экрана)
		static void Saybold( const char *msg, ...);
		// Функция вывода текста в чат игры
		static void Say( const char *msg, ...);

		static char *	ConsoleText;		// Чтение текста из консоли
		static char *	IpPort;			// Айпи и порт сервера к которому в настоящий момент подключен клиент
		static char *	UserInfo;		// Содержит базовую информацию о клиенте

		static g_CG_Printf PrintConsole;
		static g_WritetoConsole WritetoConsole;
	};
}

#endif