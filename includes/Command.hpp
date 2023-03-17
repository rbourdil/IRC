#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>
#include <ctime>

#include "Data.hpp"
#include "cmd_structs.hpp"
#include "headers.hpp"
#include "parser.hpp"

struct Scanner {
	std::string::iterator _start;
	std::string::iterator _current;
	
	Scanner() : _start(), _current() {}
};

class	Command {

	typedef void (Command::*FuncPtr)(int, const std::vector<std::string>&);

	public:

		Command(Data* data);

		void	execute_cmd(int fd, const irc_cmd& cmd);

	private:
		
		Data*							_data;
		std::map<std::string, FuncPtr>	_cmd_map;
		std::vector<std::string>		_args;

		// user commands
		void	pass(int fd, const std::vector<std::string>& params);
		void	nick(int fd, const std::vector<std::string>& params);
		void	user(int fd, const std::vector<std::string>& params);
		void	oper(int fd, const std::vector<std::string>& params);
		void	user_mode(int fd, const std::vector<std::string>& params);

		// channel commands
		void	join(int fd, std::string channel, std::string key);
		void	topic(int fd, const std::vector<std::string>& params);
		void	names(int fd, const std::vector<std::string>& params);
		void	channel_mode(int fd, const std::vector<std::string>& params);
		void	part(int fd, const std::string& channel, const std::string& message);
		void	pong(int fd, const std::vector<std::string>& params);
		void	privmsg(int fd, const std::vector<std::string>& params);
		void	list(int fd, const std::vector<std::string>& params);
		void	notice(int fd, const std::vector<std::string>& params);
		void	kick(int fd, const std::string& channel, const std::string& user, const std::string& comment);
		void	invite(int fd, const std::vector<std::string>& params);
		void	whois(int fd, const std::vector<std::string>& params);
		void	cap(int fd, const std::vector<std::string>& params);
		void	ping(int fd, const std::vector<std::string>& params);
		void	die(int fd, const std::vector<std::string>& params);

		// helper functions
		void	quit_dispatch(int fd, const std::vector<std::string>& params);
		void	join_dispatch(int fd, const std::vector<std::string>& params);
		void	part_dispatch(int fd, const std::vector<std::string>& params);
		void	mode_dispatch(int fd, const std::vector<std::string>& params);
		void	kick_dispatch(int fd, const std::vector<std::string>& params);
		void	send_to_chan(int fd, std::string &channel, std::string &message);
		void	send_to_nick(int fd, std::string &nick, std::string &message);
		int		parse_target(int fd, std::string &target);
		int		parse_target_notice(std::string &target);
		void	send_to_chan_notice(int fd, std::string &channel, std::string &message);
		void	send_to_nick_notice(int fd, std::string &nick, std::string &message);
		bool	is_valid_host(std::string &host);
		
		std::string	channel_mode_str(const std::string& channel);
		std::string	user_mode_str(int fd);

		// err and reply functions

		void	err_unknown_command(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 421 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Unknown command\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		//461
		void	err_need_moreparams(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 461 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Not enough parameters\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		//462
		void	err_already_registered(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 462 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":Unauthorized command (already registered)\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		//451
		void	err_not_registered(int dest_fd, const std::vector<std::string> args)
		{
				std::string	prefix = ":" + args[0] + " 451 " + _data->get_nickname(dest_fd) + " ";
				std::string err_message = prefix + ":You have not registered\r\n";
				_data->write_outbuff(dest_fd, err_message);		
		}

		//431
		void	err_no_nicknamegive(int dest_fd, const std::vector<std::string> args)
		{
				std::string	prefix = ":" + args[0] + " 431 " + _data->get_nickname(dest_fd) + " ";
				std::string err_message = prefix + ":No nickname given\r\n";
				_data->write_outbuff(dest_fd, err_message);		
		}

		// 432 HERE I NEED TO CHECK IF HE IS STORING THE ERRONEOUS NICKNAME
		void	err_erroneus_nickname(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 432 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Erroneus nickname\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 433
		void	err_nickname_inuse(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 433 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + "* " + args[1] + " :Nickname is already in use\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 484
		void	err_restricted(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 484 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":Your connection is restricted!\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 464
		void	err_passwd_mismatch(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 464 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + "* " + ":Password incorrect\r\n";
			_data->write_outbuff(dest_fd, err_message);		
		}
		// 501
		void	err_umode_unknownflag(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 501 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":Unknown MODE flag\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		// 502
		void	err_users_dontmatch(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 502 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":Cannot change mode for other users\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 473
		void	err_invite_onlychan(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 473 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Cannot join channel (+i)\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 471
		void	err_channel_isfull(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 471 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Cannot join channel (+l)\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 403
		void	err_nosuch_channel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 403 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :No such channel\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		// 407
		void	err_toomany_targets(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 407 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :407 recipients. " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		// 474
		void	err_banned_fromchan(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 474 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Cannot join channel (+b)\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		// 475
		void	err_badchannel_key(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 475 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Cannot join channel (+k)\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//476
		void	err_bad_chanmask(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 476 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Bad Channel Mask\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//405
		void	err_toomany_channels(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 405 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :You have joined too many channels\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//437
		void	err_unavail_resource(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 437 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Nick/channel is temporarily unavailable\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//442
		void	err_noton_channel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 442 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :You're not on that channel\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//477
		void	err_nochan_modes(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 477 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Channel doesn't support modes\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//441
		void	err_usernot_inchannel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 441 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + " :They aren't on that channel\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//467
		void	err_keyset(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 467 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Channel key already set\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//482
		void	err_chano_privsneeded(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 482 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :You're not channel operator\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//472
		void	err_unknown_mode(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 472 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :is unknown mode char to me for " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//402
		void	err_nosuch_server(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 402 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[0] + " :No such server\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//401
		void	err_nosuch_nick(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 401 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :No such nick\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//443
		void	err_user_on_channel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 443 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + " :is already on channel\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//411
		void	err_norecipient(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 411 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":No recipient given (" + args[1] + ")\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//404
		void	err_cannotsend_tochan(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 404 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Cannot send to channel\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//414
		void	err_wildto_plevel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 414 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Wildcard in toplevel domain\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//412
		void	err_notext_tosend(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 412 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":No text to send\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//413
		void	err_noto_plevel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 413 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :No toplevel domain specified\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//406
		void	err_wasno_suchnick(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 406 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :There was no such nickname\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//481
		void	err_noprivileges(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 481 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":Permission Denied- You're not an IRC operator\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//483
		void	err_cant_killserver(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 483 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":You can't kill a server!\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//409
		void	err_noorigin(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 409 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":No origin specified\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//446
		void	err_users_disabled(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 406 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":USERS has been disabled\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//424
		void	err_filerror(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 424 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":File error doing " + args[2] + "on" + args[3] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//491
		void	error_no_operhost(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0] + " 491 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":NO O-lines for your host\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}
		//replies


		void	rpl_welcome_message(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 001 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :Welcome to the Internet Relay Network " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 381
		void	rpl_youreoper(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 381 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":You are now an IRC operator\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 221
		void	rpl_umodeis(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 221 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}
		// 332
		void	rpl_topic(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 332 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 367
		void	rpl_ban_list(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 376 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		// 348
		void	rpl_except_list(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 348 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		// 324
		void	rpl_channel_modeis(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 324 " + _data->get_nickname(dest_fd) + " ";
			std::string	err_message = prefix + _data->get_nickname(dest_fd) + " " + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);		
		}

		// 346
		void	rpl_invite_list(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 346 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);		
		}

		// 325
		void	rpl_uniqops(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 325 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		// 368
		void	rpl_endof_banlist(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 368 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of channel ban list\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		// 349
		void	rpl_endof_exceptlist(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 349 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of channel exception list\r\n";
			_data->write_outbuff(dest_fd, err_message);			
		}

		// 347
		void	rpl_endof_invitelist(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 347 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of channel invite list\r\n";
			_data->write_outbuff(dest_fd, err_message);		
		}

		// 331
		void	rpl_notopic(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 331 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :No topic is set\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		// 353
		void	rpl_nam_reply(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 353 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :";
			if (args.size() > 2)
			{
				err_message = err_message + args[2];
				for (size_t i = 3; i < args.size(); i++)
					err_message = err_message + " " + args[i];
			}
			err_message = err_message +" \r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 366
		void	rpl_endof_names(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 366 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of NAMES list\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 322
		void	rpl_list(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 322 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + " :" + args[3] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);	
		}

		// 323
		void	rpl_listend(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 323 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":End of LIST\r\n";
			_data->write_outbuff(dest_fd, err_message);			
		}

		// 341
		void	rpl_inviting(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 341 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);		
		}

		// 301
		void	rpl_away(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 301 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);				
		}

		// 352
		void	rpl_whoreply(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 352 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + " " + args[3] + " " + args[0] + " " + args[5];
			err_message = err_message + "H@+: " + args[6] +  " " + args[7] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 315
		void	rpl_endof_who(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 315 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of WHO list\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 311
		void	rpl_whois_user(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 311 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + args[2] + args[3] + args[4] + " * :" + args[5] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 319
		void	rpl_whois_channel(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 319 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + ":";
			if (args.size() > 2)
			{
				err_message = err_message + args[2];
				for (size_t i = 3; i < args.size() ; i++)
					err_message = err_message + args[1];
			}
			err_message = err_message + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 317
		void	rpl_whois_idle(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 317 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + " :seconds idle\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 318
		void	rpl_endof_whois(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 312 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of WHOIS list\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 312
		void	rpl_whois_server(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 312 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[0] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 313
		void	rpl_whois_operator(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 313 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :is an IRC operator\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 314
		void	rpl_whowas_user(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 314 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " " + args[2] + " " + args[3] + "* :" + args[4] +"\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 369
		void	rpl_endof_whowas(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 369 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + args[1] + " :End of WHOWAS\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 392
		void	rpl_users_start(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 392 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":UserID   Terminal  Host\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 395
		void	rpl_nousers(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 395 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":Nobody logged in\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 393
		void	rpl_users(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 393 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":" + args[1] + " " + args[2] + " " + args[5] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// 394
		void	rpl_endof_users(int dest_fd, const std::vector<std::string> args)
		{
			std::string	prefix = ":" + args[0] + " 394 " + _data->get_nickname(dest_fd) + " ";
			std::string err_message = prefix + ":End of users\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		// command style replies

		void	join_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " JOIN " + args[1] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	part_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " PART " + args[1] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	topic_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " TOPIC " + args[1] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	kick_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " KICK " + args[1] + " " + args[2] + " :" + args[3] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	privmsg_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " PRIVMSG " + args[1] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	notice_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " NOTICE " + args[1] + " :" + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	pong_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " PONG " + args[1] + " " + args[2] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	quit_reply(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " QUIT :Quit: " + args[1] + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}


		// OTHER ERRORS

		void	error_quit(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " ERROR : Closing link: " + args[1];
			if (args.size() == 3)
			{
				std::string	suffix = " (QUIT: " + args[2] + ")";
				err_message += suffix;
			}
			err_message = err_message + "\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}


		void	error_user(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " " + args[1] + " ERROR : user does not exist\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	error_host(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " " + args[1] + " ERROR : invalid hostname/hostaddress\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}

		void	error_host_notmatch(int dest_fd, const std::vector<std::string>& args)
		{
			std::string	prefix = ":" + args[0];
			std::string	err_message = prefix + " " + args[1] + " ERROR : user does not have such hostname/hostaddress\r\n";
			_data->write_outbuff(dest_fd, err_message);
		}


					
};

std::string	get_id(void);

#endif
