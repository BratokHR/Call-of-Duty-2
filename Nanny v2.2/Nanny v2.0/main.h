#include <Windows.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <regex>
#include <cstdio>

#include "PrintEngine\cPrintEngine.h"
#include "DrawEngine\cDraw.h"

#include "cMemory.h"
#include "cSettings.h"

#define ERROR					0
#define AD						1
#define	NORMAL					2

#define	MAX_EDIT_LINE			256

#define NANNY_SECTION			"Nanny by BratokHR"
#define NANNY_INI				"nanny.ini"

void InitConmmand();
void Nanny(int msg, char *fmt, ...);
int getID(const char *rgx);

typedef struct {
	std::string	name;
	std::string	desc;
	void *		func;
} cmd_t;

typedef struct {
	int			infoValid;
	int			unknown;
	int			clientNum;
	char		name[32];
	int			team; 
	int			pad[4];
	char		bodymodel[64];
	char		headmodel[64];
	char		helmetmodel[64];

	int			pad1[203];
	int *		pointer;
	int			pad2[11];

	int			movement;

	int			pad3[3];

	int			usingMountedWeapon;

	int			pad4;

	int			pose;

	int			pad5[11];

	int *		unknown_ptr;
	int			curWeapon;	

	int			pad6[3];
} clientInfo_t;

typedef struct {
	int		cursor;
	int		widthChars1;
	int		widthChars2;
	int		pad2;
	float	pad3;
	int		pad4;
	char	buffer[MAX_EDIT_LINE];
} mfield_t;

typedef struct {
	mfield_t	console;
	int			pad[2];
	mfield_t	chat;
} field_t;

typedef struct {
	int		pad0;
	float	width;
	float	height;
	float	yOfset;
} window_t;

extern std::vector<cmd_t> cmd;
extern std::vector<std::string> arg;
extern cSetting set;
extern std::string args;