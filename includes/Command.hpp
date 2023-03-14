#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>
#include <ctime>

#include "Data.hpp"
#include "cmd_structs.hpp"
#include "headers.hpp"
#include "parser.hpp"
#include "Reply.hpp"

struct Scanner {
	std::string::iterator _start;
	std::string::iterator _current;
	
	Scanner() : _start(), _current() {}
};

class	Command {

	typedef void (Command::*FuncPtr)(int, const std::vector<std::string>&);

	private:
		
		Data*							_data;
		std::map<std::string, FuncPtr>	_cmd_map;
		std::vector<int>				_dests;
		std::vector<std::string>		_args;

		// user commands
		void	pass(int fd, const std::vector<std::string>& params);
		void	nick(int fd, const std::vector<std::string>& params);
		void	user(int fd, const std::vector<std::string>& params);
		void	user_mode(int fd, const std::vector<std::string>& params);

		// channel commands
		void	join(int fd, std::string channel, std::string key);
		void	topic(int fd, const std::vector<std::string>& params);
		void	names(int fd, const std::vector<std::string>& params);
		void	channel_mode(int fd, const std::vector<std::string>& params);
		void	part(int fd, const std::string& channel, const std::string& message);
		void	pong(int fd, const std::vector<std::string>& params);
		void	privmsg(int fd, const std::vector<std::string>& params);
		void	list(int fd, const std::vector<std::string>& params);
		void	notice(int fd, const std::vector<std::string>& params);
		void	kick(int fd, const std::string& channel, const std::string& user, const std::string& comment);

		// helper functions
		void	quit_dispatch(int fd, const std::vector<std::string>& params);
		void	join_dispatch(int fd, const std::vector<std::string>& params);
		void	part_dispatch(int fd, const std::vector<std::string>& params);
		void	mode_dispatch(int fd, const std::vector<std::string>& params);
		void	kick_dispatch(int fd, const std::vector<std::string>& params);
		void	send_to_chan(int fd, std::string &channel, std::string &message);
		void	send_to_nick(int fd, std::string &nick, std::string &message);
		int		parse_target(int fd, std::string &target);
		int		parse_target_notice(std::string &target);
		void	send_to_chan_notice(int fd, std::string &channel, std::string &message);
		void	send_to_nick_notice(int fd, std::string &nick, std::string &message);
		bool	is_valid_host(std::string &host);
		
		std::string	channel_mode_str(const std::string& channel);
		std::string	user_mode_str(int fd);

	public:

		Command(Data* data);

		void	execute_cmd(int fd, const irc_cmd& cmd);
				
};

std::string	get_id(void);

#endif
