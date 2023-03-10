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
		const std::set<std::string>&	channel_list = _data->get_user_channel_list(fd);
		std::set<std::string>::const_iterator	it = channel_list.begin();
		while (it != channel_list.end())
		{
			part(fd, *it, std::string());
			it++;
		}
	}
	else if (_data->user_channel_count(fd) > MAX_CHAN_COUNT)
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_toomany_channels(fd, _args);
	}
	else if (!_data->channel_exists(channel)) //user creates channel
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
		_args.push_back(_data->get_user_info(fd));
		_args.push_back(channel);
		join_reply(fd, _args);
		_args[0] = _data->get_srvname();
		rpl_notopic(fd, _args);
		_args[1] = "=" + channel;
		std::string	nick = _data->get_nickname(fd);
		if (_data->check_member_status(channel, fd, OPER_MFLAG))
			nick.insert(0, "@");
		else if (_data->check_member_status(channel, fd, VOICE_MFLAG))
			nick.insert(0, "+");
		_args.push_back(nick);
		rpl_nam_reply(fd, _args);
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
		std::vector<int>			members_fd = _data->get_members_list_fd(channel);	
		std::vector<int>::iterator	it = members_fd.begin();
		_args.push_back(_data->get_user_info(fd));
		_args.push_back(channel);
		for (; it != members_fd.end(); it++)
			join_reply(*it, _args);
		_args[0] = _data->get_srvname();
		std::string	topic = _data->get_channel_topic(channel);
		if (!topic.empty())
		{
			_args.push_back(topic);
			rpl_topic(fd, _args);
			_args.pop_back();
		}
		else
			rpl_notopic(fd, _args);
		if (_data->check_channel_flags(channel, PRIVATE_CFLAG))
			_args[1] = "*" + channel;
		else if (_data->check_channel_flags(channel, SECRET_CFLAG))
			_args[1] = "@" + channel;
		else
			_args[1] = "=" + channel;
		for (it = members_fd.begin(); it != members_fd.end(); it++)
		{
			std::string	nick = _data->get_nickname(*it);
			if (_data->check_member_status(channel, *it, OPER_MFLAG))
				nick.insert(0, "@");
			else if (_data->check_member_status(channel, *it, VOICE_MFLAG))
				nick.insert(0, "+");
			_args.push_back(nick);
		}
		rpl_nam_reply(fd, _args);
	}
	// implement ban masks
}

void	Command::part(int fd, const std::string& channel, const std::string& message)
{
	if (!valid_channel(channel) || !_data->channel_exists(channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_nosuch_channel(fd, _args);
	}
	else if (!_data->is_in_channel(fd, channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_noton_channel(fd, _args);
	}
	else
	{
		std::vector<int>	members_fd = _data->get_members_list_fd(channel);
		std::vector<int>::iterator	it = members_fd.begin();
		_args.push_back(_data->get_user_info(fd));
		_args.push_back(channel);
		if (!message.empty())
			_args.push_back(message);
		else
			_args.push_back(_data->get_nickname(fd));
		for (; it != members_fd.end(); ++it)
			part_reply(*it, _args);
		_data->remove_channel_from_user(fd, channel);
		_data->remove_user_from_channel(fd, channel);
		if (_data->channel_is_empty(channel))
			_data->delete_channel(channel);
	}
}

	
// helper functions

void	Command::quit_dispatch(int fd, const std::vector<std::string>& params)
{
	std::set<int>					friends = _data->get_friends(fd);
	std::set<int>::const_iterator	it = friends.begin();

	_args.push_back(_data->get_srvname());
	_args.push_back(_data->get_hostname(fd));
	if (params.size() > 0)
		_args.push_back(params[0]);
	for (; it != friends.end(); ++it)
		error_quit(*it, _args);
	_data->delete_user(fd);
	//severe connection
}

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
		for (itc = channels.begin(), itk = keys.begin(); itc != channels.end() && itk != keys.end(); itc++, itk++)
		{
			join(fd, *itc, *itk);
		}
		for (; itc != channels.end(); itc++)
			join(fd, *itc, std::string());
	}	
}

void	Command::part_dispatch(int fd, const std::vector<std::string>& params)
{
	if (params.size() < 1)
	{
		_args.push_back(_data->get_srvname());
		_args.push_back("PART");
		err_need_moreparams(fd, _args);
	}
	else
	{
		std::vector<std::string>	channels = parse_list(params[0]);
		std::string					message;
		if (params.size() == 2)
			message = params[1];
		std::vector<std::string>::iterator	itc;
		for (itc = channels.begin(); itc != channels.end(); ++itc)
			part(fd, *itc, message);	
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
	_cmd_map.insert(std::make_pair("QUIT", &Command::quit_dispatch));
	_cmd_map.insert(std::make_pair("JOIN", &Command::join_dispatch));
	_cmd_map.insert(std::make_pair("PART", &Command::part_dispatch));
	_cmd_map.insert(std::make_pair("TOPIC", &Command::topic_dispatch));
}

void	Command::execute_cmd(int fd, const irc_cmd& cmd)
{
	std::map<std::string, FuncPtr>::iterator	it = _cmd_map.find(cmd._cmd);

	if (it != _cmd_map.end()) // found a valid command
		(this->*(it->second))(fd, cmd._params);	
	else
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(cmd._cmd);
		err_unknown_command(fd, _args);
	}
}
