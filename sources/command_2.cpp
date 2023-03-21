/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:09:41 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/21 14:55:50 by pcamaren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

//TOPIC CHECK IF CHANNEL IS SECRET

void	Command::cap(int fd, const std::vector<std::string>& params)
{
	(void)fd;
	(void)params;
}

void	Command::topic(int fd, const std::vector<std::string>& params)
{
	const bool isRegistered =  _data->is_registered(fd);
	size_t p_size = params.size();
	const bool notEnoughParams = p_size < 1;
	const bool hasTooManyParams = p_size > 2;
	const bool paramsIsCorrect = !notEnoughParams && !hasTooManyParams;
	const bool channelExist = isRegistered && paramsIsCorrect && _data->channel_exists(params[0]);
	const bool isInChannel = channelExist && _data->is_in_channel(fd, params[0]);
	const bool isGetTopic = isInChannel && p_size == 1;
	const bool hasChannelPrivilege = isInChannel && _data->check_member_status(params[0], fd, OPER_MFLAG);
	const bool needPrivilege = isInChannel && _data->check_channel_flags(params[0], TOPIC_OPER_CFLAG);
	const bool canCreateTopic = hasChannelPrivilege || !needPrivilege;

	if (!isRegistered)
	{
		_args.push_back(_data->get_srvname());
		err_not_registered(fd, _args);
		return;
	}
	if (!paramsIsCorrect) {
		if (notEnoughParams)
		{
			_args.push_back(_data->get_srvname());
			_args.push_back("TOPIC");
			err_need_moreparams(fd, _args);
		}
		return;
	}
	if (!isInChannel) 
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(params[0]);
		err_noton_channel(fd, _args);
		return;
	}
	
	const std::string channel = params[0];

	if (isGetTopic) {
		const std::string	topic = _data->get_channel_topic(channel);
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		
		if (topic.empty())
		{
			rpl_notopic(fd, _args);
		}
		else
		{
			_args.push_back(topic);
			rpl_topic(fd, _args);
		}
		return;
	}
	if (!canCreateTopic)
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_chano_privsneeded(fd, _args);
		return;
	}

	const std::string new_topic = params[1];
	_data->set_channel_topic(channel, new_topic);
	_args.push_back(_data->get_user_info(fd));
	_args.push_back(channel);
	_args.push_back(new_topic);
	std::vector<int>	members = _data->get_members_list_fd(channel);
	for (std::vector<int>::iterator it = members.begin(); it != members.end(); it++)
		topic_reply(*it, _args);

}

void Command::names(int fd, const std::vector<std::string>& params)
{
    _args.push_back(_data->get_srvname());

    if (!_data->is_registered(fd))
    {
        err_not_registered(fd, _args);
        return;
    }

    if (params.empty())
    {
        list_visible_channels(fd);
    }
    else if (params.size() == 1)
    {
        list_channel_users(fd, params[0]);
    }
    else if (params.size() == 2 && params[1] == _args[0])
    {
        list_channel_users(fd, params[0]);
    }
    else
    {
        _args.erase(_args.begin() + 1, _args.end());
        _args.push_back(params[1]);
        err_nosuch_server(fd, _args);
    }
}

void Command::list_visible_channels(int fd)
{
    std::vector<std::string> visible_channels = _data->list_visible_channels(fd);

    if (visible_channels.empty())
    {
        _args.push_back("*");
        rpl_endof_names(fd, _args);
        return;
    }

    for (std::vector<std::string>::const_iterator it = visible_channels.begin(); it != visible_channels.end(); ++it)
    {
        list_channel_users(fd, *it);
    }
}

void Command::list_channel_users(int fd, const std::string& channel)
{
    if (!_data->channel_exists(channel))
    {
        _args.erase(_args.begin() + 1, _args.end());
        _args.push_back(channel);
        rpl_endof_names(fd, _args);
        return;
    }

    _args.push_back(channel);
    std::vector<std::string> users_channel = _data->get_members_list_str(channel);
    std::vector<int> users_fd = _data->get_members_list_fd(channel);
    std::vector<int>::const_iterator iter_fd = users_fd.begin();
    std::vector<std::string>::const_iterator iter_users = users_channel.begin();

    for (; iter_users != users_channel.end(); ++iter_users)
    {
        if (!_data->check_user_flags(*iter_fd, INVISIBLE_UFLAG))
        {
            std::string user = *iter_users;
            if (_data->check_member_status(channel, *iter_fd, OPER_MFLAG))
                user.insert(0,"@");
            else if (_data->check_member_status(channel, *iter_fd, VOICE_MFLAG))
                user.insert(0, "+");
            _args.push_back(user);
        }
        ++iter_fd;
    }

    rpl_nam_reply(fd, _args);
    _args.erase(_args.begin() + 2, _args.end());
    rpl_endof_names(fd, _args);
    _args.pop_back();
}

// void	Command::names(int fd, const std::vector<std::string>& params)
// {
// 	std::cout << "nani" << std::endl;
// 	_args.push_back(_data->get_srvname());
// 	int	p_size = params.size();
// 	if (_data->is_registered(fd))
// 	{
// 		if (p_size == 0)
// 		{
// 			std::vector<std::string> visible_channels = _data->list_visible_channels(fd);
// 			if (visible_channels.size() == 0)
// 			{
// 				_args.push_back("*");
// 				rpl_endof_names(fd, _args);
// 				return;
// 			}
// 			std::vector<std::string>::iterator iter_channels = visible_channels.begin();
// 			for (iter_channels = visible_channels.begin(); iter_channels != visible_channels.end(); ++iter_channels)
// 			{
// 				_args.push_back(*iter_channels);
// 				std::vector<std::string> users_channel = _data->get_members_list_str(*iter_channels);
// 				std::vector<int> users_fd = _data->get_members_list_fd(*iter_channels);
// 				std::vector<int>::iterator iter_fd = users_fd.begin();
// 				std::vector<std::string>::iterator iter_users = users_channel.begin();
// 				for (; iter_users != users_channel.end(); ++iter_users)
// 				{
// 					if (!_data->check_user_flags(*iter_fd, INVISIBLE_UFLAG))
// 					{
// 						std::string user = *iter_users;
// 						if (_data->check_member_status(*iter_channels, *iter_fd, OPER_MFLAG))
// 							user.insert(0,"@");
// 						else if(_data->check_member_status(*iter_channels, *iter_fd, VOICE_MFLAG))
// 							user.insert(0, "+");
// 						_args.push_back(user);
// 					}
// 					++iter_fd;
// 				}
// 				rpl_nam_reply(fd, _args);
// 				_args.erase(_args.begin() + 2, _args.end());
// 				rpl_endof_names(fd, _args);
// 				_args.pop_back();
// 			}
// 		}
// 		else
// 		{
// 			if (p_size >= 2)
// 			{
// 				if (p_size > 2)
// 				{
// 					_args.push_back(params[1]);
// 					err_nosuch_server(fd, _args);
// 					return;
// 				}
// 				else
// 				{
// 					if (params[1] != _args[0])
// 					{
// 						_args.push_back(params[1]);
// 						err_nosuch_server(fd, _args);
// 						return;
// 					}
// 				}
// 			}
// 			std::vector<std::string> channels = parse_list(params[0]);
// 			std::vector<std::string>::iterator chan_iter = channels.begin();
// 			for (chan_iter = channels.begin(); chan_iter != channels.end(); ++chan_iter)
// 			{
// 				if (_data->channel_exists(*chan_iter))
// 				{
// 					_args.push_back(*chan_iter);
// 					std::vector<std::string> users_channel = _data->get_members_list_str(*chan_iter);
// 					std::vector<int> users_fd = _data->get_members_list_fd(*chan_iter);
// 					std::vector<int>::iterator iter_fd = users_fd.begin();
// 					std::vector<std::string>::iterator iter_users = users_channel.begin();
// 					for (; iter_users != users_channel.end(); ++iter_users)
// 					{
// 						if (!_data->check_user_flags(*iter_fd, INVISIBLE_UFLAG))
// 						{
// 							std::string user = *iter_users;
// 							if (_data->check_member_status(*chan_iter, *iter_fd, OPER_MFLAG))
// 								user.insert(0,"@");
// 							else if(_data->check_member_status(*chan_iter, *iter_fd, VOICE_MFLAG))
// 								user.insert(0, "+");
// 							_args.push_back(user);
// 							++iter_fd;
// 						}
// 					}
// 					rpl_nam_reply(fd, _args);
// 					_args.erase(_args.begin() + 2, _args.end());
// 					rpl_endof_names(fd, _args);
// 					_args.pop_back();
// 				}
// 				else
// 				{
// 					_args.erase(_args.begin() + 1, _args.end());
// 					_args.push_back(*chan_iter);
// 					rpl_endof_names(fd, _args);
// 				}
// 			}
// 		}	
// 	}
// 	else
// 		err_not_registered(fd, _args);
// }

void	Command::pong(int fd, const std::vector<std::string>& params)
{
	int	size_p = params.size();
	if (size_p > 1)
		return;
	std::string server_name = _data->get_srvname();
	_data->set_user_last_pong(fd);
	_data->set_user_was_ping(fd, false);
	_args.push_back(server_name);
	if (size_p == 0)
	{
		err_noorigin(fd, _args);
		return;
	}
	if (size_p == 1)
	{
		if(params[0] != server_name)
		{
			_args.push_back(server_name);
			err_nosuch_server(fd, _args);
		}		
	}
}

void	Command::privmsg(int fd, const std::vector<std::string>& params)
{
	int p_size = params.size();
	if (p_size > 2)
		return;
	if (!_data->is_registered(fd))
	{
		_args.push_back(_data->get_srvname());
		err_not_registered(fd, _args);
		return;
	}
	if (p_size == 0)
	{
		_args.push_back(_data->get_srvname());
		_args.push_back("PRIVMSG");
		err_notext_tosend(fd, _args);
		err_norecipient(fd, _args);
		return ;
	}
	if (p_size == 1)
	{
		_args.push_back(_data->get_srvname());
		err_notext_tosend(fd, _args);
		return;
	}
	std::vector<std::string> target = parse_list(params[0]);
	std::string message = params[1];
	std::vector<std::string>::iterator target_iter = target.begin();
	int ret_fd;
	while (target_iter != target.end())
	{
		ret_fd = parse_target(fd, *target_iter);
		if (ret_fd != -1)
		{
			if (ret_fd == -5)
				send_to_chan(fd, *target_iter, message);
			else if (ret_fd == -6)
				send_to_nick(fd, *target_iter, message);
			else
			{
				_args.push_back(_data->get_user_info(fd));
				_args.push_back(_data->get_nickname(ret_fd));
				_args.push_back(message);
				privmsg_reply(ret_fd, _args);
			}
		}
		++target_iter;
		_args.clear();
	}
}

void	Command::notice(int fd, const std::vector<std::string>& params)
{
	int p_size = params.size();
	if (p_size > 2)
		return;

	if (!_data->is_registered(fd))
	{
		_args.push_back(_data->get_srvname());
		err_not_registered(fd, _args);
		return;
	}
	if (p_size == 0)
		return;
	if (p_size == 1)
		return;
	std::vector<std::string> target = parse_list(params[0]);
	std::string message = params[1];
	std::vector<std::string>::iterator target_iter = target.begin();
	int ret_fd;
	while (target_iter != target.end())
	{
		ret_fd = parse_target_notice(*target_iter);
		if (ret_fd != -1)
		{
			if (ret_fd == -5)
				send_to_chan_notice(fd, *target_iter, message);
			else if (ret_fd == -6)
				send_to_nick_notice(fd, *target_iter, message);
			else
			{
				_args.push_back(_data->get_user_info(fd));
				_args.push_back(params[0]);
				_args.push_back(message);
				notice_reply(ret_fd, _args);
			}
			++target_iter;
		}
		else
			++target_iter;
		_args.clear();
	}
}

void	Command::invite(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (_data->is_registered(fd))
	{
		int size_p = params.size();
		if (size_p < 2)
		{
			_args.push_back("INVITE");
			err_need_moreparams(fd, _args);
			return;
		}
		if (size_p == 2)
		{
			std::string nickname = params[0];
			std::string channel = params[1];
			if (_data->nickname_exists(nickname))
			{
				if (_data->channel_exists(channel))
				{
					if (_data->is_in_channel(_data->get_nickname(fd), channel))
					{
						if (_data->check_channel_flags(channel, INVITE_ONLY_CFLAG))
						{
							if (_data->check_member_status(channel, fd, OPER_MFLAG))
							{
								if (_data->is_in_channel(nickname, channel))
								{
									_args.push_back(nickname);
									_args.push_back(channel);
									err_user_on_channel(fd, _args);
									return;
								}
								else
								{
									int	dest_fd = _data->get_user_fd(nickname);
									if (_data->check_user_flags(dest_fd, AWAY_UFLAG))
									{
										_args.push_back(nickname);
										_args.push_back("user is away");
										rpl_away(fd, _args);
										return;
									}
									else
									{
										_args.push_back(channel);
										_args.push_back(nickname);
										rpl_inviting(fd, _args);
										std::string message = _data->get_user_info(fd) + " INVITE " + nickname + " " + channel + "\r\n";
										send(dest_fd, message.c_str(), message.size(), 0);
										_data->add_invit_mask(channel, nickname);
									}
								}
							}
							else
							{
								_args.push_back(channel);
								err_chano_privsneeded(fd, _args);
							}
						}
						else
						{
							if (_data->is_in_channel(nickname, channel))
							{
								_args.push_back(nickname);
								_args.push_back(channel);
								err_user_on_channel(fd, _args);
								return;
							}
							else
							{
								int	dest_fd = _data->get_user_fd(nickname);
								if (_data->check_user_flags(dest_fd, AWAY_UFLAG))
								{
									_args.push_back(nickname);
									_args.push_back("user is away");
									rpl_away(fd, _args);
									return;
								}
								else
								{
									_args.push_back(channel);
									_args.push_back(nickname);
									rpl_inviting(fd, _args);
									std::string message = ":" + _data->get_user_info(fd) + " INVITE " + nickname + " #" + channel + "\r\n";
									send(dest_fd, message.c_str(), message.size(), 0);
								}
							}
						}
					}
					else
					{
						_args.push_back(channel);
						err_noton_channel(fd, _args);
						return ;
					}
				}
				else
				{
					_args.push_back(channel);
					std::cout << "INVITE-errnosuchchannel" << std::endl;
					err_nosuch_channel(fd, _args);
					return;
				}
			}
			else
			{
				_args.push_back(nickname);
				err_nosuch_nick(fd, _args);
				return;
			}	
		}
	}
	else
		err_not_registered(fd, _args);
}

void	Command::whois(int fd, const std::vector<std::string>& params)
{
	const bool isRegistered =  _data->is_registered(fd);
	int p_size = params.size();
	const bool enoughParams = p_size > 0;
	_args.push_back(_data->get_srvname());
	if (!isRegistered)
	{
		_args.push_back("WHOIS");
		err_not_registered(fd, _args);
		return;
	}
	if (!enoughParams)
	{
		_args.push_back("WHOIS");
		err_need_moreparams(fd, _args);
		return;
	}
	const bool onlyNickname = p_size == 1;
	const bool hasTarget = p_size >= 2;
	if (onlyNickname)
	{
		if (!_data->nickname_exists(params[0]))
		{
			_args.push_back(params[0]);
			err_nosuch_nick(fd, _args);
			rpl_endof_whois(fd, _args);
			return ;
		}
		valid_whois(fd, params[0]);
		return;
	}
	if (hasTarget)
	{
		if (!(_data->get_srvname() == params[0]))
		{
			_args.push_back(params[0]);
			err_nosuch_server(fd, _args);
			return ;
		}
		if (!_data->nickname_exists(params[1]))
		{
			_args.push_back(params[1]);
			err_nosuch_nick(fd, _args);
			rpl_endof_whois(fd, _args);
			return ;
		}
		valid_whois(fd, params[1]);
	}
}

void	Command::ping(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	if (params.size() < 1)
	{
		_args.push_back("PING");
		err_need_moreparams(fd, _args);
		return;
	}
	_args.push_back(params[0]);
	pong_reply(fd, _args);
}

void	Command::die(int fd, const std::vector<std::string>& params)
{
	if (params.size() != 0)
		return;
	if (_data->check_user_flags(fd, OPER_UFLAG))
	{
		g_die = 1;
		return;
	}
	_args.push_back(_data->get_srvname());
	err_noprivileges(fd, _args);	
}

/*---------------------------HELPER FUNCTIONS---------------------------------*/

void	Command::valid_whois(int fd, const std::string& nickname)
{
	_args.push_back(_data->get_username(fd));
	_args.push_back(_data->get_hostname(fd));
	_args.push_back(_data->get_realname(fd));
	rpl_whois_user(fd, _args);
	_args.erase(_args.begin() + 1, _args.end());
	bool in_chans = _data->user_channel_count(fd);
	if (in_chans)
	{
		std::set<std::string>	channels = _data->get_channels_of_user(fd);
		std::set<std::string>::const_iterator it = channels.begin();
		for (; it != channels.end() ; ++it)
		{
			std::string chan_name = *it;
			if (_data->check_member_status(*it, fd, OPER_MFLAG))
				_args.push_back("@" +*it);
			else if (_data->check_member_status(*it, fd, VOICE_MFLAG))
				_args.push_back("+" +*it);
			else
				_args.push_back(*it);
		}
		rpl_whois_channel(fd, _args);
	}
	_args.erase(_args.begin() + 1, _args.end());
	_args.push_back("I am an IRC server made by rbourdil and pcamaren");
	rpl_whois_server(fd, _args);
	_args.pop_back();
	std::stringstream s;
	std::string _time;
	double time_diff = difftime(std::time(NULL), _data->get_user_last_move(fd));
	s << time_diff;
	_time = s.str();
	_args.push_back(_time);
	long int creation_time = static_cast<long int> (_data->get_creation_time(fd));
	s << creation_time;
	_time = s.str();
	_args.push_back(_time);
	rpl_whois_idle(fd, _args);
	_args.erase(_args.begin() + 1, _args.end());
	if (_data->check_user_flags(fd, OPER_UFLAG))
		rpl_whois_operator(fd, _args);
	_args.push_back(nickname);
	rpl_endof_whois(fd, _args);
}

int		Command::parse_target(int fd, std::string &target)
{
	_args.push_back(_data->get_srvname());
	if (_data->channel_exists(target))
		return -5;
	if (_data->nickname_exists(target))
		return -6;
	Scanner scanner;
	scanner._current = target.begin();
	scanner._start = scanner._current;
	size_t pos = target.find_first_of("!@%", scanner._current - target.begin());
	if (pos == std::string::npos)
	{
		_args.push_back(target);
		if (target.at(0) == '#' || target.at(0) == '&' || target.at(0) == '+' || target.at(0) == '!')
		{
			err_nosuch_channel(fd, _args);
			return -1;
		}
		else
		{
			err_nosuch_nick(fd, _args);
			return -1;
		}
	}
	else
	{
		scanner._current = scanner._start + pos;
		if (*scanner._current == '!')
		{
			std::string nickname(scanner._start, scanner._current);
			if (!_data->nickname_exists(nickname))
			{
				_args.push_back(nickname);
				err_nosuch_nick(fd, _args);
				// _args.pop_back();
				return -1;
			}
			else
			{
				++scanner._current;
				scanner._start = scanner._current;
				pos = target.find_first_of("@", scanner._current - target.begin());
				scanner._current = target.begin() + pos;
				std::string user(scanner._start, scanner._current);				
				int dest_fd = _data->get_user_fd(nickname);
				if (_data->user_nick_match(nickname, user))
				{
					++scanner._current;
					std::string host(scanner._current, target.end());
					if (is_valid_host(host))
					{
						if (_data->host_fd_match(dest_fd, host))
							return (dest_fd);
						else
						{
							_args.push_back(nickname);
							err_nosuch_nick(fd, _args);
							return -1;
						}
					}
				}
				else
				{
					_args.push_back(nickname);
					err_nosuch_nick(fd, _args);
					return -1;
				}
			}

		}
		else if (*scanner._current == '%')
		{
			std::string user(scanner._start, scanner._current);
			int valid_user = _data->user_is_unique(user);
			if (valid_user == -1)
			{
				_args.push_back(user);
				_args.push_back("user is not unique");
				err_toomany_targets(fd, _args);
				return -1;
			}
			else if (valid_user == 0)
			{
				_args.push_back(user);
				error_user(fd, _args);
				return -1;
			}
			++scanner._current;
			scanner._start = scanner._current;
			pos = target.find_first_of("@", scanner._current - target.begin());
			scanner._current = target.begin() + pos;
			std::string host(scanner._start, scanner._current);
			if (is_valid_host(host))
			{
				if (pos == std::string::npos)
				{
					if (_data->host_fd_match(valid_user, host))
						return (valid_user);
					else
					{
						_args.push_back(host);
						error_host_notmatch(fd, _args);
						return -1;
					}
				}
				else
				{
					++scanner._current;
					std::string servername(scanner._current, target.end());
					if (servername == _data->get_srvname())
					{
						return (valid_user);
					}
					else
					{
						err_nosuch_server(fd, _args);
						return -1;
					}
				}
			}
			else
			{
				_args.push_back(host);
				error_host(fd, _args);
				return -1;
			}
		}
		else if (*scanner._current == '@')
		{
			std::string user(scanner._start, scanner._current);
			int valid_user = _data->user_is_unique(user);
			if (valid_user == -1)
			{
				_args.push_back(user);
				_args.push_back("user is not unique");
				err_toomany_targets(fd, _args);
				return -1;
			}
			else if (valid_user == 0)
			{
				_args.push_back(user);
				error_user(fd, _args);
				return -1;
			}
			++scanner._current;
			std::string servername(scanner._current, target.end());
			if (servername == _data->get_srvname())
				return (valid_user);
			else
			{
				err_nosuch_server(fd, _args);
				return -1;
			}
		}
		else
		{
			_args.push_back(target);
			err_nosuch_nick(fd, _args);
			return -1;
		}
	}
	return -1;
}

void	Command::send_to_chan(int fd, std::string &channel, std::string &message)
{
	if (_data->is_banned(fd, channel))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(channel);
		err_cannotsend_tochan(fd, _args);
	}
	else if (_data->check_channel_flags(channel, NO_MESSAGES_CFLAG))
	{
		if (_data->is_in_channel(fd, channel))
		{
			if (_data->check_channel_flags(channel, MODERATED_CFLAG))
			{
				if (_data->check_user_flags(fd, OPER_UFLAG) || _data->check_member_status(channel, fd, VOICE_MFLAG))
				{
					std::vector<int> users_chan = _data->get_members_list_fd(channel);
					std::vector<int>::iterator it = users_chan.begin();
					std::string original_message = message;
					for (; it != users_chan.end(); ++it)
					{
						if (*it != fd)
						{
							if (_data->check_user_flags(*it, AWAY_UFLAG))
							{
								_args.push_back(_data->get_srvname());
								_args.push_back(_data->get_nickname(fd));
								rpl_away(fd, _args);
							}
							else
							{
								_args.push_back(_data->get_user_info(fd));
								_args.push_back(channel);
								_args.push_back(message);
								privmsg_reply(*it, _args);
							}
							_args.clear();
						}
					}
				}
				else
				{
					_args.push_back(_data->get_srvname());
					_args.push_back(channel);
					err_cannotsend_tochan(fd, _args);
				}
			}
		}
		else
		{
			_args.push_back(_data->get_srvname());
			_args.push_back(channel);
			err_cannotsend_tochan(fd, _args);
		}
	}
	else if (_data->check_channel_flags(channel, MODERATED_CFLAG))
	{
		if (_data->check_user_flags(fd, OPER_UFLAG) || _data->check_member_status(channel, fd, VOICE_MFLAG))
		{
			std::vector<int> users_chan = _data->get_members_list_fd(channel);
			std::vector<int>::iterator it = users_chan.begin();
			std::string original_message = message;
			for (; it != users_chan.end(); ++it)
			{
				if (_data->check_user_flags(*it, AWAY_UFLAG))
				{
					_args.push_back(_data->get_srvname());
					_args.push_back(_data->get_nickname(*it));
					rpl_away(fd, _args);
				}
				else
				{
					_args.push_back(_data->get_user_info(fd));
					_args.push_back(channel);
					_args.push_back(message);
					privmsg_reply(*it, _args);
				}
				_args.clear();
			}
		}
		else
		{
			_args.push_back(_data->get_srvname());
			_args.push_back(channel);
			err_cannotsend_tochan(fd, _args);
		}
	}
	else
	{
		std::vector<int> users_chan = _data->get_members_list_fd(channel);
		std::vector<int>::iterator it = users_chan.begin();
		std::string original_message = message;
		for (; it != users_chan.end(); ++it)
		{
			if (*it != fd)
			{
				if (_data->check_user_flags(*it, AWAY_UFLAG))
				{
					_args.push_back(_data->get_srvname());
					_args.push_back(_data->get_nickname(*it));
					rpl_away(fd, _args);
				}
				else
				{
					_args.push_back(_data->get_user_info(fd));
					_args.push_back(channel);
					_args.push_back(message);
					privmsg_reply(*it, _args);
				}	
				_args.clear();
			}
		}
	}
}	

void	Command::send_to_nick(int fd, std::string &nick, std::string &message)
{
	int dest_fd = _data->get_user_fd(nick);
	if (_data->check_user_flags(dest_fd, AWAY_UFLAG))
	{
		_args.push_back(_data->get_srvname());
		_args.push_back(nick);
		rpl_away(fd, _args);
	}
	else
	{
		_args.push_back(_data->get_user_info(fd));
		_args.push_back(nick);
		_args.push_back(message);
		privmsg_reply(dest_fd, _args);
	}
}

bool	Command::is_valid_host(std::string &host)
{
		struct addrinfo hints;
		struct addrinfo* result;
		
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_CANONNAME;
		
		int status = getaddrinfo(host.c_str(), NULL, &hints, &result);
		if (status != 0) {
			return false;
		}
		
		freeaddrinfo(result);
		
		return true;
}

int		Command::parse_target_notice(std::string &target)
{
	if (_data->channel_exists(target))
		return -5;
	if (_data->nickname_exists(target))
		return -6;
	Scanner scanner;
	scanner._current = target.begin();
	scanner._start = scanner._current;
	size_t pos = target.find_first_of("!@%", scanner._current - target.begin());
	if (pos == std::string::npos)
	{
		if (target.at(0) == '#' || target.at(0) == '&' || target.at(0) == '+' || target.at(0) == '!')
		{
			return -1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		scanner._current = scanner._start + pos;
		if (*scanner._current == '!')
		{
			std::string nickname(scanner._start, scanner._current);
			if (!_data->nickname_exists(nickname))
			{
				return -1;
			}
			else
			{
				++scanner._current;
				scanner._start = scanner._current;
				pos = target.find_first_of("@", scanner._current - target.begin());
				scanner._current = target.begin() + pos;
				std::string user(scanner._start, scanner._current);				
				int dest_fd = _data->get_user_fd(nickname);
				if (_data->user_nick_match(nickname, user))
				{
					++scanner._current;
					std::string host(scanner._current, target.end());
					if (is_valid_host(host))
					{
						if (_data->host_fd_match(dest_fd, host))
							return (dest_fd);
						else
							return -1;
					}
				}
				else
					return -1;
			}

		}
		else if (*scanner._current == '%')
		{
			std::string user(scanner._start, scanner._current);
			int valid_user = _data->user_is_unique(user);
			if (valid_user == -1)
				return -1;
			else if (valid_user == 0)
				return -1;
			++scanner._current;
			scanner._start = scanner._current;
			pos = target.find_first_of("@", scanner._current - target.begin());
			scanner._current = target.begin() + pos;
			std::string host(scanner._start, scanner._current);
			if (is_valid_host(host))
			{
				if (pos == std::string::npos)
				{
					
					if (_data->host_fd_match(valid_user, host))
						return (valid_user);
					else
						return -1;
				}
				else
				{
					++scanner._current;
					std::string servername(scanner._current, target.end());
					if (servername == _data->get_srvname())
						return (valid_user);
					else
						return -1;
				}
			}
			else
				return -1;
		}
		else if (*scanner._current == '@')
		{
			std::string user(scanner._start, scanner._current);
			int valid_user = _data->user_is_unique(user);
			if (valid_user == -1)
				return -1;
			else if (valid_user == 0)
				return -1;
			++scanner._current;
			std::string servername(scanner._current, target.end());
			if (servername == _data->get_srvname())
				return (valid_user);
			else
				return -1;
		}
		else
			return -1;
	}
	return -1;
}

void	Command::send_to_chan_notice(int fd, std::string &channel, std::string &message)
{
	if (_data->is_banned(fd, channel))
	{
		return;
	}
	else if (_data->check_channel_flags(channel, NO_MESSAGES_CFLAG))
	{
		if (_data->is_in_channel(fd, channel))
		{
			if (_data->check_channel_flags(channel, MODERATED_CFLAG))
			{
				if (_data->check_user_flags(fd, OPER_UFLAG) || _data->check_member_status(channel, fd, VOICE_MFLAG))
				{
					std::vector<int> users_chan = _data->get_members_list_fd(channel);
					std::vector<int>::iterator it = users_chan.begin();
					std::string original_message = message;
					for (; it != users_chan.end(); ++it)
					{
						if (*it != fd)
						{
							if (_data->check_user_flags(*it, AWAY_UFLAG))
							{
								continue;
							}
							else
							{
								_args.push_back(_data->get_user_info(fd));
								_args.push_back(channel);
								_args.push_back(message);
								notice_reply(*it, _args);
								_args.clear();
							}
						}
					}
				}
				else
					return;
			}
		}
		else
			return;
	}
	else if (_data->check_channel_flags(channel, MODERATED_CFLAG))
	{
		if (_data->check_user_flags(fd, OPER_UFLAG) || _data->check_member_status(channel, fd, VOICE_MFLAG))
		{
			std::vector<int> users_chan = _data->get_members_list_fd(channel);
			std::vector<int>::iterator it = users_chan.begin();
			std::string original_message = message;
			for (; it != users_chan.end(); ++it)
			{
				if (_data->check_user_flags(*it, AWAY_UFLAG))
					continue;
				else
				{
					_args.push_back(_data->get_user_info(fd));
					_args.push_back(channel);
					_args.push_back(message);
					notice_reply(*it, _args);
					_args.clear();
				}
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		std::vector<int> users_chan = _data->get_members_list_fd(channel);
		std::vector<int>::iterator it = users_chan.begin();
		std::string original_message = message;
		for (; it != users_chan.end(); ++it)
		{
			if (*it != fd)
			{
				if (_data->check_user_flags(*it, AWAY_UFLAG))
				{
					continue;
				}
				else
				{
					_args.push_back(_data->get_user_info(fd));
					_args.push_back(channel);
					_args.push_back(message);
					notice_reply(*it, _args);
				}	
				_args.clear();
			}
		}
	}
}	

void	Command::send_to_nick_notice(int fd, std::string &nick, std::string &message)
{
	int dest_fd = _data->get_user_fd(nick);
	if (_data->check_user_flags(dest_fd, AWAY_UFLAG))
		return;
	else
	{
		_args.push_back(_data->get_user_info(fd));
		_args.push_back(nick);
		_args.push_back(message);
		notice_reply(fd, _args);
	}
}
