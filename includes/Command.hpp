#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>
#include <ctime>

#include "Data.hpp"
#include "cmd_structs.hpp"
#include "headers.hpp"
#include "parser.hpp"
#include "Reply.hpp"

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
		void	part(int fd, const std::string& channel, const std::string& message);
		void	channel_mode(int fd, const std::vector<std::string>& params);
		void	topic(int fd, std::string channel, const std::vector<std::string>& params);

		// helper functions
		void	topic_dispatch(int fd, const std::vector<std::string>& params);
		void	quit_dispatch(int fd, const std::vector<std::string>& params);
		void	join_dispatch(int fd, const std::vector<std::string>& params);
		void	part_dispatch(int fd, const std::vector<std::string>& params);
		void	mode_dispatch(int fd, const std::vector<std::string>& params);
		
		std::string	channel_mode_str(const std::string& channel);
		std::string	user_mode_str(int fd);

	public:

		Command(Data* data);

		void	execute_cmd(int fd, const irc_cmd& cmd);
				
};

std::string	get_id(void);

#endif
