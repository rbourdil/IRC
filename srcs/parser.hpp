#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "lexer.hpp"

#define INCOMPLETE_CMD 256
#define COMPLETE_CMD 257
#define DUMP_CMD 258

struct	irc_cmd {

	int							_state;
	std::string					_prefix;
	std::string					_cmd;
	std::vector<std::string>	_params;

	irc_cmd(void) :
		_state(INCOMPLETE_CMD),
		_prefix(),
		_cmd(),
		_params()
		{ }
	
	irc_cmd(const irc_cmd& from) :
		_state(from._state),
		_prefix(from._prefix),
		_cmd(from._cmd),
		_params(from._params)
		{ }
	
	irc_cmd&	operator=(const irc_cmd& from)
	{
		_state = from._state;
		_prefix = from._prefix;
		_cmd = from._cmd;
		_params = from._params;
		return (*this);
	}
	
};

class	parser {

	// wrapper struct to facilitate use of lexer
	struct	tokenizer {

		lexer							_lex;
		std::vector<token>				_backlog;
		std::vector<token>::iterator	_it;

		tokenizer(const char* message) :
			_lex(message),
			_backlog(),
			_it(_backlog.begin())
			{ }

		void	load(const char* message)
		{
			_lex.load(message);
		}
		
		token	get_token(void)
		{
			if (_it == _backlog.end())
			{
				token	tmp = _lex.get_token();
				_backlog.push_back(tmp);
				_it = _backlog.end();
				return (tmp);
			}
			else
				return (*_it++);
		}

		void	unget_token(void)
		{
			if (_it != _backlog.begin())
				--_it;
		}

		void	pop_token(void)
		{
			_backlog.pop_back();
			_it = _backlog.end();
		}
		
		void	reload(void)
		{
			_it = _backlog.begin();
		}

		void	clear(void)
		{
			std::vector<token>	tmp(_it, _backlog.end());
			_backlog = tmp;
			_it = _backlog.begin();
		}

		// DEBUG

		void	print_backlog(void)
		{
			std::vector<token>::iterator	it;
			for (it = _backlog.begin(); it != _backlog.end(); it++)
				std::cerr << *it << std::endl;
		}
	};

	bool				_reload;
	irc_cmd				_irc_cmd;
	tokenizer			_tok;
	token				_current;

	void	match(int types);
	void	get_prefix(void);
	void	get_cmd(void);
	void	get_params(void);
	void	get_crlf(void);


	public:
		
	parser(const char* message = NULL) :
		_reload(false),
		_irc_cmd(),
		_tok(message),
		_current()
		{ }

	irc_cmd	out(void) const
	{
		return (_irc_cmd);
	}

	int	cmd_state(void) const
	{
		return (_irc_cmd._state);
	}

	void	reload(void)
	{
		_tok.reload();
	}

	void	reset(void)
	{
		_reload = false;
		_irc_cmd = irc_cmd();
		_tok.clear();
	}

	void	load(const char* message)
	{
		_tok.load(message);
		_reload = false;
	}

	void	get_token(void)
	{
		_current = _tok.get_token();
	}

	void	unget_token(void)
	{
		_tok.unget_token();
	}

	void	pop_token(void)
	{
		_tok.pop_token();
	}

	void	parse(void);

	// a 'dump' state cannot be changed
	void	update_state(int state)
	{
		if (_irc_cmd._state != DUMP_CMD && !_reload)
			_irc_cmd._state = state;
	}
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
