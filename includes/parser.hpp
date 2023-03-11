#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "lexer.hpp"
#include "cmd_structs.hpp"

#define VALID_CMD 256
#define DUMP_CMD 257
#define INCOMPLETE_CMD 258

class parser {

	private:

		scanner	_scan;
		irc_cmd	_cmd;
		token	_current;
		int		_state;
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
				if (_current != EOF_TOKEN)
				{
					std::cerr << "unexpected token: " << _current << std::endl;
					_state = DUMP_CMD;
				}
				_panic = true;
			}
			else if (!_panic)
				_current = get_token(_scan);
		}

	public:

		parser(const Buffer& buff) : _scan(buff), _state(INCOMPLETE_CMD), _panic(false)
		{
			_current = get_token(_scan);
		}
		
		void	parse(void);

		void	skip_cmd(void)
		{
			while (_current != EOF_TOKEN)
			{
				if (_current == '\n')
				{
					_current = get_token(_scan);
					break ;
				}
			}
		}

		irc_cmd	out(void) { return (_cmd); }

		int	state(void) const
		{
			return (_state);
		}

		void	reset(void)
		{
			_cmd = irc_cmd();
			_panic = false;
		}

};

bool	isspecial(int c);
bool	valid_nickname(const std::string&);
bool	valid_username(const std::string&);
bool	valid_user_mode(const std::string&);
int		valid_channel_mode(const std::string&);
bool	match_mode_params(const std::vector<std::string>&);
bool	valid_channel(const std::string&);
bool	valid_key(const std::string&);

std::vector<std::string>	parse_list(const std::string&);

#endif
