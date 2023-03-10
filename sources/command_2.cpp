/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:09:41 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/10 19:55:53 by pcamaren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

//check if i am a member of that channel

//TOPIC CHECK IF CHANNEL IS SECRET

void	Command::topic_dispatch(int fd, const std::vector<std::string>& params)
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

// void	Command::names_dispatch(int fd, const std::vector<std::string>& params)
// {
// 	_args.push_back(_data->get_srvname());
// 	if (_data->is_registered(fd))
// 	{
// 		if (params.size() == 0)
// 		{
// 			//it is the current 
// 		}
// 		std::vector<std::string>			channels = parse_list(params[0]);
// 		std::vector<std::string>::iterator	iter = channels.begin();
// 		for (; iter != channels.end(); ++iter)
// 			names(fd, *iter, params);		
// 	}
// 	else
// 		err_not_registered(fd, _args);

// }

// void	Command::names(int fd, std::string channel, const std::vector<std::string>& params)
// {
// 	(void)fd;
// 	(void)channel;
// 	(void)params;
// }