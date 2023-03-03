#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>

#include "Data.hpp"

#define ERR_NONICKNAMEGIVEN 431
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_RESTRICTED 484

struct	Reply {

	int				_rplnum;
	std::set<int>	_dest;
	std::string		_arg;

};

class	Command {

	typedef void (Command::*FuncPtr)(int, const std::vector<std::string>&);

	private:
		
		Data*							_data;
		Reply							_rpl;
		std::map<std::string, FuncPtr>	_cmd_map;

		void	set_rplnum(int rplnum)
		{
			_rpl._rplnum = rplnum;
		}

		void	add_dest(int dest)
		{
			_rpl._dest.insert(dest);
		}

		void	set_arg(const std::string& arg)
		{
			_rpl._arg = arg;
		}

		void	reset(void)
		{
			_rpl._rplnum = 0;
			_rpl._dest.clear();
			_rpl._arg.clear();
		}

		void	pass(int fd, const std::vector<std::string>& params)
		{
			add_dest(fd);
			if (params.size() == 0)
				set_rplnum(ERR_NEEDMOREPARAMS);
			else if (_data->isregistered(fd))
				set_rplnum(ERR_ALREADYREGISTRED);
			else if (!_data->compare_passwd(params[0]))
				set_rplnum(ERR_PASSWDMISMATCH);
			else
				_data->set_user_state(fd, PASSWD_VALID);
		}

		void	nick(int fd, const std::vector<std::string>& params)
		{
			add_dest(fd);
			if (params.size() == 0)
				set_rplnum(ERR_NONICKNAMEGIVEN);
			else if (!valid_nickname(params[0]))
			{
				set_rplnum(ERR_ERRONEUSNICKNAME);
				set_arg(params[0]);
			}
			else if (_data.nickname_exists(params[0]))
			{
				set_rplnum(ERR_NICKNAMEINUSE);
				set_arg(params[0]);
			}
			else if (check_user_flags(fd, RESTRICTED_UFLAG))
				set_rplnum(ERR_RESTRICTED);
			else
			{
				_data->add_nickname(fd, params[0]);
				_data->set_user_state(fd, NICK_VALID);
			}
		}

		void	user(int fd, const std::vector<std::string>& params)
		{
			add_dest(fd);
			if (params.size() < 4)
				set_rplnum(ERR_NEEDMOREPARAMS);
			else if (_data->isregistered(fd))
				set_rplnum(ERR_ALREADYREGISTRED);
			else
			{
				_data->add_username(fd, params[0]);
				if (params[1] == '4')
					set_user_flags(fd, WALLOPS_UFLAG);
				else if (params[1] == '8')
					set_user_flags(fd, INVISIBLE_UFLAG);
				_data->add_realname(fd, params[4]);
				_data->set_user_state(fd, USER_VALID);
			}
		}

	public:

		Command(void)
		{
			_cmd_map.insert(std::make_pair("PASS", pass));
			_cmd_map.insert(std::make_pair("NICK", nick));
			_cmd_map.insert(std::make_pair("USER", user));
		}
