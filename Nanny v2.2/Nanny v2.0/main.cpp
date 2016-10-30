/*
 * Nanny for Call of Duty 2 (1.0) by BratokHR
 */

#include "main.h"

field_t * field = (field_t *)0x005C3700;
window_t * window = (window_t*)0x00B68DE8;
std::string cmd_name, args;

void ReadCMD()
{
	// get full message
	std::string full_message = std::string(field->chat.buffer);

	// reset cmd_name and args
	cmd_name = "", args = "";

	// load first symbol
	std::string symbol;

	if (!set.Load(NANNY_SECTION, "symbol", (char*)symbol.c_str()) || symbol[0] == ' ') {
		symbol = "!";
	}

	// if message is empty, command list is empty or the first symbol is 'FIRST_SYMBOL'
	if (full_message.empty() || cmd.empty() || full_message[0] != symbol[0]) {
		return;
	}

	// get command
	unsigned int num_symbol = full_message.find(" "); // '-ban Player'

	if (num_symbol == -1) {
		cmd_name = full_message.substr(1, full_message.size());
		args = "";
	}
	else {
		cmd_name = full_message.substr(1, num_symbol-1);
		args = full_message.substr(num_symbol+1, full_message.size());
	}

	// get command list
	std::string cmd_list = "", reg = "^" + cmd_name + ".*$"; // ^ban.*$
	std::regex pattern(reg.c_str(), std::regex_constants::icase);

	for (UINT i = 0; i < cmd.size(); i++) {
		if (std::regex_match(cmd[i].name, pattern)) {
			cmd_list += std::string("^7") + symbol[0] + std::string(cmd[i].name) + " " + std::string(cmd[i].desc) + "\n";
		}
	}

	#pragma region standart: x y scaleX scaleY color
	float sz = *(float*)0xB68DB4 * (float)0.20833333;
	float scaleX = *(float*)0xB68DB0 * sz;
	float scaleY = *(float*)0xB68DB4 * sz;
	float x = 100.0f;
	float y = (window->height / 2.7f);
	vec4_t color = RGB(255, 255, 255);
	#pragma endregion

	// drawing command list
	Draw.Text(x, y, scaleX, scaleY, color, DT_LEFT, FONTBIG, cmd_list.c_str());
}

void hook_call_FielDraw(int a1, int a2, int a3, int a4)
{
	ReadCMD();

	( ( void (*)( int a1, int a2, int a3, int a4 ) )0x00409190 )( a1, a2, a3, a4 ); // call original
}

void hook_call_CL_AddReliableCommand(const char* msg)
{
	// reset
	static bool reset = false;
	std::string cmd_msg = std::string(field->chat.buffer);
	std::string cmd_reset = "reset";

	if (cmd_msg == cmd_reset || reset ) {
		reset = true;

		if (cmd_msg == "y") {
			remove(set.path.c_str());
			Nanny(AD, "Settings have been ^1reset");
			reset = false;
			return;
		}
		else if (cmd_msg != cmd_reset) {
			reset = false;
			return;
		}

		Nanny(AD, "To ^2reset^7 the settings, write ^3y");
		return;
	}

	if (cmd_name.empty() || cmd.empty()) {
		( ( void (*)( const char* msg ) )0x0040B650 )( msg ); // call original
		return;
	}

	std::string reg = "^" + cmd_name + "$"; // ^time$
	std::regex pattern(reg.c_str(), std::regex_constants::icase);

	for (UINT i = 0; i < cmd.size(); i++) {
		if (std::regex_match(cmd[i].name, pattern) && !IsBadReadPtr(cmd[i].func, 4)) {
			DWORD addr = (DWORD)cmd[i].func;
			__asm call addr;
			return;
		}
	}

	Nanny(ERROR, "Unknown command");
}

void main()
{
	while (GetModuleHandleA("gfx_d3d_mp_x86_s.dll") == 0)
		Sleep(100);

	__PRINT__::print::PrintInit();
	InitConmmand();

	// hooks functions
	cracking_hook_call((int)0x0040656C, (int)hook_call_FielDraw);
	cracking_hook_call((int)0x0040A437, (int)hook_call_CL_AddReliableCommand);
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		// get current folder
		char folder[MAX_PATH];
		DWORD n = GetModuleFileName( hModule, folder, MAX_PATH );

		for( char * p = &folder[ n ]; *p != '\\'; p-- ) *p = '\0';
		set.path = std::string(folder) + std::string(NANNY_INI);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}

	return true;
}