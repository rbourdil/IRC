#include "lexer.hpp"

token	get_token(scanner& scan)
{
	int			c;
	std::string	lexeme;

	c = scan.get();
	if (c == _EOF_)
		return (token(EOF_TOKEN));
	else if (c == '\0')
		return (token('\0'));
	else if (c == '\r')
		return (token('\r'));
	else if (c == '\n')
		return (token('\n'));
	else if (c == ' ')
		return (token(' ', " "));
	else if (c == ':')
		return (token(':'));
	while (c != _EOF_ && c != '\0' && c != '\r' && c != '\n' && c != ' ')
	{
		lexeme.push_back(c);
		c = scan.get();
	}
	if (c != _EOF_)
		scan.unget();
	return (token(WORD_TOKEN, lexeme));
}
