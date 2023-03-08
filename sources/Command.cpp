#include "Command.hpp"

void	Command::pass(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (_data->is_registered(fd))
		err_already_registered(fd, _args);
	else if (params.size() == 0)
	{
		_args.push_back("PASS");
		err_need_moreparams(fd, _args);
	}
	else if (!_data->compare_passwd(params[0]))
		err_passwd_mismatch(fd, _args);
	else
	{
		_data->set_user_state(fd, PASSWD_VALID);
		if (_data->is_registered(fd))
		{
			_args.push_back(_data->get_user_info(fd));
			rpl_welcome_message(fd, _args);
		}
	}
}

void	Command::nick(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (params.size() == 0)
		err_no_nicknamegive(fd, _args);
	else if (!valid_nickname(params[0]))
	{
		_args.push_back(params[0]);
		err_erroneus_nickname(fd, _args);
	}
	else if (_data->nickname_exists(params[0]))
	{
		_args.push_back(params[0]);
		err_nickname_inuse(fd, _args);
	}
	else if (_data->check_user_flags(fd, RESTRICTED_UFLAG))
		err_restricted(fd, _args);
	else
	{
		_data->add_nickname(fd, params[0]);
		_data->set_user_state(fd, NICK_VALID);
		if (_data->is_registered(fd))
		{
			_args.push_back(_data->get_user_info(fd));
			rpl_welcome_message(fd, _args);
		}
	}
}

void	Command::user(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (_data->is_registered(fd))
		err_already_registered(fd, _args);
	else if (params.size() < 4)
	{
		_args.push_back("USER");
		err_need_moreparams(fd, _args);
	}
	else if (!valid_username(params[0]))
	{
		_args.push_back(params[0]);
		err_erroneus_nickname(fd, _args);
	}
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
			_args.push_back(_data->get_user_info(fd));
			rpl_welcome_message(fd, _args);
		}
	}
}

void	Command::mode(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (!_data->is_registered(fd))
		err_not_registered(fd, _args);
	else if (params.size() < 1)
	{
		_args.push_back("MODE");
		err_need_moreparams(fd, _args);
	}
	else if (params[0] != _data->get_nickname(fd))
		err_users_dontmatch(fd, _args);
	else if (params.size() == 1)
	{
		_args.push_back(mode_str(fd));
		rpl_umodeis(fd, _args);
	}
	else if (!valid_mode(params[1]))
		err_umode_unknownflag(fd, _args);
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
		_args.push_back(mode_str(fd));
		rpl_umodeis(fd, _args);
	}
}

void	Command::quit(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (params.size() > 0)
		_args.push_back(params[0]);
	// call error quit function
}

void	Command::join(int fd, std::string channel, std::string key)
{
	if (!valid_channel(channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_nosuch_channel(fd, _args);
	}
	else if (!key.empty() && !valid_key(key))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(key);
		err_badchannel_key(fd, _args);
	}
	else if (channel == "0")
	{
		const std::set<std::string>&	channel_list = get_user_channel_list(fd);
		std::set<std::string>::const_iterator	it = channel_list.begin();
		while (it != channel_list.end())
		{
			part(fd, *it);
			it++;
		}
	}
	else if (_data->user_channel_count(fd) > MAX_CHAN_COUNT)
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_toomany_channels(fd, _args);
	}
	else if (!_data->channel_exists(channel))
	{
		if (channel[0] == '!')
		{
			std::string	id = get_id();
			channel.insert(1, id);
		}
		_data->add_channel(channel, TOPIC_OPER_CFLAG);
		_data->add_user_to_channel(fd, channel);
		if (channel[0] != '+')
			_data->set_member_status(channel, fd, OPER_MFLAG);
		if (channel[0] == '!')
			_data->set_member_status(channel, fd, CREATOR_MFLAG);
		_args.push_back(_data->get_nickname(fd));
		_args.push_back(channel);
		join_reply(fd, _args);
		rpl_notopic(fd, _args);
		rpl_nam_reply(fd, _args); // need to modify prototype to accept maps
	}
	else if (_data->check_channel_flags(channel, INVITE_ONLY_CFLAG))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_invite_onlychan(fd, _args);
	}
	else if (_data->channel_is_full(channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_channel_isfull(fd, _args);
	}
	else if (!_data->channel_authenticate(channel, key))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_badchannel_key(fd, _args);
	}
	else // user joins an existing channel
	{
		_data->add_user_to_channel(fd, channel);
		std::vector<int>	members = get_members_list_fd(channel);	
		std::vector<int>	it = members.begin();
		_args.push_back(_data->get_nickname(fd));
		_args.push_back(channel);
		for (; it != members.end(); it++)
		{
			join_reply(*it, _args);
			// need to broadcast the news


	// implement ban masks
	
// helper functions

void	Command::join_dispatch(int fd, const std::vector<std::string>& params)
{
	if (!_data->is_registered(fd))
	{
		_args.push_back(_data->get_srvname());
		err_not_registered(fd, _args);
	}
	else if (params.size() < 1)
	{
		_args.push_back("JOIN");
		err_need_moreparams(fd, _args);
	}
	else
	{
		std::vector<std::string>	channels = parse_list(params[0]);
		std::vector<std::string>	keys;
		if (params.size() == 2)
			keys = parse_list(params[1]);
		std::vector<std::string>::iterator	itc, itk;
		for (itc = channels.begin(), itk = keys.begin(); itc != channels.end(), itk != keys.end(); itc++, itk++)
		{
			join(fd, *itc, *itk);
		}
		for (; itc != channels.end(); itc++)
			join(fd, *itc, std::string());
	}	
}

std::string	Command::mode_str(int fd)
{
	std::string	ret;

	ret += _data->get_nickname(fd);
	ret += " ";
	ret += _data-> get_user_flags_str(fd);
	return (ret);
}

std::string	get_id(void)
{
	time_t			now = time(NULL);
	std::string		id;
	std::string		base("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	char			c;
	
	for (int count = 0; count < 5; count++)
	{
		c = base[now % 36];
		id.push_back(c);
		now /= 36;
	}
	return (id);
}

// INTERFACE

Command::Command(Data* data) : _data(data)
{
	// add commands to the command tree
	_cmd_map.insert(std::make_pair("PASS", &Command::pass));
	_cmd_map.insert(std::make_pair("NICK", &Command::nick));
	_cmd_map.insert(std::make_pair("USER", &Command::user));
	_cmd_map.insert(std::make_pair("MODE", &Command::mode));
	_cmd_map.insert(std::make_pair("QUIT", &Command::quit));
	_cmd_map.insert(std::make_pair("JOIN", &Command::join_dispatch));
}

Reply	Command::out(void) const
{
	return (_rpl);
}

void	Command::reset(void) // to do after each call of 'execute_cmd'
{
	_rpl._rplnum = 0;
	_rpl._dest.clear();
	_rpl._args.clear();
}

void	Command::execute_cmd(int fd, const irc_cmd& cmd)
{
	std::map<std::string, FuncPtr>::iterator	it = _cmd_map.find(cmd._cmd);

	if (it != _cmd_map.end()) // found a valid command
		(this->*(it->second))(fd, cmd._params);	
	else
	{
		add_dest(fd);
		add_arg(cmd._cmd);
		set_rplnum(ERR_UNKNOWNCOMMAND);
	}
}
