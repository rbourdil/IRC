#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

#define ERROR_TOKEN 0x0
#define CMD_L_TOKEN 0x1
#define CMD_D_TOKEN 0x2
#define SRV_NAME_TOKEN 0x4
#define PARAM_TOKEN 0x8
#define SPACE_TOKEN 0x10
#define NICK_TOKEN 0x20
#define CR_TOKEN 0x40
#define LF_TOKEN 0x80
#define COLON_TOKEN 0x100
#define EXCL_TOKEN 0x200
#define AT_TOKEN 0x400
#define CHSTRING_TOKEN 0x800
#define HASH_TOKEN 0x1000
#define DOLL_TOKEN 0x2000
#define AND_TOKEN 0x4000
#define COMMA_TOKEN 0x8000
#define NAME_TOKEN 0x10000
#define END_TOKEN 0x20000

// groups are used to match tokens more easily
// for ex: srv_name_grp is used to match server_name tokens
// since name and cmd_l are valid server_name tokens

#define SRV_NAME_GRP (SRV_NAME_TOKEN | NAME_TOKEN | CMD_L_TOKEN)
#define NICK_GRP (NICK_TOKEN | NAME_TOKEN | CMD_L_TOKEN)
#define CHSTRING_GRP (CHSTRING_TOKEN | SRV_NAME_GRP | NICK_GRP | CMD_D_TOKEN)
#define PARAM_GRP (PARAM_TOKEN | CHSTRING_GRP)

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

	bool	operator==(int type)
	{
		if ((_type & type) != 0)
			return (true);
		return (false);
	}

};

// DEBUG

std::string	map_type(int type);
std::ostream&	operator<<(std::ostream& os, const token& tok);

#endif
