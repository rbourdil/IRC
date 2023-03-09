#include "../includes/Server.hpp"
#include "../includes/Buffer.hpp"
#include <sys/types.h>
#include <sys/socket.h>

Server::Server(int listener, Data *data) : _listener(listener), _data(data)
{
	_listener = listener;
	_pfds.push_back(pollfd());
	_pfds.back().fd = listener;
	_pfds.back().events = POLLIN;
	_pfds.back().revents = 0;
}

void Server::add_to_pfds(int new_fd)
{
	_pfds.push_back(pollfd());
	_pfds.back().fd = new_fd;
	_pfds.back().events = POLLIN;
	_pfds.back().revents = 0;
}

void Server::del_from_pfds(pfd_iter iter)
{
	_pfds.erase(iter);
}

int	Server::get_fd(int i)
{
	return _pfds[i].fd;
}

void	*Server::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int	Server::accept_connection(size_t location)
{
	socklen_t				size_c_addr;
	struct sockaddr_storage	c_addr;
	int						new_fd;

	if (DEBUG)
		std::cout << "I am in the listener" << std::endl;
	size_c_addr = sizeof(c_addr);
	new_fd = accept(_listener, (struct sockaddr *)&c_addr, &size_c_addr);
	if (new_fd == -1)
	{
		perror("accept");
		return (-1);
	}
	else
	{
		if (DEBUG)
		{
			std::cout << "___________________" << std::endl;
			std::cout << "i: " << location << " pfds[i+1].fd: " << _pfds[location+1].fd << " pfds[i+1].revents: " << _pfds[location+1].revents << std::endl;
		}
		if (DEBUG)
		{
			std::cout << "___________________" << std::endl;
			std::cout << "i: " << location << " pfds[i+1].fd: " << _pfds[location+1].fd << " pfds[i+1].revents: " << _pfds[location+1].revents << std::endl;
		}

		const char *result = inet_ntop(c_addr.ss_family, get_in_addr((struct sockaddr*)&c_addr), remoteIP, INET6_ADDRSTRLEN);
		if (result == NULL) {
			perror("inet_ntop");
			exit(EXIT_FAILURE);
		}
		int err;
		if ((err = getnameinfo((struct sockaddr*)&c_addr, sizeof(c_addr), host, NI_MAXHOST, NULL, 0, 0)) != 0)
			perror("getnameinfo: ");
		else
			std::cout << "Hostname: " << host << std::endl;		
		if ((fcntl(new_fd, F_SETFL, O_NONBLOCK)) < 0)
		{
			perror("fcntl: ");
			exit(1);
		}
		add_to_pfds(new_fd);
		std::cout << "pollserver: new connection from " << remoteIP << " on socket: " << new_fd << std::endl;
		return (new_fd);
	}
}

int	Server::receive_send_data(pfd_iter iter)
{
	if (DEBUG)
		std::cout << "I am in the client" << std::endl;
	int	numbytes;
	int	sender_fd = receive_data(iter, &numbytes);
	if (numbytes > 0 && sender_fd != -1)
	{
		send_data(numbytes, sender_fd);
		return (0);
	}
	return (-1);
}

void	Server::send_data(int numbytes, int sender_fd)
{
	for (size_t j = 0; j < _pfds.size(); j++)
	{
		int dest_fd = _pfds[j].fd;
		if (dest_fd != _listener && dest_fd != sender_fd)
		{
			if (send(dest_fd, buff, numbytes, 0) == -1)
				perror("send:");
		}
	}
}

int		Server::receive_data(pfd_iter iter, int *numbytes)
{
	*numbytes = recv(iter->fd, buff, sizeof(buff), 0);
	if (*numbytes < 0)
	{
		perror("recv:");
		close(iter->fd);
		del_from_pfds(iter);
		return (-1);
	}
	return (iter->fd);
}

void	Server::run()
{
	char					buff[BUFSIZE];
	std::map<int, Buffer>	bufmap;

	if (DEBUG)
	{
		std::cout << "___________________" << std::endl;
		std::cout << " pfds[0].fd: " << _pfds[0].fd << " pfds[0].revents: " << _pfds[0].revents << std::endl;
		std::cout << " pfds[0].fd: " << _pfds[0].fd << " pfds[0].revents: " << _pfds[0].revents << std::endl;
		std::cout << "array is empty: " << _pfds.empty() <<  " size: " << _pfds.size() << std::endl;
	}
	while (1)
	{
		int poll_count = poll(&_pfds[0], _pfds.size(), 50000);

		if (poll_count == -1)
		{
			perror("poll:");
			exit(1);
		}
		else if (poll_count == 0)
		{
			std::cout << "Timeout" << std::endl;
			continue;
		}	
		if (_pfds[0].revents & POLLIN)
		{
			int fd = accept_connection(0);
			_data->add_user(fd, host, remoteIP);
			bufmap.insert(std::make_pair(fd, Buffer()));
		}
		std::vector<struct pollfd>::iterator _iter = _pfds.begin() + 1;
		while (_iter != _pfds.end())
		{
			if (_iter->revents == 0)
			{
				++_iter;
				continue;
			}
			if ( _iter->revents & POLLIN)
			{
				ssize_t	count = recv(_iter->fd, buff, BUFSIZE, 0);
				if (count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
				{
					std::cerr << _iter->fd << " blocking error" << std::endl;
					_data->delete_user(_iter->fd);
					close(_iter->fd);
					bufmap.erase(_iter->fd);
					_iter = _pfds.erase(_iter);
				}
				else if (count < 0)
				{
					perror("read");
					exit(EXIT_FAILURE);
				}
				else if (count == 0)
				{
					std::cerr << _iter->fd << " close connection" << std::endl;
					_data->delete_user(_iter->fd);
					close(_iter->fd);
					bufmap.erase(_iter->fd);
					_iter = _pfds.erase(_iter);
				}
				else
				{
					Buffer&	storage = (bufmap.find(_iter->fd))->second;
					if (storage.size() + count > BUFSIZE)
					{
						std::cerr << _iter->fd << " overflown its buffer" << std::endl;
						_data->delete_user(_iter->fd);
						close(_iter->fd);
						bufmap.erase(_iter->fd);
						_iter = _pfds.erase(_iter);
					}
					else
					{
						ssize_t	i = 0;
						while (buff[i++] != '\n' && i < count)
							;
						storage.append(buff, i);
						parser	p(storage);
						p.parse();
						if (p.state() == VALID_CMD)
						{
							Command	exec(_data);
							irc_cmd	cmd;

							cmd = p.out();
							exec.execute_cmd(_iter->fd, cmd);
						}
						if (p.state() == VALID_CMD || p.state() == DUMP_CMD)
							storage.reset();
						storage.append(buff + i, count - i);
						if (!_data->is_connected(_iter->fd))
						{
							close(_iter->fd);
							bufmap.erase(_iter->fd);
							_iter = _pfds.erase(_iter);
						}
						else
							++_iter;
					}
				}
		}
	}
}
}
