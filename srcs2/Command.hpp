#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>

#include "Data.hpp"
#include "cmd_structs.hpp"

// REPLIES
#define RPL_WELCOME 1
#define RPL_UMODEIS 221

// ERROR REPLIES
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_RESTRICTED 484
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502

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

		void	pass(int fd, const std::vector<std::string>& params)
		{
			add_dest(fd);
			if (_data->is_registered(fd))
				set_rplnum(ERR_ALREADYREGISTERED);
			else if (params.size() == 0)
				set_rplnum(ERR_NEEDMOREPARAMS);
			else if (!_data->compare_passwd(params[0]))
				set_rplnum(ERR_PASSWDMISMATCH);
			else
			{
				_data->set_user_state(fd, PASSWD_VALID);
				if (_data->is_registered(fd))
				{
					set_rplnum(RPL_WELCOME);
					set_arg(_data->get_user_info(fd));
				}
			}
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
			else if (_data->nickname_exists(params[0]))
			{
				set_rplnum(ERR_NICKNAMEINUSE);
				set_arg(params[0]);
			}
			else if (_data->check_user_flags(fd, RESTRICTED_UFLAG))
				set_rplnum(ERR_RESTRICTED);
			else
			{
				_data->add_nickname(fd, params[0]);
				_data->set_user_state(fd, NICK_VALID);
				if (_data->is_registered(fd))
				{
					set_rplnum(RPL_WELCOME);
					set_arg(_data->get_user_info(fd));
				}
			}
		}

		void	user(int fd, const std::vector<std::string>& params)
		{
			add_dest(fd);
			if (_data->is_registered(fd))
				set_rplnum(ERR_ALREADYREGISTERED);
			else if (params.size() < 4)
				set_rplnum(ERR_NEEDMOREPARAMS);
			else
			{
				_data->add_username(fd, params[0]);
				if (params[1] == "4")
					_data->set_user_flags(fd, WALLOPS_UFLAG);
				else if (params[1] == "8")
					_data->set_user_flags(fd, INVISIBLE_UFLAG);
				_data->add_realname(fd, params[3]);
				_data->set_user_state(fd, USER_VALID);
				if (_data->is_registered(fd))
				{
					set_rplnum(RPL_WELCOME);
					set_arg(_data->get_user_info(fd));
				}
			}
		}

		void	mode(int fd, const std::vector<std::string>& params)
		{
			add_dest(fd);
			if (!_data->is_registered(fd))
				set_rplnum(ERR_NOTREGISTERED);
			else if (params.size() < 1)
				set_rplnum(ERR_NEEDMOREPARAMS);
			else if (params[0] != _data->get_nickname(fd))
				set_rplnum(ERR_USERSDONTMATCH);
			else if (params.size() == 1)
			{
				set_rplnum(RPL_UMODEIS);
				set_arg(mode_str(fd));
			}
			else if (!valid_mode(params[1]))
				set_rplnum(ERR_UMODEUNKNOWNFLAG);
			else
			{
				std::string::const_iterator	it = params[1].begin();
				int							flags = 0;
				bool						add = false;

				if (*it == '+')
					add = true;
				while (it != params[1].end())
				{
					switch (*it)
					{
						case 'i':
							flags |= INVISIBLE_UFLAG;
							break;
						case 'w':
							flags |= WALLOPS_UFLAG;
							break;
						case 'o':
							if (!add)
								flags |= OPER_UFLAG;
							break;
						case 'O':
							if (!add)
								flags |= LOCAL_OPER_UFLAG;
							break;
						case 'r':
							if (add)
								flags |= RESTRICTED_UFLAG;
							break;
					}
					it++;
				}
				if (add)
					_data->set_user_flags(fd, flags);
				else
					_data->unset_user_flags(fd, flags);
				set_rplnum(RPL_UMODEIS);
				set_arg(mode_str(fd));
			}
		}

		// helper functions

		std::string	mode_str(int fd)
		{
			std::string	ret;

			ret += _data->get_nickname(fd);
			ret += " ";
			ret += _data-> get_user_flags_str(fd);
			return (ret);
		}

	public:

		Command(Data* data) : _data(data)
		{
			_cmd_map.insert(std::make_pair("PASS", &Command::pass));
			_cmd_map.insert(std::make_pair("NICK", &Command::nick));
			_cmd_map.insert(std::make_pair("USER", &Command::user));
			_cmd_map.insert(std::make_pair("MODE", &Command::mode));
		}

		Reply	out(void) const
		{
			return (_rpl);
		}

		void	reset(void) // to do after each call of 'execute_cmd'
		{
			_rpl._rplnum = 0;
			_rpl._dest.clear();
			_rpl._arg.clear();
		}

		void	execute_cmd(int fd, const irc_cmd& cmd)
		{
			std::map<std::string, FuncPtr>::iterator	it = _cmd_map.find(cmd._cmd);

			if (it != _cmd_map.end()) // found a valid command
				(this->*(it->second))(fd, cmd._params);	
			else
			{
				add_dest(fd);
				set_arg(cmd._cmd);
				set_rplnum(ERR_UNKNOWNCOMMAND);
			}
		}
				
};

#endif
