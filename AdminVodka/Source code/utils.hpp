extern clientInfo_t *	ci;

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

std::string clearMeta(std::string text)
{
	std::string new_text = std::regex_replace(text.c_str(), (std::regex)reg1[0], reg2[0]).c_str();
	for (int i = 1; i < 17; i++) { // 17 - кол-во регулярных вырожений в массиве
		new_text = (char*)std::regex_replace(new_text.c_str(), (std::regex)reg1[i], reg2[i]).c_str();
	}

	return new_text;
}

std::string clearName(char* name)
{
	std::string new_name;

	new_name = std::string(stripSpace(name));
	new_name = std::string(stripColor((char*)new_name.c_str()));
	new_name = clearMeta(new_name);

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