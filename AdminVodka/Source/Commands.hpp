typedef struct {
	std::string name;
	std::string args;
	std::string desc;
} command_t;

std::vector<command_t> cmds;

void AddCmd(std::string name, std::string args, std::string desc );
void AddStandartCmd();

void AddStandartCmd()
{
	AddCmd("commands", "", "������������� ��������� ������ ������");
	AddCmd("spec", "player", "��� ���� ������ � ��������");
}

void AddCmd(std::string name, std::string args, std::string desc )
{
	command_t cmd;

	cmd.name = name;
	cmd.args = args;
	cmd.desc = desc;

	cmds.push_back(cmd);
}