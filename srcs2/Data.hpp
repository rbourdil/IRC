#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

// USER_FLAGS
#define AWAY_UFLAG 0x1
#define INVISIBLE_UFLAG 0x2
#define WALLOPS_UFLAG 0x4
#define RESTRICTED_UFLAG 0x8
#define OPER_UFLAG 0x10
#define LOCAL_OPER_UFLAG 0x20
#define SRV_NOTICE_UFLAG 0x40

// MEMBER STATUS
#define CREATOR_MFLAG 0x1
#define OPER_MFLAG 0x2
#define VOICE_MFLAG 0x4

// CHANNEL FLAGS
#define ANON_CFLAG 0x1
#define INVITE_ONLY_CFLAG 0x2
#define MODERATED_CFLAG 0x4
#define NO_MESSAGES_CFLAG 0x8
#define QUIET_CFLAG 0x10
#define PRIVATE_CFLAG 0x20
#define SECRET_CFLAG 0x40
#define SRV_REOP_CFLAG 0x80
#define TOPIC_OPER_CFLAG 0x100
#define KEY_CFLAG 0x200
#define USER_LIMIT_CFLAG 0x400
#define BAN_MASK_CFLAG 0x800
#define EXCEPT_MASK_CFLAG 0x1000
#define INVIT_MASK_CFLAG 0x2000

// CHANNEL TYPES
#define HASH_CHAN 256
#define AND_CHAN 257
#define PLUS_CHAN 258
#define BANG_CHAN 259

// USER STATES
#define UNREGISTERED_STATE 256
#define NICK_STATE 257
#define REGISTERED_STATE 258

struct	Client {

	int							_state;
	std::string					_nickname;
	std::string					_username;
	std::string					_hostname;
	std::string					_realname;
	std::set<std::string>		_channels;
	int							_mode;

	Client(void) : _state(UNREGISTERED_STATE), _mode(0) { }

};

struct	Channels {

	int					_type;
	std::string			_topic;
	std::map<int, int>	_members;
	int					_mode;

	Channels(void) : _mode(0) { }

};

class	Data {

	typedef std::map<int, Client>::iterator					client_iterator;
	typedef std::map<int, Client>::const_iterator			client_const_iterator;
	typedef std::map<std::string, Channels>::iterator		channel_iterator;
	typedef std::map<std::string, Channels>::const_iterator	channel_const_iterator;
	typedef std::map<std::string, int>::iterator			strint_iterator;
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

		void	set_user_flags(int fd, int flags)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._mode |= flags;
		}

		void	unset_user_flags(int fd, int flags)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._mode &= (~flags);
		}

		void	add_channel_to_user(int fd, const std::string& channel)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._channels.insert(channel);
		}

		void	remove_channel_from_user(int fd, const std::string& channel)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._channels.erase(channel);
		}

			// channel operations
		void	add_user_to_channel(int fd, const std::string& channel)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._members.insert(std::make_pair(fd, 0));
		}

		void	remove_user_from_channel(int fd, const std::string& channel)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._members.erase(fd);
		}

		void	set_channel_type(const std::string& channel, int type)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._type = type;
		}

		void	set_channel_topic(const std::string& channel, const std::string& topic)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._topic = topic;
		}

		void	set_channel_flags(const std::string& channel, int flags)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._mode |= flags;
		}

		void	unset_channel_flags(const std::string& channel, int flags)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._mode &= (~flags);
		}

		void	set_member_status(const std::string& channel, int fd, int status)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				std::map<int, int>::iterator	itm = it->second._members.find(fd);
				if (itm != it->second._members.end())
					itm->second |= status;
			}
		}

		void	unset_member_status(const std::string& channel, int fd, int status)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				std::map<int, int>::iterator	itm = it->second._members.find(fd);
				if (itm != it->second._members.end())
					itm->second &= (~status);
			}
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

		const std::string&	get_nickname(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._nickname);
			else
				throw std::runtime_error("no account registered with this file descriptor");
		}

		int	get_user_state(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._state);
			else
				throw std::runtime_error("no account registered with this file descriptor");

		}

		bool	is_registered(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._state == REGISTERED_STATE);
			else
				throw std::runtime_error("no account registered with this file descriptor");
		}

		int	get_user_fd(const std::string& nick) const
		{
			strint_const_iterator	it = _nick_to_fd.find(nick);

			if (it != _nick_to_fd.end())
				return (it->second);
			else
				throw std::runtime_error("no account registered with this file descriptor");
		}

		// check if flag 'flag' is present in the user mode attribute
		bool	check_user_flags(int fd, int flags) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._mode & flags);
			else
				throw std::runtime_error("no account registered with this file descriptor");
		}

		int	user_channel_count(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			
			if (it != _clients.end())
				return (it->second._channels.size());
			else
				throw std::runtime_error("no account registered with this file descriptor");
		}

			// channel lookup
		int	get_channel_type(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._type);
			else
				throw std::runtime_error("channel name does not exist");
		}

		const std::string&	get_channel_topic(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._topic);
			else
				throw std::runtime_error("channel name does not exist");
		}
		
		bool	check_channel_flags(const std::string& channel, int flags) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._mode & flags);
			else
				throw std::runtime_error("channel name does not exist");
		}

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

		void	print_channels(const std::set<std::string>& channels) const
		{
			std::set<std::string>::const_iterator	it;
			for (it = channels.begin(); it != channels.end(); it++)
				std::cout << *it << std::endl;
		}


};

#endif
