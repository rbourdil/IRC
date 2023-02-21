#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

#define ERROR_TOKEN 0x0
#define CMD_L_TOKEN 0x1
#define CMD_D_TOKEN 0x2
#define SRV_NAME_TOKEN 0x4
#define PARAM_TOKEN 0x08
#define SPACE_TOKEN 0x20
#define NICK_TOKEN 0x40
#define CR_TOKEN 0x80
#define LF_TOKEN 0x100
#define COLON_TOKEN 0x200
#define EXCL_TOKEN 0x400
#define AT_TOKEN 0x800
#define CHSTRING_TOKEN 0x1000
#define HASH_TOKEN 0x2000
#define DOLL_TOKEN 0x4000
#define AND_TOKEN 0x8000
#define COMMA_TOKEN 0x10000
#define END_TOKEN 0x20000

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
