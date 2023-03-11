/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcamaren <pcamaren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 16:59:07 by pcamaren          #+#    #+#             */
/*   Updated: 2023/03/11 12:38:11 by rbourdil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Reply.hpp"
void	error_caller(int reply_id, const std::set<int>& dest_fds, const std::vector<std::string>& args)
{
	std::set<int>::const_iterator it = dest_fds.begin();
	switch (reply_id)
	{
	case ERR_UNKNOWNCOMMAND:
		for (; it != dest_fds.end(); ++it)
			err_unknown_command(*it, args);
		break;
	case ERR_NONICKNAMEGIVEN:
		for (; it != dest_fds.end(); ++it)
			err_no_nicknamegive(*it, args);
		break;
	case ERR_ERRONEUSNICKNAME:
		for (; it != dest_fds.end(); ++it)
			err_erroneus_nickname(*it, args);
		break;
	case ERR_NICKNAMEINUSE:
		for (; it != dest_fds.end(); ++it)
			err_nickname_inuse(*it, args);
		break;
	case ERR_NOTREGISTERED:
		for (; it != dest_fds.end(); ++it)
			err_not_registered(*it, args);
		break;
	case ERR_NEEDMOREPARAMS:
		for (; it != dest_fds.end(); ++it)
			err_need_moreparams(*it, args);
		break;
	case ERR_ALREADYREGISTERED: 
		for (; it != dest_fds.end(); ++it)
			err_already_registered(*it, args);
		break;
	case ERR_PASSWDMISMATCH: 
		for (; it != dest_fds.end(); ++it)
			err_passwd_mismatch(*it, args);
		break;
	case ERR_RESTRICTED:
		for (; it != dest_fds.end(); ++it)
			err_restricted(*it, args);
		break;
	case ERR_USERSDONTMATCH:
		for (; it != dest_fds.end(); ++it)
			err_users_dontmatch(*it, args);
	case ERR_UMODEUNKNOWNFLAG:
		for (; it != dest_fds.end(); ++it)
			err_umode_unknownflag(*it, args);
	case ERR_INVITEONLYCHAN:
		for (; it != dest_fds.end(); ++it)
			err_invite_onlychan(*it, args);
	case ERR_CHANNELISFULL:
		for (; it != dest_fds.end(); ++it)
			err_channel_isfull(*it, args);
	case ERR_NOSUCHCHANNEL:
		for (; it != dest_fds.end(); ++it)
			err_nosuch_channel(*it, args);
	case ERR_TOOMANYTARGETS:
		for (; it != dest_fds.end(); ++it)
			err_toomany_targets(*it, args);
	case ERR_BANNEDFROMCHAN:
		for (; it != dest_fds.end(); ++it)
			err_banned_fromchan(*it, args);
	case ERR_BADCHANNELKEY:
		for (; it != dest_fds.end(); ++it)
			err_badchannel_key(*it, args);
	case ERR_BADCHANMASK:
		for (; it != dest_fds.end(); ++it)
			err_bad_chanmask(*it, args);
	case ERR_TOOMANYCHANNELS:
		for (; it != dest_fds.end(); ++it)
			err_toomany_channels(*it, args);
	case ERR_UNAVAILRESOURCE:
		for (; it != dest_fds.end(); ++it)
			err_unavail_resource(*it, args);
	case ERR_NOTONCHANNEL:
		for (; it != dest_fds.end(); ++it)
			err_noton_channel(*it, args);
	case ERR_NOCHANMODES:
		for (; it != dest_fds.end(); ++it)
			err_nochan_modes(*it, args);
	case ERR_USERNOTINCHANNEL:
		for (; it != dest_fds.end(); ++it)
			err_usernot_inchannel(*it, args);
	case ERR_KEYSET:
		for (; it != dest_fds.end(); ++it)
			err_keyset(*it, args);
	case ERR_CHANOPRIVSNEEDED:
		for (; it != dest_fds.end(); ++it)
			err_chano_privsneeded(*it, args);
	case ERR_UNKNOWNMODE:
		for (; it != dest_fds.end(); ++it)
			err_unknown_mode(*it, args);
	case ERR_NOSUCHSERVER:
		for (; it != dest_fds.end(); ++it)
			err_nosuch_server(*it, args);
	case ERR_NOSUCHNICK:
		for (; it != dest_fds.end(); ++it)
			err_nosuch_nick(*it, args);
	case ERR_USERONCHANNEL:
		for (; it != dest_fds.end(); ++it)
			err_user_on_channel(*it, args);
	case ERR_NORECIPIENT:
		for (; it != dest_fds.end(); ++it)
			err_norecipient(*it, args);
	case ERR_CANNOTSENDTOCHAN:
		for (; it != dest_fds.end(); ++it)
			err_cannotsend_tochan(*it, args);
	case ERR_WILDTOPLEVEL:
		for (; it != dest_fds.end(); ++it)
			err_wildto_plevel(*it, args);
	case ERR_NOTEXTTOSEND:
		for (; it != dest_fds.end(); ++it)
			err_notext_tosend(*it, args);
	case ERR_NOTOPLEVEL:
		for (; it != dest_fds.end(); ++it)
			err_noto_plevel(*it, args);
	case ERR_WASNOSUCHNICK:
		for (; it != dest_fds.end(); ++it)
			err_wasno_suchnick(*it, args);
	case ERR_NOPRIVILEGES:
		for (; it != dest_fds.end(); ++it)
			err_noprivileges(*it, args);
	case ERR_CANTKILLSERVER:
		for (; it != dest_fds.end(); ++it)
			err_cant_killserver(*it, args);
	case ERR_NOORIGIN:
		for (; it != dest_fds.end(); ++it)
			err_noorigin(*it, args);
	case ERR_USERSDISABLED:
		for (; it != dest_fds.end(); ++it)
			err_users_disabled(*it, args);
	case ERR_FILEERROR:
		for (; it != dest_fds.end(); ++it)
			err_filerror(*it, args);
	case RPL_WELCOME:
		for (; it != dest_fds.end(); ++it)
			rpl_welcome_message(*it, args);
		break;
	case RPL_UMODEIS:
		for (; it != dest_fds.end(); ++it)
			rpl_umodeis(*it, args);
		break;
	case RPL_BANLIST:
		for (; it != dest_fds.end(); ++it)
			rpl_ban_list(*it, args);
		break;
	case RPL_EXCEPTLIST:
		for (; it != dest_fds.end(); ++it)
			rpl_except_list(*it, args);
		break;
	case RPL_CHANNELMODEIS:
		for (; it != dest_fds.end(); ++it)
			rpl_channel_modeis(*it, args);
		break;
	case RPL_INVITELIST:
		for (; it != dest_fds.end(); ++it)
			rpl_invite_list(*it, args);
		break;
	case RPL_UNIQOPIS:
		for (; it != dest_fds.end(); ++it)
			rpl_uniqops(*it, args);
		break;
	case RPL_ENDOFBANLIST:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_banlist(*it, args);
		break;
	case RPL_ENDOFEXCEPTLIST:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_exceptlist(*it, args);
		break;
	case RPL_ENDOFINVITELIST:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_invitelist(*it, args);
		break;
	case RPL_NOTOPIC:
		for (; it != dest_fds.end(); ++it)
			rpl_notopic(*it, args);
		break;
	case RPL_NAMREPLY:
		for (; it != dest_fds.end(); ++it)
			rpl_nam_reply(*it, args);
		break;
	case RPL_ENDOFNAMES:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_names(*it, args);
		break;
	case RPL_LIST:
		for (; it != dest_fds.end(); ++it)
			rpl_list(*it, args);
		break;
	case RPL_LISTEND:
		for (; it != dest_fds.end(); ++it)
			rpl_listend(*it, args);
		break;
	case RPL_INVITING:
		for (; it != dest_fds.end(); ++it)
			rpl_inviting(*it, args);
		break;
	case RPL_AWAY:
		for (; it != dest_fds.end(); ++it)
			rpl_away(*it, args);
		break;
	case RPL_WHOREPLY:
		for (; it != dest_fds.end(); ++it)
			rpl_whoreply(*it, args);
		break;
	case RPL_ENDOFWHO:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_who(*it, args);
		break;
	case RPL_WHOISUSER:
		for (; it != dest_fds.end(); ++it)
			rpl_whois_user(*it, args);
		break;
	case RPL_WHOISCHANNELS:
		for (; it != dest_fds.end(); ++it)
			rpl_whois_channel(*it, args);
		break;
	case RPL_WHOISIDLE:
		for (; it != dest_fds.end(); ++it)
			rpl_whois_idle(*it, args);
		break;
	case RPL_ENDOFWHOIS:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_whois(*it, args);
		break;
	case RPL_WHOISSERVER:
		for (; it != dest_fds.end(); ++it)
			rpl_whois_server(*it, args);
		break;
	case RPL_WHOISOPERATOR:
		for (; it != dest_fds.end(); ++it)
			rpl_whois_operator(*it, args);
		break;
	case RPL_WHOWASUSER:
		for (; it != dest_fds.end(); ++it)
			rpl_whowas_user(*it, args);
		break;
	case RPL_ENDOFWHOWAS:
		for (; it != dest_fds.end(); ++it)
			rpl_umodeis(*it, args);
		break;
	case RPL_USERSSTART:
		for (; it != dest_fds.end(); ++it)
			rpl_users_start(*it, args);
		break;
	case RPL_NOUSERS:
		for (; it != dest_fds.end(); ++it)
			rpl_nousers(*it, args);
		break;
	case RPL_USERS:
		for (; it != dest_fds.end(); ++it)
			rpl_users(*it, args);
		break;
	case RPL_ENDOFUSERS:
		for (; it != dest_fds.end(); ++it)
			rpl_endof_users(*it, args);
		break;
	default:
		break;
	}
}


//pass

//    ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
// Done
//nick

//    ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//    ERR_NICKNAMEINUSE               
//    ERR_RESTRICTED
// Done
//USER

// ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
// Done

// OPER
//            ERR_NEEDMOREPARAMS              RPL_YOUREOPER
//            ERR_NOOPERHOST                  ERR_PASSWDMISMATCH

// USER MODE 

// ERR_NEEDMOREPARAMS              ERR_USERSDONTMATCH
// ERR_UMODEUNKNOWNFLAG            RPL_UMODEIS

//JOIN

//    ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//    ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//    ERR_CHANNELISFULL               ERR_BADCHANMASK
//    ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//    ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
//    RPL_TOPIC


//PART
//    ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//    ERR_NOTONCHANNEL

// CHANNEL MODE
//    ERR_NEEDMOREPARAMS              ERR_KEYSET
//    ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
//    ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
//    RPL_CHANNELMODEIS
//    RPL_BANLIST                     RPL_ENDOFBANLIST
//    RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
//    RPL_INVITELIST                  RPL_ENDOFINVITELIST
//    RPL_UNIQOPIS

// TOPIC
// NAMES
// LIST

// INVITE
// ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
// ERR_NOTONCHANNEL                ERR_USERONCHANNEL
// ERR_CHANOPRIVSNEEDED
// RPL_INVITING                    RPL_AWAY

// KICK
// LERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
// LERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
// LERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

// PRIVMSG

// ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
// ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
// ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
// ERR_NOSUCHNICK
// RPL_AWAY

// MOTD?
//	RPL_MOTDSTART                   RPL_MOTD
//	RPL_ENDOFMOTD                   ERR_NOMOTD

// STATS?
//	ERR_NOSUCHSERVER
//	RPL_STATSLINKINFO                RPL_STATSUPTIME
//	RPL_STATSCOMMANDS                RPL_STATSOLINE
//	RPL_ENDOFSTATS


// WHO

//ERR_NOSUCHSERVER
// RPL_WHOREPLY                  RPL_ENDOFWHO

// WHOIS

//    ERR_NOSUCHSERVER              ERR_NONICKNAMEGIVEN
//    RPL_WHOISUSER                 RPL_WHOISCHANNELS
//    RPL_WHOISCHANNELS             RPL_WHOISSERVER
//    RPL_AWAY                      RPL_WHOISOPERATOR
//    RPL_WHOISIDLE                 ERR_NOSUCHNICK
//    RPL_ENDOFWHOIS

//WHOWAS

//	ERR_NONICKNAMEGIVEN           ERR_WASNOSUCHNICK
//	RPL_WHOWASUSER                RPL_WHOISSERVER
//	RPL_ENDOFWHOWAS

// KILL
// ERR_NOPRIVILEGES              ERR_NEEDMOREPARAMS
// ERR_NOSUCHNICK                ERR_CANTKILLSERVER

// PING
// ERR_NOORIGIN                  ERR_NOSUCHSERVER

// PONG
// ERR_NOORIGIN                  ERR_NOSUCHSERVER

// USERS
// ERR_NOSUCHSERVER              ERR_FILEERROR
// RPL_USERSSTART                RPL_USERS
// RPL_NOUSERS                   RPL_ENDOFUSERS
// ERR_USERSDISABLED

// ERRORS

//
void	err_unknown_command(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 421 ";
	std::string err_message = prefix + args[1] + " :Unknown command\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
	// std::string	prefix = ":" + args[0] + " " + args[1] + args[2];
	// std::string err_message = prefix + " " + args[3] + " :Unknown command\n";
	// send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

//461
void	err_need_moreparams(int dest_fd, const std::vector<std::string> args)
{
	// if (args.size() > 3)
	// {
	// 	std::string	prefix = ":" + args[0] + " 461 ";
	// 	std::string err_message = prefix + args[1] + " :Not enough parameters\n";
	// 	send(dest_fd, err_message.c_str(), err_message.size(), 0);
	// }
	// else
	// {
		std::string	prefix = ":" + args[0] + " 461 ";
		std::string err_message = prefix + args[1] + " :Not enough parameters\n";
		send(dest_fd, err_message.c_str(), err_message.size(), 0);
	// }
}

//462
void	err_already_registered(int dest_fd, const std::vector<std::string> args)
{
// 	if (args.size() > 2)
// 	{
// 		std::string	prefix = ":" + args[0] + " " + args[1] + " " + args[2];
// 		std::string err_message = prefix + " :Unauthorized command (already registered)\n";
// 		send(dest_fd, err_message.c_str(), err_message.size(), 0);	
// 	}
// 	else
// 	{
		std::string	prefix = ":" + args[0] + " 462 ";
		std::string err_message = prefix + " :Unauthorized command (already registered)\n";
		send(dest_fd, err_message.c_str(), err_message.size(), 0);	
	// }
	// ":Unauthorized command (alredy registered)"
}

//451
void	err_not_registered(int dest_fd, const std::vector<std::string> args)
{
		std::string	prefix = ":" + args[0] + " 451 ";
		std::string err_message = prefix + ":You have not registered\n";
		send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

//431
void	err_no_nicknamegive(int dest_fd, const std::vector<std::string> args)
{
		std::string	prefix = ":" + args[0] + " 431 ";
		std::string err_message = prefix + ":No nickname given\n";
		send(dest_fd, err_message.c_str(), err_message.size(), 0);		
	// ":No nickname given"
}

// 432 HERE I NEED TO CHECK IF HE IS STORING THE ERRONEOUS NICKNAME
void	err_erroneus_nickname(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 432 ";
	std::string err_message = prefix + args[1] + " :Erroneus nickname\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 433
void	err_nickname_inuse(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 433 ";
	std::string err_message = prefix + ":Nickname is already in use\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 484
void	err_restricted(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 484 ";
	std::string err_message = prefix + ":Your connection is restricted!\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 464
void	err_passwd_mismatch(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 464 ";
	std::string err_message = prefix + ":Password incorrect\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}
// 501
void	err_umode_unknownflag(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 501";
	std::string err_message = prefix + ":Unknown MODE flag\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
// 502
void	err_users_dontmatch(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 502 ";
	std::string err_message = prefix + ":Cannot change mode for other users\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 473
void	err_invite_onlychan(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 473";
	std::string err_message = prefix + args[1] + " :Cannot join channel (+i)\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 471
void	err_channel_isfull(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 471 ";
	std::string err_message = prefix + args[1] + " :Cannot join channel (+l)\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 403
void	err_nosuch_channel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 403 ";
	std::string err_message = prefix + args[1] + " :No such channel\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
// 407
void	err_toomany_targets(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 407 ";
	std::string err_message = prefix + args[1] + " :407 recipients. " + args[3] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
// 474
void	err_banned_fromchan(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 474 ";
	std::string err_message = prefix + args[1] + " :Cannot join channel (+b)\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
// 475
void	err_badchannel_key(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 475 ";
	std::string err_message = prefix + args[1] + " :Cannot join channel (+k)\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//476
void	err_bad_chanmask(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 476 ";
	std::string err_message = prefix + args[1] + " :Bad Channel Mask\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//405
void	err_toomany_channels(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 405 ";
	std::string err_message = prefix + args[1] + " :You have joined too many channels\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//437
void	err_unavail_resource(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 437 ";
	std::string err_message = prefix + args[1] + " :Nick/channel is temporarily unavailable\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//442
void	err_noton_channel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 442 ";
	std::string err_message = prefix + args[1] + " :You're not on that channel\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//477
void	err_nochan_modes(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 477 ";
	std::string err_message = prefix + args[1] + " :Channel doesn't support modes\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//441
void	err_usernot_inchannel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 441 ";
	std::string err_message = prefix + args[1] + " " + args[2] + " :They aren't on that channel\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//467
void	err_keyset(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 467 ";
	std::string err_message = prefix + args[1] + " :Channel key already set\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//482
void	err_chano_privsneeded(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 482 ";
	std::string err_message = prefix + args[1] + " :You're not channel operator\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//472
void	err_unknown_mode(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 472 ";
	std::string err_message = prefix + args[1] + " :is unknown mode char to me for " + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//402
void	err_nosuch_server(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 402 ";
	std::string err_message = prefix + args[0] + " :No such server\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//401
void	err_nosuch_nick(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 401 ";
	std::string err_message = prefix + args[1] + " :No such nick/channel\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//443
void	err_user_on_channel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 443 ";
	std::string err_message = prefix + args[1] + " " + args[2] + " :is already on channel\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//411
void	err_norecipient(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 411 ";
	std::string err_message = prefix + ":No recipient given (" + args[1] + ")\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//404
void	err_cannotsend_tochan(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 404 ";
	std::string err_message = prefix + args[1] + " :Cannot send to channel\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//414
void	err_wildto_plevel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + "  ";
	std::string err_message = prefix + args[1] + " :Wildcard in toplevel domain\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//412
void	err_notext_tosend(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 412 ";
	std::string err_message = prefix + ":No text to send\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//413
void	err_noto_plevel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 413 ";
	std::string err_message = prefix + args[1] + " :No toplevel domain specified\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//406
void	err_wasno_suchnick(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 406 ";
	std::string err_message = prefix + args[1] + " :There was no such nickname\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//481
void	err_noprivileges(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 481 ";
	std::string err_message = prefix + ":Permission Denied- You're not an IRC operator\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//483
void	err_cant_killserver(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 483 ";
	std::string err_message = prefix + ":You can't kill a server!\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//409
void	err_noorigin(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 409 ";
	std::string err_message = prefix + " :No origin specified\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//446
void	err_users_disabled(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 406 ";
	std::string err_message = prefix + " :USERS has been disabled\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//424
void	err_filerror(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 424 ";
	std::string err_message = prefix + " :File error doing " + args[2] + "on" + args[3] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}
//replies


void	rpl_welcome_message(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 001  ";
	std::string err_message = prefix + ":Welcome to the Internet Relay Network " + args[1] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 381
void	rpl_youreoper(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 381 ";
	std::string err_message = prefix + ":You are now an IRC operator\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 221
void	rpl_umodeis(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 221 ";
	std::string err_message = prefix + args[1] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
// 332
void	rpl_topic(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 332 ";
	std::string err_message = prefix + args[1] + " :" + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 367
void	rpl_ban_list(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 376 ";
	std::string err_message = prefix + args[1] + " " + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}

// 348
void	rpl_except_list(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 348 ";
	std::string err_message = prefix + args[1] + " " + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}

// 324
void	rpl_channel_modeis(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 324 ";
	std::string err_message = prefix + args[1] + " " + args[2] +"\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 346
void	rpl_invite_list(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 346 ";
	std::string err_message = prefix + args[1] + " " + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 325
void	rpl_uniqops(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 325 ";
	std::string err_message = prefix + args[1] + " " + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);	
}

// 368
void	rpl_endof_banlist(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 368 ";
	std::string err_message = prefix + args[1] + " :End of channel ban list\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 349
void	rpl_endof_exceptlist(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 349 ";
	std::string err_message = prefix + args[1] + " :End of channel exception list\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);			
}

// 347
void	rpl_endof_invitelist(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 347 ";
	std::string err_message = prefix + args[1] + " :End of channel invite list\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 331
void	rpl_notopic(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 331 ";
	std::string err_message = prefix + args[1] + " :No topic is set\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 353
void	rpl_nam_reply(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 353 ";
	std::string err_message = prefix + args[1] + " :";
	if (args.size() > 2)
	{
		err_message = err_message + args[2];
		for (size_t i = 3; i < args.size(); i++)
			err_message = err_message + " " + args[i];
	}
	err_message = err_message +" \n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 366
void	rpl_endof_names(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 366 ";
	std::string err_message = prefix + args[1] + " :End of NAMES list\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 322
void	rpl_list(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 322 ";
	std::string err_message = prefix + args[1] + " " + args[2] + " :" + args[3] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 323
void	rpl_listend(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 323 ";
	std::string err_message = prefix + ":End of LIST\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);			
}

// 341
void	rpl_inviting(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 341 ";
	std::string err_message = prefix + args[1] + " " + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);		
}

// 301
void	rpl_away(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 301 ";
	std::string err_message = prefix + args[1] + " :" + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);				
}

// 352
void	rpl_whoreply(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 352 ";
	std::string err_message = prefix + args[1] + " " + args[2] + " " + args[3] + " " + args[0] + " " + args[5];
	err_message = err_message + "H@+: " + args[6] +  " " + args[7] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 315
void	rpl_endof_who(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 315 ";
	std::string err_message = prefix + args[1] + " :End of WHO list\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 311
void	rpl_whois_user(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 311 ";
	std::string err_message = prefix + args[1] + args[2] + args[3] + args[4] + " * :" + args[5] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 319
void	rpl_whois_channel(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 319 ";
	std::string err_message = prefix + args[1] + ":";
	if (args.size() > 2)
	{
		err_message = err_message + args[2];
		for (size_t i = 3; i < args.size() ; i++)
			err_message = err_message + args[1];
	}
	err_message = err_message + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 317
void	rpl_whois_idle(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 317 ";
	std::string err_message = prefix + args[1] + " " + args[2] + " :seconds idle\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 318
void	rpl_endof_whois(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 312 ";
	std::string err_message = prefix + args[1] + " :End of WHOIS list\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 312
void	rpl_whois_server(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 312 ";
	std::string err_message = prefix + args[1] + " " + args[0] + " :" + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 313
void	rpl_whois_operator(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 313 ";
	std::string err_message = prefix + args[1] + " :is an IRC operator\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 314
void	rpl_whowas_user(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 314 ";
	std::string err_message = prefix + args[1] + " " + args[2] + " " + args[3] + "* :" + args[4] +"\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 369
void	rpl_endof_whowas(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 369 ";
	std::string err_message = prefix + args[1] + " :End of WHOWAS\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 392
void	rpl_users_start(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 392 ";
	std::string err_message = prefix + ":UserID   Terminal  Host\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 395
void	rpl_nousers(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 395 ";
	std::string err_message = prefix + ":Nobody logged in\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 393
void	rpl_users(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 393 ";
	std::string err_message = prefix + ":" + args[1] + " " + args[2] + " " + args[5] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// 394
void	rpl_endof_users(int dest_fd, const std::vector<std::string> args)
{
	std::string	prefix = ":" + args[0] + " 394 ";
	std::string err_message = prefix + ":End of users\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

// command style replies

void	join_reply(int dest_fd, const std::vector<std::string>& args)
{
	std::string	prefix = ":" + args[0];
	std::string	err_message = prefix + " JOIN " + args[1] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}

void	part_reply(int dest_fd, const std::vector<std::string>& args)
{
	std::string	prefix = ":" + args[0];
	std::string	err_message = prefix + " PART " + args[1] + " :" + args[2] + "\n";
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}


// OTHER ERRORS

void	error_quit(int dest_fd, const std::vector<std::string>& args)
{
	std::string	prefix = ":" + args[0];
	std::string	err_message = prefix + " ERROR: Closing link: " + args[1];
	if (args.size() == 3)
	{
		std::string	suffix = " (QUIT: " + args[2] + ")";
		err_message += suffix;
	}
	send(dest_fd, err_message.c_str(), err_message.size(), 0);
}
