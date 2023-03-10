#ifndef _HEADERS_HPP_
#define _HEADERS_HPP_

#include <netdb.h>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <ctime>

#ifndef DEBUG
#define DEBUG 0
#endif

#define	TIMEOUT 5000
#define PING_TIME 30
#define DISCONNECT_TIME 60

// REPLIES
#define RPL_WELCOME 1
#define RPL_UMODEIS 221
#define RPL_TOPIC 332
#define RPL_BANLIST 367
#define RPL_EXCEPTLIST 348
#define RPL_CHANNELMODEIS 324
#define RPL_INVITELIST 346
#define RPL_UNIQOPIS 325
#define RPL_ENDOFBANLIST 368
#define RPL_ENDOFEXCEPTLIST 349
#define RPL_ENDOFINVITELIST 347
#define RPL_NOTOPIC 331
#define RPL_NAMREPLY 353
#define RPL_ENDOFNAMES 366
#define RPL_LIST 322
#define RPL_LISTEND 323
#define RPL_INVITING 341
#define RPL_AWAY 301
#define RPL_WHOREPLY 352
#define RPL_ENDOFWHO 315
#define RPL_WHOISUSER 311
#define RPL_WHOISCHANNELS 319
#define RPL_WHOISIDLE 317
#define RPL_ENDOFWHOIS 318
#define RPL_WHOISSERVER 312
#define RPL_WHOISOPERATOR 313
#define RPL_WHOWASUSER 314
#define RPL_ENDOFWHOWAS 369
#define RPL_USERSSTART 392
#define RPL_NOUSERS 395
#define RPL_USERS 393
#define RPL_ENDOFUSERS 394


// ERROR REPLIES

#define ERR_UNKNOWNCOMMAND 421
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_RESTRICTED 484
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502
#define	ERR_INVITEONLYCHAN 473
#define ERR_CHANNELISFULL 471
#define ERR_NOSUCHCHANNEL 403
#define ERR_TOOMANYTARGETS 407
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_TOOMANYCHANNELS 405
#define ERR_UNAVAILRESOURCE 437
#define ERR_NOTONCHANNEL 442
#define ERR_NOCHANMODES 477
#define ERR_USERNOTINCHANNEL 441
#define ERR_KEYSET 467
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_UNKNOWNMODE 472
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHNICK 401
#define ERR_USERONCHANNEL 443
#define ERR_NORECIPIENT 411
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_WILDTOPLEVEL 414
#define ERR_NOTEXTTOSEND 412
#define ERR_NOTOPLEVEL 413
#define ERR_WASNOSUCHNICK 406
#define ERR_NOPRIVILEGES 481
#define ERR_CANTKILLSERVER 483
#define ERR_NOORIGIN 409
#define ERR_USERSDISABLED 446
#define ERR_FILEERROR 424

#define ERROR -1

#define BUFSIZE 512

#define MAX_CHAN_COUNT 10

typedef std::vector<struct pollfd>::iterator pfd_iter;

#endif
