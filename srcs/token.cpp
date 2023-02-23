#include "token.hpp"

std::string	map_type(int type)
{
	switch (type)
	{
		case ERROR_TOKEN:
			return ("ERROR_TOKEN");
		case CMD_L_TOKEN:
			return ("CMD_L_TOKEN");
		case CMD_D_TOKEN:
			return ("CMD_D_TOKEN");
		case SRV_NAME_TOKEN:
			return ("SRV_NAME_TOKEN");
		case PARAM_TOKEN:
			return ("PARAM_TOKEN");
		case SPACE_TOKEN:
			return ("SPACE_TOKEN");
		case NICK_TOKEN:
			return ("NICK_TOKEN");
		case CR_TOKEN:
			return ("CR_TOKEN");
		case LF_TOKEN:
			return ("LF_TOKEN");
		case COLON_TOKEN:
			return ("COLON_TOKEN");
		case EXCL_TOKEN:
			return ("EXCL_TOKEN");
		case AT_TOKEN:
			return ("AT_TOKEN");
		case CHSTRING_TOKEN:
			return ("CHSTRING_TOKEN");
		case HASH_TOKEN:
			return ("HASH_TOKEN");
		case DOLL_TOKEN:
			return ("DOLL_TOKEN");
		case AND_TOKEN:
			return ("AND_TOKEN");
		case COMMA_TOKEN:
			return ("COMMA_TOKEN");
		case END_TOKEN:
			return ("END_TOKEN");
	}
	return ("UNKNOWN_TOKEN");
}

std::ostream&	operator<<(std::ostream& os, const token& tok)
{
	os << map_type(tok._type) << " : " << tok.print_lexeme();
	return (os);
}
