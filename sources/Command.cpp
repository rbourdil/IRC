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
	} }

void	Command::nick(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (params.size() == 0)
		err_no_nicknamegive(fd, _args);
	else if (!valid_nickname(params[0]) || params[0] == "anonymous")
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

void	Command::user_mode(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (params[0] != _data->get_nickname(fd))
		err_users_dontmatch(fd, _args);
	else if (params.size() == 1)
	{
		_args.push_back(user_mode_str(fd));
		rpl_umodeis(fd, _args);
	}
	else if (!valid_user_mode(params[1]))
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
		_args.push_back(user_mode_str(fd));
		std::cerr << _args.size() << std::endl;
		rpl_umodeis(fd, _args);
	}
}

void	Command::join(int fd, std::string channel, std::string key)
{
	if (channel == "0")
	{
		std::set<std::string>					channel_list = _data->get_user_channel_list(fd);
		std::set<std::string>::const_iterator	it = channel_list.begin();
		while (it != channel_list.end())
		{
			part(fd, *it, std::string());
			_args.clear();
			it++;
		}
	}
	else if (!valid_channel(channel))
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
		_data->add_channel_to_user(fd, channel);
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
	else if (_data->check_channel_flags(channel, INVITE_ONLY_CFLAG) && !_data->match_invit_masks(fd, channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_invite_onlychan(fd, _args);
	}
	else if (_data->is_banned(fd, channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_banned_fromchan(fd, _args);
	}
	else if (!_data->channel_authenticate(channel, key))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_badchannel_key(fd, _args);
	}
	else if (_data->channel_is_full(channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_channel_isfull(fd, _args);
	}
		else // user joins an existing channel
	{
		_data->add_user_to_channel(fd, channel);
		_data->add_channel_to_user(fd, channel);
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
	}
}

void	Command::channel_mode(int fd, const std::vector<std::string>& params)
{
	const std::string	channel = params[0];
	int					char_mode;

	_args.push_back(_data->get_srvname());
	if (!_data->channel_exists(channel))
	{
		_args.push_back(channel);
		err_nosuch_channel(fd, _args);
	}
	else if (channel[0] == '+')
	{
		_args.push_back(channel);
		err_nochan_modes(fd, _args);
	}
	else if (!_data->check_member_status(channel, fd, OPER_MFLAG))
	{
		_args.push_back(channel);
		err_chano_privsneeded(fd, _args);
	}
	else if (params.size() == 1) // if MODE <channel> alone, list modes
	{
		_args.push_back(channel);
		_args.push_back(_data->get_channel_flags_str(channel));
		rpl_channel_modeis(fd, _args);	
	}
	else if ((char_mode = valid_channel_mode(params[1])) != 0)
	{
		std::string	str_mode;
		str_mode.push_back(char_mode);
		_args.push_back(str_mode);
		_args.push_back(channel);
		err_unknown_mode(fd, _args);
	}
	else if (!match_mode_params(params)) // check if there are enough params supplied to the flags
	{
		_args.push_back("MODE");
		err_need_moreparams(fd, _args);
	}
	else // channel is regonized and there are flags
	{
		std::vector<std::string>::const_iterator	itv = params.begin() + 2;
		std::string					flags = params[1];
		std::string::const_iterator	itf = flags.begin();
		bool						add = true;

		if (*itf == '+' || *itf == '-')
		{
			if (*itf == '-')
				add = false;
			itf++;
		}
		for (; itf != flags.end(); itf++)
		{
			std::cerr << "DEBUG " << *itv << std::endl;
			switch (*itf) 
			{
				case 'o':
					if (_data->is_in_channel(*itv, channel))
					{
						if (add)
							_data->set_member_status(channel, _data->get_user_fd(*itv), OPER_MFLAG);
						else
							_data->unset_member_status(channel, _data->get_user_fd(*itv), OPER_MFLAG);
						++itv;
					}
					else
					{
						_args.push_back(*itv++);
						_args.push_back(channel);
						err_usernot_inchannel(fd, _args);
						_args.pop_back();
						_args.pop_back();
					}
					break;
				case 'v':
					if (_data->is_in_channel(*itv, channel))
					{
						if (add)
							_data->set_member_status(channel, _data->get_user_fd(*itv), VOICE_MFLAG);
						else
							_data->unset_member_status(channel, _data->get_user_fd(*itv), VOICE_MFLAG);
						++itv;
					}
					else
					{
						_args.push_back(*itv++);
						_args.push_back(channel);
						err_usernot_inchannel(fd, _args);
						_args.pop_back();
						_args.pop_back();
					}
					break;
				case 'a':
					if (add)
						_data->set_channel_flags(channel, ANON_CFLAG);
					else
						_data->unset_channel_flags(channel, ANON_CFLAG);
					break;
				case 'i':
					if (add)
						_data->set_channel_flags(channel, INVITE_ONLY_CFLAG);
					else
						_data->unset_channel_flags(channel, INVITE_ONLY_CFLAG);
					break;
				case 'm':
					if (add)
						_data->set_channel_flags(channel, MODERATED_CFLAG);
					else
						_data->unset_channel_flags(channel, MODERATED_CFLAG);
					break;
				case 'n':
					if (add)
						_data->set_channel_flags(channel, NO_MESSAGES_CFLAG);
					else
						_data->unset_channel_flags(channel, NO_MESSAGES_CFLAG);
					break;
				case 'q':
					if (add)
						_data->set_channel_flags(channel, QUIET_CFLAG);
					else
						_data->unset_channel_flags(channel, QUIET_CFLAG);
					break;
				case 'p':
					if (add)
						_data->set_channel_flags(channel, PRIVATE_CFLAG);
					else
						_data->unset_channel_flags(channel, PRIVATE_CFLAG);
					break;
				case 's':
					if (add)
						_data->set_channel_flags(channel, SECRET_CFLAG);
					else
						_data->unset_channel_flags(channel, SECRET_CFLAG);
					break;
				case 'r':
					if (add)
						_data->set_channel_flags(channel, SRV_REOP_CFLAG);
					else
						_data->unset_channel_flags(channel, SRV_REOP_CFLAG);
					break;
				case 't':
					if (add)
						_data->set_channel_flags(channel, TOPIC_OPER_CFLAG);
					else
						_data->unset_channel_flags(channel, TOPIC_OPER_CFLAG);
					break;
				case 'k':
				{
					std::string	key = *itv++;
					if (_data->check_channel_flags(channel, KEY_CFLAG))
					{
						_args.push_back(channel);
						err_keyset(fd, _args);
					}
					else if (!valid_key(key))
					{
						_args.push_back(key);
						err_badchannel_key(fd, _args);
					}
					else if (add)
					{
						_data->set_channel_flags(channel, KEY_CFLAG);
						_data->set_channel_key(channel, key);
					}
					else
						_data->unset_channel_flags(channel, KEY_CFLAG);
					break;
				}
				case 'l':
					if (add)
					{
						std::stringstream	ss(*itv++);
						unsigned int		limit = 0;
						ss >> limit;

						if (ss)
						{
							_data->set_channel_flags(channel, USER_LIMIT_CFLAG);
							_data->set_members_limit(channel, limit);
						}
					}
					else
						_data->unset_channel_flags(channel, USER_LIMIT_CFLAG);
					break;
				case 'b':
					if (add)
					{
						if (itv != params.end())
						{
							_data->set_channel_flags(channel, BAN_MASK_CFLAG);
							_data->add_ban_mask(channel, *itv++);
						}
						else
						{
							const std::set<std::string>&	ban_masks = _data->get_ban_masks(channel);
							std::set<std::string>::const_iterator	itm = ban_masks.begin();
							_args.push_back(channel);
							for (; itm != ban_masks.end(); ++itm)
							{
								_args.push_back(*itm);
								rpl_ban_list(fd, _args);
								_args.pop_back();
							}
							rpl_endof_banlist(fd, _args);
						}
					}
					else
					{
						_data->unset_channel_flags(channel, BAN_MASK_CFLAG);
						_data->remove_ban_mask(channel, *itv++);
					}
					break;
				case 'e':
					if (add)
					{
						if (itv != params.end())
						{
							_data->set_channel_flags(channel, EXCEPT_MASK_CFLAG);
							_data->add_except_mask(channel, *itv++);
						}
						else
						{
							const std::set<std::string>&	except_masks = _data->get_except_masks(channel);
							std::set<std::string>::const_iterator	itm = except_masks.begin();
							_args.push_back(channel);
							for (; itm != except_masks.end(); ++itm)
							{
								_args.push_back(*itm);
								rpl_except_list(fd, _args);
								_args.pop_back();
							}
							rpl_endof_exceptlist(fd, _args);
						}
					}
					else
					{
						_data->unset_channel_flags(channel, EXCEPT_MASK_CFLAG);
						_data->remove_except_mask(channel, *itv++);
					}
					break;
				case 'I':
					if (add)
					{
						if (itv != params.end())
						{
							_data->set_channel_flags(channel, INVIT_MASK_CFLAG);
							_data->add_invit_mask(channel, *itv++);
						}
						else
						{
							const std::set<std::string>&	invit_masks = _data->get_invit_masks(channel);
							std::set<std::string>::const_iterator	itm = invit_masks.begin();
							_args.push_back(channel);
							for (; itm != invit_masks.end(); ++itm)
							{
								_args.push_back(*itm);
								rpl_invite_list(fd, _args);
								_args.pop_back();
							}
							rpl_endof_invitelist(fd, _args);
						}

					}
					else
					{
						_data->unset_channel_flags(channel, INVIT_MASK_CFLAG);
						_data->remove_invit_mask(channel, *itv++);
					}
					break;
			}
			_args.erase(_args.begin() + 1, _args.end());
		}
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
	// need to modify to send a part message instead for anon channels
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
		_args.push_back(_data->get_srvname());
		_args.push_back("JOIN");
		err_need_moreparams(fd, _args);
	}
	else
	{
		std::vector<std::string>	channels = parse_list(params[0]);
		std::vector<std::string>	keys;
		if (params.size() == 2)
			keys = parse_list(params[1]);
		std::vector<std::string>::iterator	itc = channels.begin();
		std::vector<std::string>::iterator	itk = keys.begin();
		for (; itc != channels.end() && itk != keys.end(); itc++, itk++)
		{
			join(fd, *itc, *itk);
			_args.clear();
		}
		for (; itc != channels.end(); itc++)
		{
			join(fd, *itc, std::string());
			_args.clear();
		}
	}	
}

void	Command::part_dispatch(int fd, const std::vector<std::string>& params)
{
	if (!_data->is_registered(fd))
	{
		_args.push_back(_data->get_srvname());
		err_not_registered(fd, _args);
	}
	else if (params.size() < 1)
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

void	Command::mode_dispatch(int fd, const std::vector<std::string>& params)
{
	if (!_data->is_registered(fd))
	{
		_args.push_back(_data->get_srvname());
		err_not_registered(fd, _args);
	}
	else if (params.size() < 1)
	{
		_args.push_back(_data->get_srvname());
		_args.push_back("MODE");
		err_need_moreparams(fd, _args);
	}
	else if (_data->nickname_exists(params[0]))
		user_mode(fd, params);
	else
		channel_mode(fd, params);
}

void	Command::list(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	std::vector<std::string>	channels;
	if (!_data->is_registered(fd))
	{
		err_not_registered(fd, _args);
	}
	else if (params.size() < 1)
		channels = _data->list_visible_channels(fd);
	else
		channels = parse_list(params[1]);
	std::vector<std::string>::const_iterator	it = channels.begin();
	for (; it != channels.end(); it++)
	{
		if (_data->channel_is_visible(*it))
		{
			_args.push_back(*it);
			int	count = _data->channel_visible_members_count(*it);	
			std::stringstream	ss;
			ss << count;
			_args.push_back(ss.str());
			_args.push_back(_data->get_channel_topic(*it));
			rpl_list(fd, _args);
			_args.erase(_args.begin() + 1, _args.end());
		}
	}
	rpl_listend(fd, _args);
}


std::string	Command::user_mode_str(int fd)
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
	_cmd_map.insert(std::make_pair("TOPIC", &Command::topic));
	_cmd_map.insert(std::make_pair("NAMES", &Command::names));
	_cmd_map.insert(std::make_pair("PONG", &Command::pong));
	_cmd_map.insert(std::make_pair("PRIVMSG", &Command::privmsg));
	_cmd_map.insert(std::make_pair("MODE", &Command::mode_dispatch));
	_cmd_map.insert(std::make_pair("QUIT", &Command::quit_dispatch));
	_cmd_map.insert(std::make_pair("JOIN", &Command::join_dispatch));
	_cmd_map.insert(std::make_pair("PART", &Command::part_dispatch));
	_cmd_map.insert(std::make_pair("LIST", &Command::list));
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
