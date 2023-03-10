/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_2.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 19:09:41 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/10 14:28:31 by pcamaren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

void	Command::topic_dispatch(int fd, const std::vector<std::string>& params)
{
	_args.push_back(_data->get_srvname());
	size_t p_size = params.size();
	if (p_size < 1)
	{
		_args.push_back("TOPIC");
		err_need_moreparams(fd, _args);
	}
	else
	{
		std::string channel = params[0];
		std::cout << "channel:" << channel << std::endl;
	}
}

void	Command::topic(int fd, std::string channel, const std::vector<std::string>& params)
{
	(void)fd;
	(void)channel;
	(void)params;
}