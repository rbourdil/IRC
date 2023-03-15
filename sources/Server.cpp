#include "../includes/Server.hpp"
#include "../includes/Buffer.hpp"
#include <sys/types.h>
#include <sys/socket.h>

#include "../debug.hpp"

Server::Server(int listener, Data *data) : _listener(listener), _data(data)
{
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

pfd_iter	Server::handle_timeout(pfd_iter _iter)
{
	double time_diff = difftime(std::time(NULL), _data->get_user_last_move(_iter->fd));
	double pong_diff = difftime(std::time(NULL), _data->get_user_last_pong(_iter->fd));

	if ((!_data->get_user_was_ping(_iter->fd)) && time_diff > PING_TIME && time_diff < DISCONNECT_TIME)
	{
		ping(_iter->fd);
		_data->set_user_last_pong(_iter->fd);
		++_iter;
	}
	else if (_data->get_user_was_ping(_iter->fd) && pong_diff > PONG_TIME)
		_iter = disconnect_user(_iter);
	else
		++_iter;
	return (_iter);
}

pfd_iter Server::disconnect_user(pfd_iter iter)
{
	std::cout << "Will disconnect client from socket " << iter->fd << " for innactivity" << std::endl;
	std::string	err_message = "ERROR: You are being disconnected for innactivity, bye boo xoxo\n";
	send(iter->fd, err_message.c_str(), err_message.size(), 0);
	_data->delete_user(iter->fd);
	close(iter->fd);
	_storage_map.erase(iter->fd);
	iter = _pfds.erase(iter);
	return (iter);
}

void	Server::ping(int fd)
{
	_data->set_user_was_ping(fd, true);
	std::string ping_message = "PING :" + _data->get_srvname() + "\n";
	send(fd, ping_message.c_str(), ping_message.size(), 0);
}


void	Server::run()
{

	char					buff[BUFSIZE];

	if (DEBUG)
	{
		std::cout << "___________________" << std::endl;
		std::cout << " pfds[0].fd: " << _pfds[0].fd << " pfds[0].revents: " << _pfds[0].revents << std::endl;
		std::cout << " pfds[0].fd: " << _pfds[0].fd << " pfds[0].revents: " << _pfds[0].revents << std::endl;
		std::cout << "array is empty: " << _pfds.empty() <<  " size: " << _pfds.size() << std::endl;
	}
	while (1)
	{
		if (g_die == 1)
			return;
		int poll_count = poll(&_pfds[0], _pfds.size(), TIMEOUT);

        // the poll call was interrupted by a signal, retry
		if (errno == EINTR)
        	continue;

		if (poll_count == -1)
		{
			perror("poll:");
			exit(1);
		}
		if (_pfds[0].revents & POLLIN)
		{
			int fd = accept_connection(0);
			_data->add_user(fd, host, remoteIP);
			_storage_map.insert(std::make_pair(fd, Buffer()));
		}
		pfd_iter _iter = _pfds.begin() + 1;
		while (_iter != _pfds.end())
		{
			if (_iter->revents == 0)
			{
				// _iter = handle_timeout(_iter);
				++_iter;
				continue;
			}
			if ( _iter->revents & POLLIN)
			{
				ssize_t	count = recv(_iter->fd, buff, BUFSIZE, 0);
				if (count < 0)
				{
					perror("read");
					exit(EXIT_FAILURE);
				}
				else if (count == 0)
				{
					std::cerr << "Client from socket: " << _iter->fd << ": hung up" << std::endl;
					_data->delete_user(_iter->fd);
					close(_iter->fd);
					_storage_map.erase(_iter->fd);
					_iter = _pfds.erase(_iter);
				}
				else
				{
					_data->set_user_last_move(_iter->fd);
					Buffer&	storage = (_storage_map.find(_iter->fd))->second;
					if (storage.size() + count > BUFSIZE)
					{
						std::cerr << _iter->fd << " overflown its buffer" << std::endl;
						_data->delete_user(_iter->fd);
						close(_iter->fd);
						_storage_map.erase(_iter->fd);
						_iter = _pfds.erase(_iter);
					}
					else
					{
						std::string	msg = get_raw(std::string(buff, buff + count));
						std::cerr << "[ MSG ] " << msg << std::endl;
						ssize_t	i,j;

						i = j = 0;
						while (i < count)
						{
							while (j < count)
							{
								if (buff[j] == '\r')
								{
									j++;
									if (buff[j] == '\n')
									{
										j++;
										break;
									}
								}
								else 
									j++;
							}
							storage.append(buff + i, j);
							parser	p(storage);
							p.parse();
							if (p.state() == VALID_CMD)
							{
								Command	exec(_data);
								irc_cmd	cmd;

								cmd = p.out();
								exec.execute_cmd(_iter->fd, cmd);
								if (g_die == 1)
									return;
							}
							if (p.state() == VALID_CMD || p.state() == DUMP_CMD)
								storage.reset();
							i = j;
						}
						if (!_data->is_connected(_iter->fd))
						{
							std::cerr << "Client from socket: " << _iter->fd << " hung up" << std::endl;
							close(_iter->fd);
							_storage_map.erase(_iter->fd);
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
