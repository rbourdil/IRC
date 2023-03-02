#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>

#define AWAY_FLAG 0x1
#define INVISIBLE_FLAG 0x2
#define WALLOPS_FLAG 0x4
#define RESTRICTED_FLAG 0x8
#define OPER_FLAG 0x10
#define LOCAL_OPER_FLAG 0x20
#define SRV_NOTICE_FLAG 0x40

#define UNREGISTERED_STATE 256
#define NICK_STATE 257
#define REGISTERED_STATE 258

struct	Client {

	int							_state;
	std::string					_nickname;
	std::string					_username;
	std::string					_hostname;
	std::string					_realname;
	std::vector<std::string>	_channels;
	int							_mode;

	Client(void) : _state(UNREGISTERED_STATE), _mode(0) { }

};

struct	Channels {

	std::vector<int>	_members;
	int					_flags;

	Channels(void) : _flags(0) { }

};

class	Data {

	typedef std::map<int, Client>::iterator				client_iterator;
	typedef std::map<int, Client>::const_iterator				client_const_iterator;
	typedef std::map<std::string, Channels>::iterator	channel_iterator;
	typedef std::map<std::string, Channels>::const_iterator	channel_const_iterator;
	typedef std::map<std::string, int>::iterator		strint_iterator;
	typedef std::map<std::string, int>::const_iterator		strint_const_iterator;

	private:

		const std::string	_passwd;

		// used to lookup by nickname
		std::map<std::string, int>	_nick_to_fd;

		// actual data
		// clients are looked up by nickname
		// channels are looked up by name
		std::map<int, Client>				_clients;
		std::map<std::string, Channels>		_channels;

			
	public:

		// constructor
		explicit Data(const char* passwd) : _passwd(passwd) { }

		// modifiers

			//user operations
		void	add_user(int fd)
		{
			_clients.insert(std::make_pair(fd, Client()));
		}

		void	delete_user(int fd)
		{
			client_iterator	it;
			it = _clients.find(fd);
			_nick_to_fd.erase(it->second._nickname);
			_clients.erase(it);
		}		
		
		void	add_nickname(int fd, const std::string& nick)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
			{
				it->second._nickname = nick;
				_nick_to_fd.insert(std::make_pair(nick, fd));
			}
		}

		void	add_username(int fd, const std::string& nick)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._username = nick;
		}

		void	add_hostname(int fd, const std::string& nick)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._hostname = nick;
		}

		void	add_realname(int fd, const std::string& realname)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._realname = realname;
		}

		void	set_state(int fd, int state)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._state = state;
		}

		void	set_flags(int fd, int flags)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._mode |= flags;
		}

		void	unset_flags(int fd, int flags)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._mode &= (~flags);
		}

		void	add_channel_to_user(int fd, const std::string& channel)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._channels.push_back(channel);
		}

		void	remove_channel_from_user(int fd, const std::string& channel)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				remove_from_vector(channel, it->second._channels);
		}

		// lookup

			// user lookup
		bool	compare_passwd(const std::string& passwd) const
		{
			return (passwd == _passwd);
		}
		
		bool	nickname_exists(const std::string& nick) const
		{
			strint_const_iterator	it = _nick_to_fd.find(nick);

			if (it != _nick_to_fd.end())
				return (true);
			return (false);
		}

		int	get_state(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			return (it->second._state);
		}

		bool	is_registered(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			return (it->second._state == REGISTERED_STATE);
		}

		int	get_fd(const std::string& nick) const
		{
			strint_const_iterator	it = _nick_to_fd.find(nick);

			if (it != _nick_to_fd.end())
				return (it->second);
			return (-1);
		}

		// check if flag 'flag' is present in the user mode attribute
		bool	check_flags(int fd, int flags) const
		{
			client_const_iterator	it = _clients.find(fd);
			return (it->second._mode & flags);
		}

		int	user_channel_count(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			return (it->second._channels.size());
		}

			// channel lookup


		// DEBUG

		void	print_user(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			
			if (it != _clients.end())
			{
				std::cout << it->second._nickname << "!";
				std::cout << it->second._username << "@";
				std::cout << it->second._hostname << std::endl;
				std::cout << it->second._realname << std::endl;
				print_channels(it->second._channels);
			}
		}

		void	print_channels(const std::vector<std::string>& channels) const
		{
			std::vector<std::string>::const_iterator	it;
			for (it = channels.begin(); it != channels.end(); it++)
				std::cout << *it << std::endl;
		}

	private:

		// helper functions
		template <typename T>
		void	remove_from_vector(const T& elem, std::vector<T>& vec)
		{
			typename std::vector<T>::iterator	it;

			for (it = vec.begin(); it != vec.end(); it++)
			{
				if (*it == elem)
				{
					vec.erase(it);
					break;
				}
			}
		}

};

#endif
