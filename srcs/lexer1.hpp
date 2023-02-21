#ifndef LEXER_HPP
#define LEXER_HPP

#include "scanner.hpp"
#include "token.hpp"

#define START_STATE
#define 
#define CR 0xd
#define LF 0xa

class	lexer {

	scanner	_scan;
	int		_state;
	int		_type;

	public:

	lexer(const char* message) :
		_scan(message),
		_state(START_STATE),
		_type(END_TOKEN)
	{ }

	void	set_state(int state)
	{
		_state = state;
	}

	int	get_state(void) const
	{
		return (_state);
	}

	void	set_type(int type)
	{
		_type = type;
	}

	int	get_type(void) const
	{
		return (_type);
	}
	
	token	get_token(void);

};

#endif
