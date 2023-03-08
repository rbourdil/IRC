#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

#define WORD_TOKEN 256
#define EOF_TOKEN 257

struct	token {

	int			_type;
	std::string	_lexeme;

	token(void) :
		_type(0),
		_lexeme()
		{ }

	token(int type) :
		_type(type),
		_lexeme()
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

	bool	operator==(int type) const
	{
		return (_type == type);
	}

	bool	operator!=(int type) const
	{
		return (_type != type);
	}

	// only used to print something for non printable characters
	std::string	print_lexeme(void) const
	{
		if (_type == ' ')
			return ("SPACE");
		else if (_type == '\r')
			return ("\\r");
		else if (_type == '\n')
			return ("\\n");
		else if (_type == EOF_TOKEN)
			return ("EOF");
		else
			return (_lexeme);
	}

};

// DEBUG

std::string	map_type(int type);
std::ostream&	operator<<(std::ostream& os, const token& tok);

#endif
