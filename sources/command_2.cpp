/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:09:41 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/14 21:33:11 by pcamaren         ###   ########.fr       */
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
	_args.push_back(_data->get_srvname());
	if (_data->is_registered(fd))
	{
		size_t p_size = params.size();
		if (p_size < 1)
		{
			_args.push_back("TOPIC");
			err_need_moreparams(fd, _args);
		}
		else if (p_size > 2)
			return;
		else
		{
			const std::string channel = params[0];
			_args.push_back(channel);
			if (_data->channel_exists(channel) && _data->is_in_channel(fd, channel))
			{
				const std::string	topic = _data->get_channel_topic(channel);
				if (p_size == 1)
				{
					if (topic.empty())
						rpl_notopic(fd, _args);
					else
					{
						_args.push_back(topic);
						rpl_topic(fd, _args);
					}
				}
				else
				{
					const std::string new_topic = params[1];
					if (_data->check_channel_flags(channel, TOPIC_OPER_CFLAG))
					{
						if (_data->check_member_status(channel, fd, OPER_MFLAG))
							_data->set_channel_topic(channel, new_topic);
						else
						{
							err_nochan_modes(fd, _args);
							err_chano_privsneeded(fd, _args);
						}
					}
					else
						_data->set_channel_topic(channel, new_topic);
				}
			}
			else
				err_noton_channel(fd, _args);
		}
	}
	else
		err_not_registered(fd, _args);
}

void	Command::names(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	int	p_size = params.size();
	if (_data->is_registered(fd))
	{
		if (p_size == 0)
		{
			std::vector<std::string> visible_channels = _data->list_visible_channels(fd);
			if (visible_channels.size() == 0)
			{
				_args.push_back("*");
				rpl_endof_names(fd, _args);
				return;
			}
			std::vector<std::string>::iterator iter_channels = visible_channels.begin();
			for (iter_channels = visible_channels.begin(); iter_channels != visible_channels.end(); ++iter_channels)
			{
				_args.push_back(*iter_channels);
				std::vector<std::string> users_channel = _data->get_members_list_str(*iter_channels);
				std::vector<int> users_fd = _data->get_members_list_fd(*iter_channels);
				std::vector<int>::iterator iter_fd = users_fd.begin();
				std::vector<std::string>::iterator iter_users = users_channel.begin();
				for (; iter_users != users_channel.end(); ++iter_users)
				{
					if (!_data->check_user_flags(*iter_fd, INVISIBLE_UFLAG))
					{
						std::string user = *iter_users;
						if (_data->check_member_status(*iter_channels, *iter_fd, OPER_MFLAG))
							user.insert(0,"@");
						else if(_data->check_member_status(*iter_channels, *iter_fd, VOICE_MFLAG))
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
		}
		else
		{
			if (p_size >= 2)
			{
				if (p_size > 2)
				{
					_args.push_back(params[1]);
					err_nosuch_server(fd, _args);
					return;
				}
				else
				{
					if (params[1] != _args[0])
					{
						_args.push_back(params[1]);
						err_nosuch_server(fd, _args);
						return;
					}
				}
			}
			std::vector<std::string> channels = parse_list(params[0]);
			std::vector<std::string>::iterator chan_iter = channels.begin();
			for (chan_iter = channels.begin(); chan_iter != channels.end(); ++chan_iter)
			{
				if (_data->channel_exists(*chan_iter))
				{
					_args.push_back(*chan_iter);
					std::vector<std::string> users_channel = _data->get_members_list_str(*chan_iter);
					std::vector<int> users_fd = _data->get_members_list_fd(*chan_iter);
					std::vector<int>::iterator iter_fd = users_fd.begin();
					std::vector<std::string>::iterator iter_users = users_channel.begin();
					for (; iter_users != users_channel.end(); ++iter_users)
					{
						if (!_data->check_user_flags(*iter_fd, INVISIBLE_UFLAG))
						{
							std::string user = *iter_users;
							if (_data->check_member_status(*chan_iter, *iter_fd, OPER_MFLAG))
								user.insert(0,"@");
							else if(_data->check_member_status(*chan_iter, *iter_fd, VOICE_MFLAG))
								user.insert(0, "+");
							_args.push_back(user);
							++iter_fd;
						}
					}
					rpl_nam_reply(fd, _args);
					_args.erase(_args.begin() + 2, _args.end());
					rpl_endof_names(fd, _args);
					_args.pop_back();
				}
				else
				{
					_args.erase(_args.begin() + 1, _args.end());
					_args.push_back(*chan_iter);
					rpl_endof_names(fd, _args);
				}
			}
		}	
	}
	else
		err_not_registered(fd, _args);
}

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
	_args.push_back(_data->get_srvname());
	if (!_data->is_registered(fd))
	{
		err_not_registered(fd, _args);
		return;
	}
	if (p_size == 0)
	{
		_args.push_back("PRIVMSG");
		err_notext_tosend(fd, _args);
		err_norecipient(fd, _args);
	}
	else if (p_size == 1)
		err_notext_tosend(fd, _args);
	else
	{
		std::vector<std::string> target = parse_list(params[0]);
		std::string message = params[1];
		std::vector<std::string>::iterator target_iter = target.begin();
		std::cout << "first element of vector: " << *target_iter << std::endl;
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
					std::string sender_nick = _data->get_nickname(fd);
					message = sender_nick + ": " + message + "\n";
					send(ret_fd, message.c_str(), message.size(), 0);
				}
				++target_iter;
			}
			else
				++target_iter;
			_args.erase(_args.begin() + 1, _args.end());
		}
	}
}

void	Command::notice(int fd, const std::vector<std::string>& params)
{
	int p_size = params.size();
	if (p_size > 2)
		return;
	_args.push_back(_data->get_srvname());
	if (_data->is_registered(fd))
	{
		if (p_size == 0)
			return;
		else if (p_size == 1)
			return;
		else
		{
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
						send_to_chan(fd, *target_iter, message);
					else if (ret_fd == -6)
						send_to_nick(fd, *target_iter, message);
					else
					{
						std::string sender_nick = _data->get_nickname(fd);
						message = sender_nick + ": " + message + "\n";
						send(ret_fd, message.c_str(), message.size(), 0);
					}
					++target_iter;
				}
				else
					++target_iter;
			}
		}
	}
	else
		err_not_registered(fd, _args);
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
	(void)fd;
	(void)params;
}

void	Command::ping(int fd, const std::vector<std::string>& params)
{
	std::cout << "PING" << std::endl;
	(void)fd;
	(void)params;
}


/*---------------------------HELPER FUNCTIONS---------------------------------*/

int		Command::parse_target(int fd, std::string &target)
{
	if (_data->channel_exists(target))
	{
		std::cout << "it is a channel" << std::endl;
		return -5;
	}
	else if (_data->nickname_exists(target))
	{
		std::cout << "it is a nickname" << std::endl;
		return -6;
	}
	else
	{
		Scanner scanner;
		scanner._current = target.begin();
		scanner._start = scanner._current;
		size_t pos = target.find_first_of("!@%", scanner._current - target.begin());
		if (pos == std::string::npos)
		{
			_args.push_back(target);
			if (target.at(0) == '#' || target.at(0) == '&' || target.at(0) == '+' || target.at(0) == '!')
			{
				std::cout << "PRIVMSG-PARSETARGET-NOSUCH-CHANNEL" << std::endl;
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
			std::cout << "scanner._current: " << *scanner._current;
			if (*scanner._current == '!')
			{
				std::cout << "it is a !: " << *scanner._current << std::endl;
				std::string nickname(scanner._start, scanner._current);
				std::cout << "nickname is: " << nickname << std::endl;
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
					std::cout << "USER: nickname!user is: " << user << std::endl;
					int dest_fd = _data->get_user_fd(nickname);
					if (_data->user_nick_match(nickname, user))
					{
						++scanner._current;
						std::string host(scanner._current, target.end());
						std::cout << "HOST: nickname!user@host is: " << host << std::endl;
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
				std::cout << "user is: " << user << std::endl;
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
					std::cout << "nani-meow" << std::endl;
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
						
						std::cout << "HOST: user'%'host: " << host << std::endl;
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
						std::cout << "HOST: user'%'host@servername: " << host << std::endl;
						++scanner._current;
						std::string servername(scanner._current, target.end());
						std::cout << "SERVERNAME: user'%'host@servername: " << servername << std::endl;
						if (servername == _data->get_srvname())
						{
							std::cout << "valid user: " << valid_user << std::endl;
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
				std::cout << "it is a @: " << *scanner._current << std::endl;
				std::string user(scanner._start, scanner._current);
				std::cout << "user is: " << user << std::endl;
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
				std::cout << "SERVERNAME: user@servername: " << servername << std::endl;
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
	}
	std::cout << "extra return" << std::endl;
	return -1;
}

void	Command::send_to_chan(int fd, std::string &channel, std::string &message)
{
	std::cout << "nani" << std::endl;
	_args.push_back(channel);
	if (_data->is_banned(fd, channel))
	{
		std::cout << "nani2" << std::endl;
		err_cannotsend_tochan(fd, _args);
	}
	else if (_data->check_channel_flags(channel, NO_MESSAGES_CFLAG))
	{
		std::cout << "nani3" << std::endl;
		if (_data->is_in_channel(fd, channel))
		{
			if (_data->check_channel_flags(channel, MODERATED_CFLAG))
			{
				if (_data->check_user_flags(fd, OPER_UFLAG))
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
								_args.pop_back();
								_args.push_back(_data->get_nickname(fd));
								rpl_away(fd, _args);
								_args.pop_back();
							}
							else
							{
								std::string sender_nick = _data->get_nickname(fd);
								message = sender_nick + ": " + message + "\n";
								send(*it, message.c_str(), message.size(), 0);
								message = original_message;
							}
						}
					}
				}
				else
					err_cannotsend_tochan(fd, _args);
			}
		}
		else
			err_cannotsend_tochan(fd, _args);
	}
	else if (_data->check_channel_flags(channel, MODERATED_CFLAG))
	{
		std::cout << "nani4" << std::endl;
		if (_data->check_user_flags(fd, OPER_UFLAG))
		{
			std::vector<int> users_chan = _data->get_members_list_fd(channel);
			std::vector<int>::iterator it = users_chan.begin();
			std::string original_message = message;
			for (; it != users_chan.end(); ++it)
			{
				if (_data->check_user_flags(*it, AWAY_UFLAG))
				{
					_args.pop_back();
					_args.push_back(_data->get_nickname(fd));
					rpl_away(fd, _args);
					_args.pop_back();
				}
				else
				{
					std::string sender_nick = _data->get_nickname(fd);
					message = sender_nick + ": " + message + "\n";
					send(*it, message.c_str(), message.size(), 0);
					message = original_message;
				}
			}
		}
		else
		{
			err_cannotsend_tochan(fd, _args);
			err_chano_privsneeded(fd, _args);
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
					_args.pop_back();
					_args.push_back(_data->get_nickname(fd));
					rpl_away(fd, _args);
					_args.pop_back();
				}
				else
				{
					std::string sender_nick = _data->get_nickname(fd);
					message = sender_nick + ": " + message + "\n";
					send(*it, message.c_str(), message.size(), 0);
					message = original_message;
				}	
			}
		}
	}
	std::cout << "nani5" << std::endl;
	// _args.erase(_args.begin() + 1, _args.end());
}	

void	Command::send_to_nick(int fd, std::string &nick, std::string &message)
{
	int dest_fd = _data->get_user_fd(nick);
	if (_data->check_user_flags(dest_fd, AWAY_UFLAG))
	{
		_args.push_back(nick);
		rpl_away(fd, _args);
	}
	else
	{
		std::string sender_nick = _data->get_nickname(fd);
		message = sender_nick + ": " + message + "\n";	
		send(dest_fd, message.c_str(), message.size(), 0);
	}
	_args.erase(_args.begin() + 1, _args.end());
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
	{
		std::cout << "it is a channel" << std::endl;
		return -5;
	}
	else if (_data->nickname_exists(target))
	{
		std::cout << "it is a nickname" << std::endl;
		return -6;
	}
	else
	{
		Scanner scanner;
		scanner._current = target.begin();
		scanner._start = scanner._current;
		size_t pos = target.find_first_of("!@%", scanner._current - target.begin());
		if (pos == std::string::npos)
		{
			if (target.at(0) == '#')
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
			std::cout << "scanner._current: " << *scanner._current;
			if (*scanner._current == '!')
			{
				std::cout << "it is a !: " << *scanner._current << std::endl;
				std::string nickname(scanner._start, scanner._current);
				std::cout << "nickname is: " << nickname << std::endl;
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
					std::cout << "USER: nickname!user is: " << user << std::endl;
					int dest_fd = _data->get_user_fd(nickname);
					if (_data->user_nick_match(nickname, user))
					{
						++scanner._current;
						std::string host(scanner._current, target.end());
						std::cout << "HOST: nickname!user@host is: " << host << std::endl;
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
				std::cout << "user is: " << user << std::endl;
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
						
						std::cout << "HOST: user'%'host: " << host << std::endl;
						if (_data->host_fd_match(valid_user, host))
							return (valid_user);
						else
							return -1;
					}
					else
					{
						std::cout << "HOST: user'%'host@servername: " << host << std::endl;
						++scanner._current;
						std::string servername(scanner._current, target.end());
						std::cout << "SERVERNAME: user'%'host@servername: " << servername << std::endl;
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
				std::cout << "it is a @: " << *scanner._current << std::endl;
				std::string user(scanner._start, scanner._current);
				std::cout << "user is: " << user << std::endl;
				int valid_user = _data->user_is_unique(user);
				if (valid_user == -1)
					return -1;
				else if (valid_user == 0)
					return -1;
				++scanner._current;
				std::string servername(scanner._current, target.end());
				std::cout << "SERVERNAME: user@servername: " << servername << std::endl;
				if (servername == _data->get_srvname())
					return (valid_user);
				else
					return -1;
			}
			else
				return -1;
		}
	}
	std::cout << "extra return" << std::endl;
	return -1;
}

void	Command::send_to_chan_notice(int fd, std::string &channel, std::string &message)
{
	if (_data->check_user_flags(fd, RESTRICTED_UFLAG))
	{
		std::cout << "nani2" << std::endl;
		return ;
	}
	else if (_data->check_channel_flags(channel, NO_MESSAGES_CFLAG))
	{
		std::cout << "nani3" << std::endl;
		if (_data->is_in_channel(fd, channel))
		{
			if (_data->check_channel_flags(channel, MODERATED_CFLAG))
			{
				if (_data->check_user_flags(fd, OPER_UFLAG))
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
								std::string sender_nick = _data->get_nickname(fd);
								message = sender_nick + ": " + message + "\n";
								send(*it, message.c_str(), message.size(), 0);
								message = original_message;
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
		std::cout << "nani4" << std::endl;
		if (_data->check_user_flags(fd, OPER_UFLAG))
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
					std::string sender_nick = _data->get_nickname(fd);
					message = sender_nick + ": " + message + "\n";
					send(*it, message.c_str(), message.size(), 0);
					message = original_message;
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
					std::string sender_nick = _data->get_nickname(fd);
					message = sender_nick + ": " + message + "\n";
					send(*it, message.c_str(), message.size(), 0);
					message = original_message;
				}	
			}
		}
	}
	std::cout << "nani5" << std::endl;
}	

void	Command::send_to_nick_notice(int fd, std::string &nick, std::string &message)
{
	int dest_fd = _data->get_user_fd(nick);
	if (_data->check_user_flags(dest_fd, AWAY_UFLAG))
	{
		return;
	}
	else
	{
		std::string sender_nick = _data->get_nickname(fd);
		message = sender_nick + ": " + message + "\n";	
		send(dest_fd, message.c_str(), message.size(), 0);
	}
}