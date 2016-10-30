#include "main.h"

std::vector<cmd_t> cmd;
std::vector<std::string> arg;

int *			client_num	= (int*)			0x01379004;
clientInfo_t *	ci			= (clientInfo_t *)	0x0145A914;

cSetting set;

bool ReadPass()
{
	char* pass = (char*)0x01339B24;

	if (strlen(pass) == 0) {
		Nanny(ERROR, "Unknown password. Write ^2save.rcon password");
		return false;
	}

	return true;
}

void Say(const char *msg, ...)
{
	va_list		argptr;
	char		msg_arg[MAX_PATH], text[MAX_PATH];

	std::string rcon;
	if (!set.Load(NANNY_SECTION, "rcon_say", (char*)rcon.c_str()))
		rcon = "say";

	va_start (argptr, msg);
	_vsnprintf_s<MAX_PATH> (msg_arg, MAX_PATH, msg, argptr);
	va_end (argptr);

	sprintf_s(text, "rcon %s \"%s\"", rcon.c_str(), msg_arg);

	__PRINT__::print::WritetoConsole(text);
}

void Console(const char *msg, ...)
{
	va_list		argptr;
	char		msg_arg[MAX_PATH], text[MAX_PATH];

	va_start (argptr, msg);
	_vsnprintf_s<MAX_PATH> (msg_arg, MAX_PATH, msg, argptr);
	va_end (argptr);

	sprintf_s(text, "rcon %s", msg_arg);

	__PRINT__::print::WritetoConsole(text);
}

/**
 * COMMANDS
 */
void cmd_time() // time
{
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);

	Nanny(NORMAL, "Time: ^3%s", asctime(timeinfo));
}

void cmd_id() // id
{
	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Nanny(NORMAL, "Client: ^2%s ^7Num: ^1%i", ci[id].name, id);
}

void cmd_kick() // kick
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: kick ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Say("^2%s ^7was kicked by an ^1admin", ci[id].name);
	Sleep(1000);
	Console("clientkick %i", id);
}

void cmd_fly() // fly
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("wtfoff %i", id);
	Sleep(500);
	Console("wtfon %i", id);
	Sleep(1000);
	Say("^2%s ^7fly", ci[id].name);
}

void cmd_flyoff() // flyoff
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("wtfoff %i", id);
	Sleep(1000);
	Say("^2%s ^7human", ci[id].name);
}

void cmd_pk() // pk
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("pkaon %i", id);
}

void cmd_pkoff() // pk
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("pkaoff %i", id);
}

void cmd_mute() // mute
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: mute ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Say("^2%s ^1muted", ci[id].name);
	Sleep(1000);
	Console("adm_chatoff %i", id);
}

void cmd_unmute() // unmute
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: unmute ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Say("^2%s ^1unmuted", ci[id].name);
	Sleep(1000);
	Console("adm_chaton %i", id);
}

void cmd_spec() // spec
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: spec ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("adm_tospec %i", id);
	Sleep(1000);
	Say("^2%s ^7gone to ^3bed", ci[id].name);
}

void cmd_play() // play
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: play ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("adm_toplay %i", id);
	Sleep(1000);
	Say("^2%s ^7plays", ci[id].name);
}

void cmd_kill() // kill
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: kill ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("adm_kill %i", id);
	Sleep(1000);
	Say("^2%s ^7was ^1killed", ci[id].name);
}

void cmd_spawn() // spawn
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("adm_swapteam %i", id);
}

void cmd_mortar() // mortar
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		Nanny(ERROR, "USAGE: mortar ^3nickname");
		return;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	const int count = 10;
	for (int i = 0; i < count; i++) {
		Console("adm_mortar %i", id);
		Sleep(100);
	}
}

void cmd_login() // login
{
	std::string rcon;
	if (!set.Load(NANNY_SECTION, "rcon_password", (char*)rcon.c_str()))
		rcon = "";

	char login[64];
	sprintf_s<64>(login, "rcon login %s", rcon.c_str());

	__PRINT__::print::WritetoConsole(login);
}

void cmd_fastman()
{
	if (!ReadPass())
		return;

	int id;

	if (args.empty()) {
		id = *client_num;
	}
	else {
		id = getID(args.c_str());

		if (id == -1) { Nanny(ERROR, "No matches: ^1%s", args.c_str()); return; }
		else if (id == 65) { Nanny(ERROR, "Too many matches: ^1%s",args.c_str()); return; }
	}

	Console("adm_swapteam %i", id);

	for (int i = 0; i < 4; i++) {
		Sleep(1000);
		Console("mon1000 %i", id);
	}
}

void cmd_cmdlist() // cmdlist
{
	if (cmd.empty()) {
		Nanny(ERROR, "Command list is empty");
		return;
	}

	__PRINT__::print::PrintConsole("^2] ^7Nanny ^3Command list:\n");
	for (UINT i = 0; i < cmd.size(); i++) {
		__PRINT__::print::PrintConsole("^5] ^7%s ^3%s\n", cmd[i].name.c_str(), cmd[i].desc.c_str());
	}
	__PRINT__::print::PrintConsole("^2] ^7by BratokHR\n");

	Nanny(AD, "To see command list press ^3Shift + ~");
}

void save_symbol() // save.symbol
{
	if (args.empty()) {
		Nanny(ERROR, "USAGE: save.symbol ^3symbol");
		return;
	}

	if (set.Save(NANNY_SECTION, "symbol", (char*)args.c_str()))
		Nanny(AD, "Saving is ^2successful^7, for more information press ^3Shift + ~");
}

void save_rcon() // save.rcon
{
	if (args.empty()) {
		Nanny(ERROR, "USAGE: save.rcon ^3password");
		return;
	}

	if (set.Save(NANNY_SECTION, "rcon_password", (char*)args.c_str()))
		Nanny(AD, "Saving is ^2successful^7, for more information press ^3Shift + ~");
}

void save_say() // save.say
{
	if (args.empty()) {
		Nanny(ERROR, "USAGE: save.say ^3say");
		return;
	}

	if (set.Save(NANNY_SECTION, "rcon_say", (char*)args.c_str()))
		Nanny(AD, "Saving is ^2successful^7, for more information press ^3Shift + ~");
}

/**
 * MAIN
 */
char* stripSpace(char* text)
{
	char ret_text[MAX_EDIT_LINE];
	char *p = &ret_text[0];

	for (char *a = text; *a; a++) {
		if (*a == ' ')
			continue;

		*p = *a;
		*p++;
	}
	*p = '\0';

	return (char*)ret_text;
}

char* stripColor(char* text)
{
	char ret_text[MAX_EDIT_LINE];
	char* _text = text, *p = &ret_text[0], *_p = &ret_text[0];
	bool gotcolor = true;

	while (gotcolor) {
		gotcolor = false;

		for (char *a = _text; *a; a++) {
			if (*a == '^' && (*(a+1) <= '9') && (*(a+1) >= '0')) {
				a++;
				gotcolor = true;
				continue;
			}

			*p = *a;
			p++;
		}
		*p = '\0';
		p = _p;
		_text = ret_text;
	}

	return (char*)ret_text;
}

const char *reg1[] = { "\\?",  "\\/",  "\\*",  "\\-",  "\\+",  "\\\\", "\\.",  "\\^",  "\\)",  "\\(",  "\\}",  "\\{",  "\\]",  "\\[",  "\\$",  "\\%",  "\\@"  };
const char *reg2[] = { "\\\?", "\\\/", "\\\*", "\\\-", "\\\+", "\\\\", "\\\.", "\\\^", "\\\)", "\\\(", "\\\}", "\\\{", "\\\]", "\\\[", "\\\$", "\\\%", "\\\@" };

std::string clearName(char* name)
{
	std::string new_name;

	new_name = std::string(stripSpace(name));
	new_name = std::string(stripColor((char*)new_name.c_str()));

	new_name = std::regex_replace(new_name.c_str(), (std::regex)reg1[0], reg2[0]).c_str();
	for (int i = 1; i < 17; i++) { // 17 - кол-во регулярных вырожений в массиве
		new_name = (char*)std::regex_replace(new_name.c_str(), (std::regex)reg1[i], reg2[i]).c_str();
	}

	return new_name;
}

int getID(const char *rgx_name)
{
	int count = 0, id = -1;
	std::string rgx = clearName((char*)rgx_name);
	std::regex pattern(rgx.c_str(), std::regex_constants::icase);

	for (int i = 0; i < 64; i++) {
		clientInfo_t *client = &ci[i];
		char* client_name = client->name;

		if (!strlen(client_name))
			continue;

		std::string name = clearName(client_name);

		std::cmatch reg_search; // dummy
		if (std::regex_search(name.c_str(), reg_search, pattern)) {
			id = client->clientNum;
			count++;
		}
	}

	if (count > 1)
		return 65;

	return id;
}

void Nanny(int msg, char *fmt, ...)
{
	va_list		argptr;
	char		text[MAX_SAY_TEXT];

	va_start (argptr, fmt);
	_vsnprintf_s<150> (text, 150, fmt, argptr);
	va_end (argptr);

	if (msg == ERROR)
		__PRINT__::print::Say("^2[^5Nanny^1:error^2]^7: %s", text);
	else if (msg == AD)
		__PRINT__::print::Say("^2[^5Nanny^3:ad^2]^7: %s", text);
	else if (msg == NORMAL)
		__PRINT__::print::Say("^2[^5Nanny^2]^7: %s", text);
}

void AddCommand(char *name, void* func, char* desc = "")
{
	cmd_t new_cmd;

	new_cmd.name = name;
	new_cmd.desc = desc;
	new_cmd.func = func;

	cmd.push_back(new_cmd);
}

void InitConmmand()
{
	// A
	// B
	// C
	AddCommand("cmdlist", (void*)cmd_cmdlist); // standart
	// D
	// E
	// F
	AddCommand("fastman", (void*)cmd_fastman, "^3<nickname>");
	AddCommand("fly", (void*)cmd_fly, "^3<nickname>");
	AddCommand("flyoff", (void*)cmd_flyoff, "^3<nickname>");
	// G
	// H
	// I
	AddCommand("id", (void*)cmd_id, "^3<nickname>"); // standart
	// K
	AddCommand("kick", (void*)cmd_kick, "^3<nickname>"); // standart
	AddCommand("kill", (void*)cmd_kill, "^3<nickname>");
	// L
	AddCommand("login", (void*)cmd_login);
	// M
	AddCommand("mortar", (void*)cmd_mortar, "^3<nickname>");
	AddCommand("mute", (void*)cmd_mute, "^3<nickname>");
	// N
	// O
	// P
	AddCommand("pk", (void*)cmd_pk, "^3<nickname>");
	AddCommand("pkoff", (void*)cmd_pkoff, "^3<nickname>");
	AddCommand("play", (void*)cmd_play, "^3<nickname>");
	// Q
	// R
	// S
	AddCommand("save.rcon", (void*)save_rcon, "^3<password>"); // standart
	AddCommand("save.say", (void*)save_say, "^3<say>"); // standart
	AddCommand("save.symbol", (void*)save_symbol, "^3<symbol>"); // standart
	AddCommand("spawn", (void*)cmd_spawn, "^3<nickname>"); // standart
	AddCommand("spec", (void*)cmd_spec, "^3<nickname>"); // standart
	// T
	AddCommand("time", (void*)cmd_time);
	// U
	AddCommand("unmute", (void*)cmd_unmute, "^3<nickname>");
	// V
	// X
	// Y
	// Z
}