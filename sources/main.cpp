/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 19:46:46 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/06 18:37:09 by rbourdil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/SocketServer.hpp"
#include "../includes/Server.hpp"
#include "../includes/Data.hpp"
#include <fstream>

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "usage: ./ircserv <port> <password>";
		exit(1);
	}

	std::ifstream	hostfile("/etc/hostname");
	std::string		hostname;
	hostfile >> hostname;
	Data data(hostname, av[2]);

	SocketServer socket_server(av[1]);
	int	listener = socket_server.get_listener();
	Server server(listener, &data);
	server.run();
	return 0;
}
