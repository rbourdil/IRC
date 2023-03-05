#ifndef CMD_STRUCTS_HPP
#define CMD_STRUCTS_HPP

#include <set>

struct	irc_cmd {

	std::string					_prefix;
	std::string					_cmd;
	std::vector<std::string>	_params;

};

struct	Reply {

	int				_rplnum;
	std::set<int>	_dest;
	std::string		_arg;

	Reply(void) : _rplnum(0) { }

};

#endif
