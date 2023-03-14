#ifndef _REPLY_HPP_
#define _REPLY_HPP_

#include "headers.hpp"

/*******************************ERRORS***********************************/
void	error_caller(int reply_id, const std::set<int>& dest_fds, const std::vector<std::string>& args);

void	err_unknown_command(int dest_fd, const std::vector<std::string> args);
void	err_need_moreparams(int dest_fd, const std::vector<std::string> args);
void	err_already_registered(int dest_fd, const std::vector<std::string> args);
void	err_not_registered(int dest_fd, const std::vector<std::string> args);
void	err_no_nicknamegive(int dest_fd, const std::vector<std::string> args);
void	err_erroneus_nickname(int dest_fd, const std::vector<std::string> args);
void	err_nickname_inuse(int dest_fd, const std::vector<std::string> args);
void	err_restricted(int dest_fd, const std::vector<std::string> args);
void	err_passwd_mismatch(int dest_fd, const std::vector<std::string> args);
void	err_umode_unknownflag(int dest_fd, const std::vector<std::string> args);
void	err_users_dontmatch(int dest_fd, const std::vector<std::string> args);
void	err_invite_onlychan(int dest_fd, const std::vector<std::string> args);
void	err_channel_isfull(int dest_fd, const std::vector<std::string> args);
void	err_nosuch_channel(int dest_fd, const std::vector<std::string> args);
void	err_toomany_targets(int dest_fd, const std::vector<std::string> args);
void	err_banned_fromchan(int dest_fd, const std::vector<std::string> args);
void	err_badchannel_key(int dest_fd, const std::vector<std::string> args);
void	err_bad_chanmask(int dest_fd, const std::vector<std::string> args);
void	err_toomany_channels(int dest_fd, const std::vector<std::string> args);
void	err_unavail_resource(int dest_fd, const std::vector<std::string> args);
void	err_noton_channel(int dest_fd, const std::vector<std::string> args);
void	err_nochan_modes(int dest_fd, const std::vector<std::string> args);
void	err_usernot_inchannel(int dest_fd, const std::vector<std::string> args);
void	err_keyset(int dest_fd, const std::vector<std::string> args);
void	err_chano_privsneeded(int dest_fd, const std::vector<std::string> args);
void	err_unknown_mode(int dest_fd, const std::vector<std::string> args);
void	err_nosuch_server(int dest_fd, const std::vector<std::string> args);
void	err_nosuch_nick(int dest_fd, const std::vector<std::string> args);
void	err_user_on_channel(int dest_fd, const std::vector<std::string> args);
void	err_norecipient(int dest_fd, const std::vector<std::string> args);
void	err_cannotsend_tochan(int dest_fd, const std::vector<std::string> args);
void	err_wildto_plevel(int dest_fd, const std::vector<std::string> args);
void	err_notext_tosend(int dest_fd, const std::vector<std::string> args);
void	err_noto_plevel(int dest_fd, const std::vector<std::string> args);
void	err_wasno_suchnick(int dest_fd, const std::vector<std::string> args);
void	err_noprivileges(int dest_fd, const std::vector<std::string> args);
void	err_cant_killserver(int dest_fd, const std::vector<std::string> args);
void	err_noorigin(int dest_fd, const std::vector<std::string> args);
void	err_users_disabled(int dest_fd, const std::vector<std::string> args);
void	err_filerror(int dest_fd, const std::vector<std::string> args);

/******************************REPLIES***********************************/

void	reply_caller(int reply_id, const std::set<int> dest_fds, const std::vector<std::string>& args);
void	rpl_welcome_message(int dest_fd, const std::vector<std::string> args);
void	rpl_umodeis(int dest_fd, const std::vector<std::string> args);
void	rpl_topic(int dest_fd, const std::vector<std::string> args);
void	rpl_ban_list(int dest_fd, const std::vector<std::string> args);
void	rpl_except_list(int dest_fd, const std::vector<std::string> args);
void	rpl_channel_modeis(int dest_fd, const std::vector<std::string> args);
void	rpl_invite_list(int dest_fd, const std::vector<std::string> args);
void	rpl_uniqops(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_banlist(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_exceptlist(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_invitelist(int dest_fd, const std::vector<std::string> args);
void	rpl_notopic(int dest_fd, const std::vector<std::string> args);
void	rpl_nam_reply(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_names(int dest_fd, const std::vector<std::string> args);
void	rpl_list(int dest_fd, const std::vector<std::string> args);
void	rpl_listend(int dest_fd, const std::vector<std::string> args);
void	rpl_inviting(int dest_fd, const std::vector<std::string> args);
void	rpl_away(int dest_fd, const std::vector<std::string> args);
void	rpl_whoreply(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_who(int dest_fd, const std::vector<std::string> args);
void	rpl_whois_user(int dest_fd, const std::vector<std::string> args);
void	rpl_whois_channel(int dest_fd, const std::vector<std::string> args);
void	rpl_whois_idle(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_whois(int dest_fd, const std::vector<std::string> args);
void	rpl_whois_server(int dest_fd, const std::vector<std::string> args);
void	rpl_whois_server(int dest_fd, const std::vector<std::string> args);
void	rpl_whois_operator(int dest_fd, const std::vector<std::string> args);
void	rpl_whowas_user(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_whowas(int dest_fd, const std::vector<std::string> args);
void	rpl_users_start(int dest_fd, const std::vector<std::string> args);
void	rpl_nousers(int dest_fd, const std::vector<std::string> args);
void	rpl_users(int dest_fd, const std::vector<std::string> args);
void	rpl_endof_users(int dest_fd, const std::vector<std::string> args);

void	rpl_youreoper(int dest_fd, const std::vector<std::string> args);

void	join_reply(int dest_fd, const std::vector<std::string>& args);
void	part_reply(int dest_fd, const std::vector<std::string>& args);
void	kick_reply(int dest_fd, const std::vector<std::string>& args);

void	error_quit(int dest_fd, const std::vector<std::string>& args);
void	error_user(int dest_fd, const std::vector<std::string>& args);
void	error_host(int dest_fd, const std::vector<std::string>& args);
void	error_host_notmatch(int dest_fd, const std::vector<std::string>& args);

#endif
