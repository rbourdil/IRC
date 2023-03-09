/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 19:47:15 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/09 13:45:54 by rbourdil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/SocketServer.hpp"

SocketServer::SocketServer(const char *port) : _port(port)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, _port, &hints, &servinfo) != 0)
	{
		perror("getaddrinfo:");
		exit(1);
	}
	socket_bind();
	socket_listen();
}

SocketServer::~SocketServer()
{
	if (listener > 0)
	{
		shutdown(listener, 2);
		close(listener);
	}
}

void	SocketServer::socket_bind()
{
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		int yes = 1;
		if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))
		{
			perror("setsockopt:");
			exit(1);
		}
		if (fcntl(listener, F_SETFL, O_NONBLOCK) < 0)
		{
			perror("fcntl:");
			exit(1);
		}
		if (bind(listener, p->ai_addr, p->ai_addrlen) == -1)
			continue;
		break;
	}
		freeaddrinfo(servinfo);
	if (p == NULL)
	{
		std::cerr << "ERROR: failed to bind socket" << std::endl;
		exit(1);
	}
}

void	SocketServer::socket_listen()
{
		if (listen(listener, 10) == -1)
	{
		std::cerr << "ERROR: failed to listen on socket" << std::endl;
		exit(1);
	}
}

int	SocketServer::get_listener(void) const
{
	return listener;
}

