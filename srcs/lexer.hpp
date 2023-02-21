#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstdlib> // to remove with switch 'default' case

#include "scanner.hpp"
#include "token.hpp"

#define START_STATE 256
#define ACCEPT_STATE 257
#define SPACE_STATE 258
#define CMD_L_STATE 259
#define CMD_D_STATE 260
#define CMD_D_STATE1 261
#define CMD_D_STATE2 262
#define PARAM_STATE 263
#define POINT_STATE 264
#define NICK_STATE 265
#define SRV_NAME_STATE 266
#define CHSTRING_STATE 267
#define END_STATE 268

#define CR 0xd
#define LF 0xa
#define BELL 0x7

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
