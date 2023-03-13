#include "parser.hpp"

void	parser::get_prefix(void) { match(':'); update_prefix(); match(WORD_TOKEN);
	match(' ');
	get_cmd();
}

void	parser::get_cmd(void)
{
	update_cmd();
	match(WORD_TOKEN);
	get_params();
}

void	parser::get_params(void)
{
	while ((nb_of_params() < 14 && _current == ' ') && !_panic)
	{
		match(' ');
		if (_current == ':')
		{
			match(':');
			get_trailing();
			break ;
		}
		update_params();
		match(WORD_TOKEN);
	} if (nb_of_params() == 14 && _current == ' ') { match(' ');
		if (_current == ':')
			match(':');
		get_trailing(); }
}

void	parser::get_trailing(void)
{
	update_params();
	while ((_current == ' ' || _current == WORD_TOKEN) && !_panic)
	{
		if (_current == ' ')
			match(' ');
		else
			match(WORD_TOKEN);
		if (_current == ' ' || _current == WORD_TOKEN)
			merge_params();
	}
}

void	parser::get_crlf(void)
{
	match('\r');
	match('\n');
}
	
void	parser::parse(void)
{
	if (_current == '\r')
	{
		get_crlf();
		_state = DUMP_CMD;
		return ;
	}
	if (_current == ':')
		get_prefix();
	else
		get_cmd();
	get_crlf();
	if (!_panic)
		_state = VALID_CMD;
}

bool	isspecial(int c)
{
	if ((c >= 0x5b && c <= 0x60) || (c >= 0x7b && c <= 0x7d))
		return (true);
	return (false);
}

bool	valid_nickname(const std::string& nick)
{
	std::string::const_iterator	it = nick.begin();

	if (nick.empty())
		return (false);
	if (!isalpha(*it) && !isspecial(*it))
		return (false);
	it++;
	for (int count = 0; it != nick.end() && count < 8; it++)
	{
		if (!isdigit(*it) && !isalpha(*it) && !isspecial(*it) && *it != '-')
			break ;
	}
	if (it != nick.end())
		return (false);
	return (true);
}

bool	valid_username(const std::string& user)
{
	std::string::const_iterator	it = user.begin();

	if (user.empty())
		return (false);
	for (; it != user.end(); it++)
	{
		if (*it == '@')
			break;
	}
	if (it != user.end())
		return (false);
	return (true);
}

bool	valid_user_mode(const std::string& mode)
{
	std::string::const_iterator	it = mode.begin();

	if (mode.empty())
		return (false);
	if (*it != '+' && *it != '-')
		return (false);
	it++;
	while (*it == 'i' || *it == 'w' || *it == 'o' || *it == 'O' || *it == 'r')
		it++;
	if (it != mode.end())
		return (false);
	return (true);
}

int	valid_channel_mode(const std::string& mode)
{
	std::string::const_iterator	it = mode.begin();

	if (mode.empty())
		return (false);
	if (*it == '+' || *it == '-')
		++it;
	while (*it == 'O' || *it == 'o' || *it == 'v' || *it == 'a' || *it == 'i' || *it == 'm' \
		|| *it == 'n' || *it == 'q' || *it == 'p' || *it == 's' || *it == 'r' || *it == 't' \
		|| *it == 'k' || *it == 'l' || *it == 'b' || *it == 'e' || *it == 'I')
		++it;
	if (it != mode.end())
		return (*it);
	return (0);
}

bool	match_mode_params(const std::vector<std::string>& params)
{
	std::vector<std::string>::const_iterator	itv = params.begin() + 2;
	std::string::const_iterator					its = params[1].begin();
	bool										add = false;

	if (*its == '+' || *its == '-')
	{
		if (*its == '+')
			add = true;
		++its;
	}
	for (; its != params[1].end(); ++its)
	{
		if (*its == 'o' || *its == 'v')
		{
			if (itv == params.end())
				return (false);
			else
				++itv;
		}
		else if (add && (*its == 'k' || *its == 'l' || *its == 'b' || *its == 'e' || *its == 'I'))
		{
			if (itv == params.end())
				return (false);
			else
				++itv;
		}
	}
	return (true);
}

bool	valid_channel(const std::string& channel)
{
	std::string::const_iterator	it = channel.begin();

	if (channel.empty())
		return (false);
	if (*it != '#' && *it != '+' && *it != '&' && *it != '!')
		return (false);
	it++;
	while (it != channel.end() && *it != '\a')
		it++;
	if (((it - channel.begin()) > 50) || it != channel.end())
		return (false);
	return (true);
}

bool	valid_key(const std::string& key)
{
	std::string::const_iterator	it = key.begin();

	if (key.empty())
		return (false);
	while (it != key.end() && *it != '\f' && *it != '\t' && *it != '\v')
		it++;
	std::string::difference_type	len = it - key.begin();
	if (it != key.end() || len == 0 || len > 23)
		return (false);
	return (true);
}

std::vector<std::string>	parse_list(const std::string& list)
{
	std::vector<std::string>	items;
	std::string::const_iterator	start, current = list.begin();

	while (current != list.end())
	{
		start = current;
		while (current != list.end() && *current != ',')
			current++;
		items.push_back(std::string(start, current));
		if (current != list.end())
			current++;
	}
	return (items);
}
