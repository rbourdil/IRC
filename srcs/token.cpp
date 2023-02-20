#include "token.hpp"

std::string	map_type(int type)
{
	switch (type)
	{
		case ERROR_TOKEN:
			return ("ERROR_TOKEN");
			break;
		case CMD_TOKEN:
			return ("CMD_TOKEN");
			break;
		case PREFIX_TOKEN:
			return ("PREFIX_TOKEN");
			break;
		case PARAM_TOKEN:
			return ("PARAM_TOKEN");
			break;
		case END_TOKEN:
			return ("END_TOKEN");
			break;
	}
	return ("UNKNOWN_TOKEN");
}

std::ostream&	operator<<(std::ostream& os, const token& tok)
{
	os << map_type(tok._type) << " : " << tok._lexeme;
	return (os);
}
