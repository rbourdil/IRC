/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 19:46:46 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/15 19:38:29 by pcamaren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/SocketServer.hpp"
#include "../includes/Server.hpp"
#include "../includes/Data.hpp"
#include <fstream>

int g_die = 0;

static void	handle_signal(int signal)
{
	std::cout << "caught signal" << signal << std::endl;
	g_die = 1;
}


int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "usage: ./ircserv <port> <password>";
		exit(1);
	}

	struct sigaction sigact;
    sigact.sa_handler = handle_signal;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;

	std::ifstream	hostfile("/etc/hostname");
	std::string		hostname;
	hostfile >> hostname;
	Data data(hostname, av[2]);

	if (sigaction(SIGINT, &sigact, NULL) == -1) {
        std::cerr << "Failed to install signal handler.\n";
        return 1;
    }
	
	SocketServer socket_server(av[1]);
	int	listener = socket_server.get_listener();
	Server server(listener, &data);
	server.run();
	return 0;
}
