#include "token.hpp"

std::string	map_type(int type)
{
	switch (type)
	{
		case '\0':
			return ("NUL");
		case '\r':
			return ("CR");
		case '\n':
			return ("LF");
		case ' ':
			return ("SPACE");
		case ':':
			return ("COLON");
		case WORD_TOKEN:
			return ("WORD_TOKEN");
		case EOF_TOKEN:
			return ("EOF_TOKEN");
	}
	return ("UNKNOWN_TOKEN");
}

std::ostream&	operator<<(std::ostream& os, const token& tok)
{
	os << map_type(tok._type) << " : " << tok.print_lexeme();
	return (os);
}
