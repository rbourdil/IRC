#include "lexer.hpp"

token	lexer::get_token(void)
{
	int	c;

	c = _scan.getc();
	if (c == ':')
	{
		while (_scan.peek() != ' ')
		{
			c = _scan.getc();
			_scan.addc(c);
		}
		std::string	lexeme = _scan.flush();
		while (_scan.peek() == ' ')
			_scan.getc();
		return (token(PREFIX_TOKEN, lexeme));
	}
	else if (isalpha(c))
	{
		_scan.addc(c);
		while (isalpha(_scan.peek()))
		{
			c = _scan.getc();
			_scan.addc(c);
		}
		return (token(CMD_TOKEN, _scan.flush()));
	}
	else if (isdigit(c))
	{
		_scan.addc(c);
		while (isdigit(_scan.peek()))
		{
			c = _scan.getc();
			_scan.addc(c);
		}
		return (token(CMD_TOKEN, _scan.flush()));
	}
	else if (c == ' ')
	{
		while ((c = _scan.getc()) == ' ')
			;
		if (c == ':')
		{
			while (_scan.peek() != '\0' && _scan.peek() != CR && _scan.peek() != LF)
			{
				c = _scan.getc();
				_scan.addc(c);
			}
			return (token(PARAM_TOKEN, _scan.flush()));
		}
		else
		{
			_scan.addc(c);
			while (_scan.peek() != '\0' && _scan.peek() != ' ' && _scan.peek() != CR && _scan.peek() != LF)
			{
				c = _scan.getc();
				_scan.addc(c);
			}
			return (token(PARAM_TOKEN, _scan.flush()));
		}
	}
	else if (c == CR)
	{
		c = _scan.getc();
		if (c == LF)
			return (token(END_TOKEN));
		else
			return (token(ERROR_TOKEN));
	}
	else
		return (token(ERROR_TOKEN));
}
