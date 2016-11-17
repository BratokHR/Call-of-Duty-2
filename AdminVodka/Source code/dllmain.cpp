#include "stdafx.h"
#include "Commands.hpp"
#include "utils.hpp"

field_t *		field	= (field_t *)		0x005C3700;
serverInfo_t *	si		= (serverInfo_t *)	0x0137068C;
clientInfo_t *	ci		= (clientInfo_t *)	0x0145A914;
cg_t *			cg		= (cg_t *)			0x01379000;
bool *			inGame			= (bool *)	0x00C1E26C;
int * 			inMenu			= (int *)	0x008DC20C;
client_t client;

typedef void	(*t_EndFrame)	( );
t_EndFrame	o_EndFrame = (t_EndFrame)NULL;

const char *copyright = "AdminVodka: developed BratokHR specially to CrackedVodka";

vec4_t color_spec_text = { 1, 1, 1, 1 };

void h_EndFrame()
{
	o_EndFrame();

	if (!client.onVodka)
		return;

	if (client.spec) {
		float x = 0.0f, y = si->Height/3.0f;

		Draw.Text(x+5, y+15.0f, 1, 1, color_spec_text, DT_LEFT, FONTBIG, "^5Spectator ^7mode:\n^9P^7layer: %s^5[%i]", ci[client.spec_id].name, client.spec_id);
	}
}

void h_SendCommandToServer(char *text)
{
	// 0xA6EE80 - (int) кол-во аргументов
	// 0xA6C480 - указатели на аргументы

	if (!client.onVodka || cmds.empty()) {
		SendCommand(text);
		return;
	}

	std::string command = std::string(*(char**)0xA6C480);

	if (command[0] == '/')
		command = command.substr(1, command.size()-1);

	if (command == "commands") {
		cmds.clear(); // Подготовка вектора
		client.readCommands = true; // Флаг о том, что будут считываться команды
		client.readTick = si->FrameCount + 10; // Время считывания будет 2 ед. времени сервера
		SendCommand("/help"); // На сервер отправляется команда "//help"
	}
	else if (command == "error_test") {
		((void(*)(int a1, char *text)) 0x430B20)(6, "EXE_SAY"); // ошибка
	}
	else if (command == "spec") {
		if (client.spec) {
			client.spec ^= 1;
			Sayline("^9Admin^7Vodka: spectator ^5mode ^7is ^1disabled");
			return;
		}

		if (ci[cg->clientNum].team == team_t::TEAM_SPECTATOR) { // Проверка, что клиент в зрителях
			std::string arg = std::string(*(char**)0xA6C484); // Получение аргумента
			int player_id = getID(arg.c_str());  // Получение ID игрока

			if (player_id < 65 && player_id >= 0) {
				if (player_id == cg->clientNum) {
					Sayline("^9Admin^7Vodka: you ^1can't^7 choose yourself");
					return;
				}
				else if (ci[player_id].team == team_t::TEAM_SPECTATOR) {
					Sayline("^9Admin^7Vodka: this ^2player ^7plays for the spectators");
					return;
				}

				client.spec = true;
				client.spec_id = player_id;
				Sayline("^9Admin^7Vodka: spectator ^5mode ^7is ^1enabled");
			}
		}
		return;
	}
	else {
		SendCommand(text);
		return;
	}
}

void h_PrintInConsole(int a1, char *text)
{
	if (!client.onVodka) {
		PrintInConsole(a1, text);
		return;
	}

	if (client.readCommands) {
		std::string reg = "^(\\^5(.*)\\b.*\\^7(.*)\\b.*\\^7(.*)|\\^5(.*)\\b.*\\^7(.*)|\\^5(.*))\\n$";
		std::regex pattern(reg.c_str(), std::regex_constants::icase);
		std::cmatch match;

		if (std::regex_match(text, pattern))
		if (std::regex_search(text, match, pattern)) {
			if (!match[7].str().empty()) {
				AddCmd((char*)match[7].str().c_str(), "", "");
			}
			else if (!match[5].str().empty()){
				std::string arg_or_desc = match[6].str();

				for (int i = 0; i < 7; i++) {
					if (arg_or_desc == args_list[i]) {
						AddCmd((char*)match[5].str().c_str(), (char*)arg_or_desc.c_str(), "");
						return;
					}
				}
				AddCmd((char*)match[5].str().c_str(), "", (char*)arg_or_desc.c_str());
			}
			else if (!match[2].str().empty()){
				AddCmd((char*)match[2].str().c_str(), (char*)match[3].str().c_str(), (char*)match[4].str().c_str());
			}
		}
		return;
	}

	( ( void (*)(int a1, char *text ) )0x4303A0 )( a1, text );
}

void h_DrawConsole()
{
	// call original
	( ( void (*)( ) )0x00405B90 )( );

	if (!client.onVodka || cmds.empty())
		return;

	// Получение текста в консоле
	std::string cmd = std::string(field->console.buffer);

	if (strncmp(cmd.c_str(), "//", 2) == 0) {
		cracking_write_text(0x00561B08, "Vodka: 5.1> \0", 13);

		std::string cmd_name = "", cmd_args = "";

		// Получение команды и аргументов
		cmd = cmd.substr(2, cmd.size()-2);
		unsigned int num_symbol = cmd.find(" ");

		if (num_symbol == -1) {
			cmd_name = cmd;
		}
		else {
			cmd_name = cmd.substr(0, num_symbol);
			cmd_args = cmd.substr(num_symbol+1, cmd.size());
		}

		cmd_name = clearMeta(cmd_name);
		cmd_args = clearMeta(cmd_args);

		// Получене списка команд
		std::string cmd_list = "^5", reg = "^"+cmd_name+(num_symbol==-1?".*":"$");
		int count_commands = 0, crnt_command = -1;
		std::regex pattern(reg.c_str(), std::regex_constants::icase);

		for (UINT i = 0; i < cmds.size(); i++) {
			if (std::regex_match(cmds[i].name, pattern)) {
				cmd_list += (count_commands!=0?std::string("\n"):"") + cmds[i].name;
				count_commands++;
				crnt_command = i;
			}
		}

		// Вывод всех команд
		if (count_commands > 24) {
			std::string txt = "^5" + std::to_string(count_commands) + " matches (too many to show)";
			DrawConsoleBox(1, (char*)txt.c_str());
		}
		else if (count_commands > 1) {
			DrawConsoleBox(count_commands, (char*)cmd_list.c_str());
		}
		else if (count_commands == 1) {
			std::string tab = "                 ^7";

			if (!cmds[crnt_command].args.empty())
			if (cmd_args.empty()) {
				tab += "<" + cmds[crnt_command].args + ">";
			}
			else {
				tab += "=> ";
				std::string arg = cmds[crnt_command].args;

				if (arg == "player") {
					int player_id = getID(cmd_args.c_str());

					if (player_id == -1)
						tab += "^1Error: player not found";
					else if (player_id == 65)
						tab += "^2Error: too many matches";
					else
						tab += ci[player_id].name;
				}
				else if (arg == "player id") {
					if (cmd_args >= "0" && cmd_args <= "64" || cmd_args == "7" || cmd_args == "8" || cmd_args == "9") {
						int player_id = atoi(cmd_args.c_str());

						if (!ci[player_id].infoValid)
							goto p_not_found;

						tab += ci[player_id].name;
					}
					else {
						p_not_found:
						tab += "^1Error: player not found";
					}
				}
				else {
					tab += cmd_args;
				}
			}

			std::string command = cmd_list + tab.substr(cmd_list.size(), tab.size()) + "\n^9" + cmds[crnt_command].desc;
			DrawConsoleBox(cmds[crnt_command].desc.empty()?1:2, (char*)command.c_str());
		}
	}
	else {
		cracking_write_text(0x00561B08, "%s: %s> \0\0\0\0C ", 13);
	}
}

bool _sort( command_t lhs, command_t rhs )
{
	return ( strcmp( rhs.name.c_str(), lhs.name.c_str() ) > 0 );
}

void mainloop()
{
	for (;; Sleep(100)) {
		if (client.readCommands && client.readTick < si->FrameCount) {
			client.readCommands = false;
			client.readTick = 0;

			if (!cmds.empty()) {
				AddStandartCmd(); // Добавление стандартных команд
				Sayline("^9Admin^7Vodka: you have ^2successfully ^9logged ^7in!");
			}

			std::sort(cmds.begin(), cmds.end(), _sort);
		}

		// Проверка что клиент на водке
		if (cg->clientFrame != 0 && strncmp(si->ServerName, "Cracked^5Vodka", 14) == 0) {
			cracking_hook_push((int)0x406893, (int)copyright);
			client.onVodka = true;
		}
		else {
			client.onVodka = false;
			cracking_hook_push((int)0x406893, (int)0x561AB8);
			cmds.clear();
		}

		// Автоматическая загрузка команд
		if (client.onVodka && cmds.empty() && (si->FrameCount % 20 == 0)) {
			client.readCommands = true;
			client.readTick = si->FrameCount + 10;
			SendCommand("/help");
		}

		if (client.spec) {
			if (cg->snap && inGame && *(int*)inMenu == 0 && cg->snap->ps.clientNum != client.spec_id) {
				if (cg->snap->ps.clientNum < client.spec_id) {
					keybd_event(VK_LBUTTON, 0, 0, 0);
					keybd_event(VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0);
				}
				else {
					keybd_event(VK_RBUTTON, 0, 0, 0);
					keybd_event(VK_RBUTTON, 0, KEYEVENTF_KEYUP, 0);
				}
			}
		}
	}
}

void main()
{
	while (GetModuleHandleA("gfx_d3d_mp_x86_s.dll") == 0)
		Sleep(100);

	// hooks functions
	cracking_hook_call((int)0x0041356D, (int)h_DrawConsole);
	cracking_hook_call((int)0x0040DDE9, (int)h_PrintInConsole);
	cracking_hook_call((int)0x0041FDB7, (int)h_SendCommandToServer);
	o_EndFrame = (t_EndFrame)DetourFunction( (PBYTE)0x4C5A40, (PBYTE)h_EndFrame );

	AddStandartCmd();

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainloop, 0, 0, 0);
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}

	return true;
}
