#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "SocketServer.hpp"
#include "Data.hpp"
#include "Command.hpp"
#include "Reply.hpp"

class Server
{
	typedef	std::vector<struct pollfd>::iterator pfd_iter;

	private:
		std::vector<struct pollfd>			_pfds;
		int 								_listener;
		std::map<int, Client*>				_clients;
		char								remoteIP[INET6_ADDRSTRLEN];
		char 								host[NI_MAXHOST];
		Data								*_data;
		std::map<int, Buffer>               _storage_map;

	public:
	
	Server(int listener, Data* data);

	void		add_to_pfds(int new_fd);
	void		run();
	void		*get_in_addr(struct sockaddr *sa);
	void		send_data(int numbytes, int sender_fd);
	void		del_from_pfds(pfd_iter iter);

	int			get_fd(int i);
	int			accept_connection(size_t location);
	int			receive_send_data(pfd_iter iter);
	int			receive_data(pfd_iter iter, int *numbytes);
	void		ping(int fd);
	pfd_iter	handle_timeout(pfd_iter _iter);
	pfd_iter	disconnect_user(pfd_iter iter);


};

#endif
