#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "lexer.hpp"
#include "cmd_structs.hpp"

#define VALID_CMD 256
#define DUMP_CMD 257
#define STOP_PARSE 258

class parser {

	private:

		scanner	_scan;
		irc_cmd	_cmd;
		token	_current;
		bool	_panic;

		void	update_prefix(void) {_cmd. _prefix = _current._lexeme; }
		void	update_cmd(void) { _cmd._cmd = _current._lexeme; }
		void	update_params(void) { _cmd._params.push_back(_current._lexeme); }
		void	merge_params(void) { _cmd._params.back() += _current._lexeme; }
		size_t	nb_of_params(void) const { return (_cmd._params.size()); }

		void	get_prefix(void);
		void	get_cmd(void);
		void	get_params(void);
		void	get_trailing(void);
		void	get_crlf(void);

		void	match(int type)
		{
			if (_current != type && !_panic)
			{
				std::cerr << "unexpected token: " << _current << std::endl;
				_panic = true;
			}
			else if (!_panic)
				_current = get_token(_scan);
		}

	public:

		parser(int fd) : _scan(fd), _panic(false)
		{
			_current = get_token(_scan);
		}
		
		void	parse(void);

		void	skip_cmd(void)
		{
			while (_current != EOF_TOKEN)
			{
				if (_current == '\r')
				{
					_current = get_token(_scan);
					if (_current == '\n')
					{
						_current = get_token(_scan);
						break;
					}
				}
				if (_current != '\r')
					_current = get_token(_scan);
			}
		}

		irc_cmd	out(void) { return (_cmd); }

		int	state(void) const
		{
			if (_panic && _current == EOF_TOKEN)
				return (STOP_PARSE);
			else if (_panic)
				return (DUMP_CMD);
			return (VALID_CMD);
		}

		void	reset(void)
		{
			_cmd = irc_cmd();
			_panic = false;
		}

};

bool	isspecial(int c);
bool	valid_nickname(const std::string& nick);
bool	valid_mode(const std::string& mode);

#endif
