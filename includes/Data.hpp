#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "headers.hpp"
#include "parser.hpp"

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

// USER STATES
#define UNREGISTERED_STATE 0x0
#define NICK_VALID 0x1
#define PASSWD_VALID 0x2
#define USER_VALID 0x4
#define REGISTERED_STATE 0x7

struct	Client {

	int							_state;
	std::string					_nickname;
	std::string					_username;
	std::string					_hostname;
	std::string					_hostaddress;
	std::string					_realname;
	std::set<std::string>		_channels;
	int							_mode;
	
	time_t						_last_move;
	time_t						_last_pong;
	bool						_was_ping;

	Client(const std::string& hostname, const std::string& hostaddress) : _state(UNREGISTERED_STATE), _hostname(hostname), _hostaddress(hostaddress), _mode(0), _last_move(std::time(NULL)), _was_ping(false) { }

};

struct	Channel {

	std::string			_topic;
	std::string			_key;
	std::map<int, int>	_members;
	int					_mode;
	std::set<std::string>	_ban_masks;
	std::set<std::string>	_except_masks;
	std::set<std::string>	_invit_masks;
	unsigned int		_members_limit;

	Channel(int mode) : _mode(mode), _members_limit(0) { }

};

class	Data {

	typedef std::map<int, Client>::iterator					client_iterator;
	typedef std::map<int, Client>::const_iterator			client_const_iterator;
	typedef std::map<std::string, Channel>::iterator		channel_iterator;
	typedef std::map<std::string, Channel>::const_iterator	channel_const_iterator;
	typedef std::map<std::string, int>::iterator			strint_iterator;
	typedef std::map<std::string, int>::const_iterator		strint_const_iterator;

	private:

		const std::string	_srvname;
		const std::string	_passwd;

		const std::string	_oper_name;
		const std::string	_oper_passwd;

		// used to lookup by nickname
		std::map<std::string, int>	_nick_to_fd;

		// actual data
		// clients are looked up by nickname
		// channels are looked up by name
		std::map<int, Client>				_clients;
		std::map<std::string, Channel>		_channels;

			
	public:

		// constructor
		explicit Data(const std::string& srvname, const char* passwd) : _srvname(srvname), _passwd(passwd), _oper_name("operator"), _oper_passwd("operator") { }

		// modifiers

			//user operations
		void	add_user(int fd, const std::string& hostname, const std::string& hostaddress)
		{
			_clients.insert(std::make_pair(fd, Client(hostname, hostaddress)));
		}

		void	delete_user(int fd) // deletes user and removes it from any channel it belongs
		{
			client_iterator	it;
			it = _clients.find(fd);
			if (it != _clients.end())
			{
				std::set<std::string>::iterator	itc = it->second._channels.begin();
				for (; itc !=  it->second._channels.end(); ++itc)
					remove_user_from_channel(fd, *itc);
				_nick_to_fd.erase(it->second._nickname);
				_clients.erase(it);
			}
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

		void	set_user_was_ping(int fd, bool was_pong)
		{
			client_iterator it = _clients.find(fd);
			if (it != _clients.end())
			{
				if (was_pong)
					it->second._was_ping = true;
				else
					it->second._was_ping = false;
			}
		}

		void	set_user_last_move(int fd)
		{
			client_iterator it = _clients.find(fd);
			if (it != _clients.end())
				it->second._last_move = std::time(NULL);
		}

		void	set_user_last_pong(int fd)
		{
			client_iterator it = _clients.find(fd);
			if (it != _clients.end())
				it->second._last_pong = std::time(NULL);
		}

		void	set_user_state(int fd, int state)
		{
			client_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				it->second._state |= state;
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
		void	add_channel(const std::string& name, int mode)
		{
			_channels.insert(std::make_pair(name, Channel(mode)));
		}

		void	delete_channel(const std::string& channel)
		{
			_channels.erase(channel);
		}

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
			{
				it->second._members.erase(fd);
				if (it->second._members.size() == 0)
					_channels.erase(it);
			}
		}

		void	set_channel_topic(const std::string& channel, const std::string& topic)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._topic = topic;
		}

		void	set_channel_key(const std::string& channel, const std::string& key)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._key = key;
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

		void	set_members_limit(const std::string& channel, unsigned int limit)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._members_limit = limit;
		}

		void	add_ban_mask(const std::string& channel, const std::string& banmask)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._ban_masks.insert(banmask);
		}

		void	add_except_mask(const std::string& channel, const std::string& exceptmask)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._except_masks.insert(exceptmask);
		}

		void	add_invit_mask(const std::string& channel, const std::string& invitmask)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._invit_masks.insert(invitmask);
		}

		void	remove_ban_mask(const std::string& channel, const std::string& banmask)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._ban_masks.erase(banmask);
		}

		void	remove_except_mask(const std::string& channel, const std::string& exceptmask)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._except_masks.erase(exceptmask);
		}

		void	remove_invit_mask(const std::string& channel, const std::string& invit_mask)
		{
			channel_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				it->second._invit_masks.erase(invit_mask);
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

		bool	comp_oper_passwd(const std::string& passwd)
		{
			return (passwd = _oper_passwd);
		}

		bool	get_user_was_ping(int fd)
		{
			client_const_iterator	it = _clients.find(fd);
			if (it != _clients.end())
				return it->second._was_ping;
			else
				throw std::runtime_error("get_user_was_ping: no account registered with this file descriptor");
		}
		
		time_t				get_user_last_move(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			if (it != _clients.end())
				return it->second._last_move;
			else
				throw std::runtime_error("get_user_last_move: no account registered with this file descriptor");
		}

		time_t				get_user_last_pong(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			if (it != _clients.end())
				return it->second._last_pong;
			else
				throw std::runtime_error("get_user_last_pong: no account registered with this file descriptor");
		}

		const std::string&	get_srvname(void) const
		{
			return (_srvname);
		}

		bool	is_connected(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (true);
			return (false);
		}

		int		user_is_unique(std::string &user)
		{
			std::map<int, Client>::iterator  it = _clients.begin();
			std::pair<std::set<std::string>::iterator,bool> ret;
			int	repeat = 0;
			while (it != _clients.end())
			{
				if (it->second._username == user)
					repeat++;
				if (repeat > 1)
					return -1;
				++it;
			}
			if (repeat == 1)
				return (it->first);
			else
				return (0);
		}

			// user lookup

		std::vector<std::string>	list_users(void) const
		{
			std::vector<std::string>	users;
			client_const_iterator		it = _clients.begin();

			for (; it != _clients.end(); it++)
			{
				if ((it->second._mode & INVISIBLE_UFLAG) == 0)
					users.push_back(it->second._nickname);
			}
			return (users);
		}

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
				throw std::runtime_error("get_nickname: no account registered with this file descriptor");
		}

		const std::string&	get_hostname(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._hostname);
			else
				throw std::runtime_error("get_hostname: no account registered with this file descriptor");
		}

		std::set<int>	get_friends(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
			{
				const std::set<std::string>&			channels = it->second._channels;
				std::set<std::string>::const_iterator	itc = channels.begin();
				std::set<int>							friends;
				for (; itc != channels.end(); ++itc)
				{
					std::vector<int>					members_fd = get_members_list_fd(*itc);
					std::vector<int>::const_iterator	itm = members_fd.begin();
					for (; itm != members_fd.end(); ++itm)
						friends.insert(*itm);
				}
				return (friends);
			}
			else
				throw std::runtime_error("get_friends: no account registered with this file descriptor");
		}

		bool	user_nick_match(std::string &nickname, std::string &user)
		{
			int fd = get_user_fd(nickname);
			client_iterator it = _clients.find(fd);
			if (it->second._username == user)
				return true;
			return false;
		}

		bool	host_fd_match(int fd, std::string &host)
		{
			client_iterator it = _clients.find(fd);
			if (it->second._hostname == host)
				return true;
			else if (it->second._hostaddress == host)
				return true;
			else
				return false;
		}

		bool	is_in_channel(int fd, const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				const std::map<int, int>&	members_cpy = it->second._members;
				if (members_cpy.find(fd) != members_cpy.end())
					return (true);
				else
					return (false);
			}
			return (false);
		}

		bool	is_in_channel(const std::string& nick, const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);
			int						fd = get_user_fd(nick);

			if (it != _channels.end())
			{
				const std::map<int, int>&	members_cpy = it->second._members;
				if (members_cpy.find(fd) != members_cpy.end())
					return (true);
				else
					return (false);
			}
			return (false);
		}

		std::string	get_user_info(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
			{
				std::string	info;
				info += it->second._nickname;
				info += "!";
				info += it->second._username;
				info += "@";
				info += it->second._hostname;
				return (info);
			}
			else
				throw std::runtime_error("get_user_info: no account registered with this file descriptor");
		}	

		bool	check_user_state(int fd, int state) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._state & state);
			else
				throw std::runtime_error("check_user_status: no account registered with this file descriptor");

		}

		bool	is_registered(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._state == REGISTERED_STATE);
			else
				throw std::runtime_error("is_registered: no account registered with this file descriptor");
		}

		int	get_user_fd(const std::string& nick) const
		{
			strint_const_iterator	it = _nick_to_fd.find(nick);

			if (it != _nick_to_fd.end())
				return (it->second);
			else
				throw std::runtime_error("get_user_fd: no account registered with this file descriptor");
		}

		const std::set<std::string>&	get_user_channel_list(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._channels);
			else
				throw std::runtime_error("get_user_channel_list: no account registered with this file descriptor");
		}

		// check if flag 'flag' is present in the user mode attribute
		bool	check_user_flags(int fd, int flags) const
		{
			client_const_iterator	it = _clients.find(fd);

			if (it != _clients.end())
				return (it->second._mode & flags);
			else
				throw std::runtime_error("check_user_flags: no account registered with this file descriptor");
		}

		std::string	get_user_flags_str(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			std::string				flags("+");

			if (it != _clients.end())
			{
				if ((it->second._mode & AWAY_UFLAG) != 0)
					flags += "a";		
				if ((it->second._mode & INVISIBLE_UFLAG) != 0)
					flags += "i";
				if ((it->second._mode & WALLOPS_UFLAG) != 0)
					flags += "w";
				if ((it->second._mode & RESTRICTED_UFLAG) != 0)
					flags += "r";
				if ((it->second._mode & OPER_UFLAG) != 0)
					flags += "o";
				if ((it->second._mode & LOCAL_OPER_UFLAG) != 0)
					flags += "O";
				if ((it->second._mode & SRV_NOTICE_UFLAG) != 0)
					flags += "s";
			}
			return (flags);
		}

		int	user_channel_count(int fd) const
		{
			client_const_iterator	it = _clients.find(fd);
			
			if (it != _clients.end())
				return (it->second._channels.size());
			else
				throw std::runtime_error("get_user_flags_str: no account registered with this file descriptor");
		}

			// channel lookup

		std::vector<std::string>	list_visible_channels(int fd) const
		{
			std::vector<std::string>	channels;
			channel_const_iterator		itc = _channels.begin();

			for (; itc != _channels.end(); itc++)
			{
				if ((itc->second._mode & SECRET_CFLAG) == 0)
					channels.push_back(itc->first);
				else if (itc->second._members.find(fd) != itc->second._members.end())
					channels.push_back(itc->first);
			}
			return (channels);
		}

		bool	channel_exists(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (true);
			return (false);
		}

		int	channel_members_count(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._members.size());
			else
				throw std::runtime_error("channel_members_count: channel name does not exist");
		}

		int	channel_visible_members_count(const std::string& channel) const
		{
			channel_const_iterator	itc = _channels.find(channel);
			int						count = 0;

			if (itc != _channels.end())
			{
				std::map<int, int>::const_iterator	itm = itc->second._members.begin();
				for (; itm != itc->second._members.end(); itm++)
				{
					if (!check_user_flags(itm->first, INVISIBLE_UFLAG))
						count++;
				}
				return (count);
			}
			else
				throw std::runtime_error("channel_members_count: channel name does not exist");
		}

		bool	channel_is_empty(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._members.size() == 0);
			return (false);
		}

		bool	channel_is_full(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				if ((it->second._mode & USER_LIMIT_CFLAG) != 0 && it->second._members.size() >= it->second._members_limit)
					return (true);
			}
			return (false);
		}

		bool	channel_is_visible(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end() && !check_channel_flags(channel, SECRET_CFLAG))
				return (true);
			return (false);
		}

		const std::string&	get_channel_topic(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._topic);
			else
				throw std::runtime_error("get_channel_topic: channel name does not exist");
		}

		const std::string&	get_channel_key(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._key);
			else
				throw std::runtime_error("get_channel_key: channel name does not exist");
		}

		const std::set<std::string>&	get_ban_masks(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._ban_masks);
			else
				throw std::runtime_error("get_ban_mask: channel name does not exist");
		}

		const std::set<std::string>&	get_except_masks(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._except_masks);
			else
				throw std::runtime_error("get_except_mask: channel name does not exist");
		}

		const std::set<std::string>&	get_invit_masks(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._invit_masks);
			else
				throw std::runtime_error("get_invit_mask: channel name does not exist");
		}

		bool	match_ban_masks(int fd, const std::string& channel) const
		{
			channel_const_iterator	itchan = _channels.find(channel);
			std::string				uinfo = get_user_info(fd);

			if (itchan == _channels.end())
				return (false);
			std::set<std::string>::const_iterator	itmask = itchan->second._ban_masks.begin();
			for (; itmask != itchan->second._ban_masks.end(); itmask++)
			{
				if (match_mask(uinfo, *itmask))
					return (true);
			}
			return (false);
		}

		bool	match_except_masks(int fd, const std::string& channel) const
		{
			channel_const_iterator	itchan = _channels.find(channel);
			std::string				uinfo = get_user_info(fd);

			if (itchan == _channels.end())
				return (false);
			std::set<std::string>::const_iterator	itmask = itchan->second._except_masks.begin();
			for (; itmask != itchan->second._except_masks.end(); itmask++)
			{
				if (match_mask(uinfo, *itmask))
					return (true);
			}
			return (false);
		}

		bool	match_invit_masks(int fd, const std::string& channel) const
		{
			channel_const_iterator	itchan = _channels.find(channel);
			std::string				uinfo = get_user_info(fd);

			if (itchan == _channels.end())
				return (false);
			std::set<std::string>::const_iterator	itmask = itchan->second._invit_masks.begin();
			for (; itmask != itchan->second._invit_masks.end(); itmask++)
			{
				if (match_mask(uinfo, *itmask))
					return (true);
			}
			return (false);
		}

		bool	is_banned(int fd, const std::string& channel) const
		{
			channel_const_iterator	itchan = _channels.find(channel);
			std::string				uinfo = get_user_info(fd);

			if (itchan == _channels.end())
				return (false);
			const std::set<std::string>&	ban_masks = itchan->second._ban_masks;
			const std::set<std::string>&	except_masks = itchan->second._except_masks;
			std::set<std::string>::const_iterator		itmask = ban_masks.begin();
			for (; itmask != ban_masks.end(); itmask++)
			{
				if (match_mask(uinfo, *itmask))
				{
					for (itmask = except_masks.begin(); itmask != except_masks.end(); itmask++)
					{
						if (match_mask(uinfo, *itmask))
							return (false);
					}
					return (true);
				}
			}
			return (false);
		}

		bool	check_channel_flags(const std::string& channel, int flags) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
				return (it->second._mode & flags);
			else
				throw std::runtime_error("check_channel_flags: channel name does not exist");
		}

		bool	check_member_status(const std::string& channel, int fd, int status) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				std::map<int, int>::const_iterator	itm = it->second._members.find(fd);
				if (itm != it->second._members.end())
					return (itm->second & status);
			}
			return (false);
		}

		std::vector<int>	get_members_list_fd(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				std::vector<int>	members;
				std::map<int, int>::const_iterator	itm = it->second._members.begin();
				for (; itm != it->second._members.end(); itm++)
					members.push_back(itm->first);
				return (members);
			}
			else
				throw std::runtime_error("get_members_list_fd: channel name does not exist");
		}

		std::vector<std::string>	get_members_list_str(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				std::vector<std::string>	members;
				std::map<int, int>::const_iterator	itm = it->second._members.begin();
				for (; itm != it->second._members.end(); itm++)
					members.push_back(get_nickname(itm->first));
				return (members);
			}
			else
				throw std::runtime_error("get_members_list_str: channel name does not exist");
		}

		bool	channel_authenticate(const std::string& channel, const std::string& key) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				if ((it->second._mode & KEY_CFLAG) != 0)
					return (it->second._key == key);
				return (true);
			}
			return (false);
		}

		std::string	get_channel_flags_str(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);
			std::string				flags("+");

			if (it != _channels.end())
			{
				int	mode = it->second._mode;
				if (mode & ANON_CFLAG)
					flags += "a";
				if (mode & INVITE_ONLY_CFLAG)
					flags += "i";
				if (mode & MODERATED_CFLAG)
					flags += "m";
				if (mode & NO_MESSAGES_CFLAG)
					flags += "n";
				if (mode & QUIET_CFLAG)
					flags += "q";
				if (mode & PRIVATE_CFLAG)
					flags += "p";
				if (mode & SECRET_CFLAG)
					flags += "s";
				if (mode & SRV_REOP_CFLAG)
					flags += "r";
				if (mode & TOPIC_OPER_CFLAG)
					flags += "t";
				if (mode & KEY_CFLAG)
					flags += "k";
				if (mode & USER_LIMIT_CFLAG)
					flags += "l";
				if (mode & BAN_MASK_CFLAG)
					flags += "b";
				if (mode & EXCEPT_MASK_CFLAG)
					flags += "e";
				if (mode & INVIT_MASK_CFLAG)
					flags += "I";
			}
			return (flags);
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
				print_user_channels(it->second._channels);
			}
		}

		void	print_user_channels(const std::set<std::string>& channels) const
		{
			std::set<std::string>::const_iterator	it;
			for (it = channels.begin(); it != channels.end(); it++)
				std::cout << *it << std::endl;
		}

		void	print_channel(const std::string& channel) const
		{
			channel_const_iterator	it = _channels.find(channel);

			if (it != _channels.end())
			{
				std::cout << it->second._topic << std::endl;
				std::vector<int>	members = get_members_list_fd(channel);
				for (std::vector<int>::iterator itv = members.begin(); itv != members.end(); itv++)
					std::cout << *itv << ",";
				std::cout << std::endl;
			}
		}

};

#endif
