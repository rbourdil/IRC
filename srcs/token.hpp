#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

#define ERROR_TOKEN 256
#define PREFIX_TOKEN 257
#define CMD_TOKEN 258
#define PARAM_TOKEN 259
#define END_TOKEN 260

struct	token {

	int			_type;
	std::string	_lexeme;

	token(void) { }

	token(int type) :
		_type(type)
		{ }
	
	token(int type, const std::string& lexeme) :
		_type(type),
		_lexeme(lexeme)
		{ }
	
	token(const token& from) :
		_type(from._type),
		_lexeme(from._lexeme)
		{ }
	
	token&	operator=(const token& from)
	{
		_type = from._type;
		_lexeme = from._lexeme;
		return (*this);
	}
	
};

// DEBUG

std::string	map_type(int type);

std::ostream&	operator<<(std::ostream& os, const token& tok);

#endif
