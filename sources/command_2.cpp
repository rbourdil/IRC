/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:09:41 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/11 23:38:41 by pcamaren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

//TOPIC CHECK IF CHANNEL IS SECRET

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
					std::string user = *iter_users;
					if (_data->check_member_status(*iter_channels, *iter_fd, OPER_MFLAG))
						user.insert(0,"@");
					else if(_data->check_member_status(*iter_channels, *iter_fd, VOICE_MFLAG))
						user.insert(0, "+");
					_args.push_back(user);
					++iter_fd;
				}
				rpl_nam_reply(fd, _args);
				_args.erase(_args.begin() + 2, _args.end());
				rpl_endof_names(fd, _args);
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
						std::string user = *iter_users;
						if (_data->check_member_status(*chan_iter, *iter_fd, OPER_MFLAG))
							user.insert(0,"@");
						else if(_data->check_member_status(*chan_iter, *iter_fd, VOICE_MFLAG))
							user.insert(0, "+");
						_args.push_back(user);
						++iter_fd;
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
	std::string server_name = _data->get_srvname();
	_data->set_user_last_pong(fd);
	_data->set_user_was_ping(fd, false);
	_args.push_back(server_name);
	int	size_p = params.size();
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
	_args.push_back(_data->get_srvname());
	int p_size = params.size();
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
		// std::vector<std::string> target = parse_list(params[0]);
		// std::string message = params[1];
		// std::vector<std::string>::iterator target_iter = target.begin();
		//if domain
			//not allowed
		//if it is channel
			//check all are channel
				//if all are channel
					//for each channel
						//if channel exists
							//if +m
								//if client has permission
									//for each user in channel
										//if user is away
											//rpl_away
										//else
											//send
								//else
									//err_cannotsendtochan
							//if +n
								//if the user has joined the channel
									//for each user in channel
										//if user is away
											//rpl_away
										//else
											//send
								//else
									//err_cannotsendtochan
							//if size > 5
								//err_toomanytargets
						//else
							//err_nosuchchannel
				//else
					//err_nosuchchan
		//else if it is a nickname
			//check all are nickname
				//if all are nickname
					//for each user
						//if nickname exists
							//if user away
								//rpl_way -> continue
							//if user on the same channel
								//send
							//else
								//error you are not on channel
						//else
							//err_nosuchnick
				//else
					//error
		//else if it is user
			//for each user
				//if user is unique
					//send
				//else
					//err_toomanytarget
	}
}

/*---------------------------HELPER FUNCTIONS---------------------------------*/

bool isTopLevelDomain(std::string target) {
    // Check that the target string is not empty
    if (target.empty()) {
        return false;
    }

    // Check that the target string ends with a dot and a TLD
    size_t pos = target.find_last_of(".");
    if (pos == std::string::npos || pos == target.size() - 1) {
        return false;
    }
    std::string tld = target.substr(pos + 1);

    // Check that the TLD matches the pattern
    if (tld.size() < 2 || tld.size() > 63) {
        return false;
    }
    for (size_t i = 0; i < tld.size(); i++) {
        if (!isalpha(tld[i])) {
            return false;
        }
    }

    return true;
}
