#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "SocketServer.hpp"
#include "Data.hpp"
#include "Command.hpp"
#include "Reply.hpp"

class Server
{
	private:
		std::vector<struct pollfd>			_pfds;
		int 								_listener;
		char								buff[256];
		std::map<int, Client*>				_clients;
		char								remoteIP[INET6_ADDRSTRLEN];
		char 								host[NI_MAXHOST];

		// std::map<std::string, Channel*>		_channels;
		Data								*_data;
	public:
	
	Server(int listener, Data* data);

	void add_to_pfds(int new_fd);

	void del_from_pfds(pfd_iter iter);

	int	get_fd(int i);

	void	*get_in_addr(struct sockaddr *sa);

	int		accept_connection(size_t location);

	int		receive_send_data(pfd_iter iter);

	void	send_data(int numbytes, int sender_fd);

	int		receive_data(pfd_iter iter, int *numbytes);

	void run();

};

#endif
