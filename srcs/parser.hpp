#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "lexer.hpp"

struct	irc_cmd {

	std::string					_prefix;
	std::string					_cmd;
	std::vector<std::string>	_params;

	irc_cmd(void) :
		_prefix(),
		_cmd(),
		_params()
		{ }
	
	irc_cmd(const irc_cmd& from) :
		_prefix(from._prefix),
		_cmd(from._cmd),
		_params(from._params)
		{ }
	
	irc_cmd&	operator=(const irc_cmd& from)
	{
		_prefix = from._prefix;
		_cmd = from._cmd;
		_params = from._params;
		return (*this);
	}
	
};

class	parser {

	irc_cmd				_irc_cmd;
	lexer				_lex;
	std::vector<token>	_token_list;
	token				_current;

	void	match(int types);
	void	get_prefix(void);
	void	get_cmd(void);
	void	get_params(void);
	void	get_crlf(void);

	public:
		
	parser(const char* message) :
		_irc_cmd(),
		_lex(message),
		_token_list()
		{ }
	
	irc_cmd	out(void)
	{
		return (_irc_cmd);
	}

	void	load(const char* message)
	{
		_lex.load(message);
	}

	void	get_token(void)
	{
		_current = _lex.get_token();
		_token_list.push_back(_current);
	}

	void	parse(void);

	void	update_prefix(void)
	{
		_irc_cmd._prefix = _current._lexeme;
	}

	void	merge_prefix(void)
	{
		_irc_cmd._prefix += _current._lexeme;
	}

	void	update_cmd(void)
	{
		_irc_cmd._cmd = _current._lexeme;
	}

	void	merge_cmd(void)
	{
		_irc_cmd._cmd += _current._lexeme;
	}

	void	update_params(void)
	{
		_irc_cmd._params.push_back(_current._lexeme);
	}

	void	merge_params(void)
	{
		_irc_cmd._params.back() += _current._lexeme;
	}

};

//DEBUG

void	print_irc_cmd(const irc_cmd& cmd);

#endif
