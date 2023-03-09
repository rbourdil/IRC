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
	}
	if (nb_of_params() == 14 && _current == ' ')
	{
		match(' ');
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
	match('\n');
}
	
void	parser::parse(void)
{
	if (_current == '\n')
	{
		get_crlf();
		_state = DUMP_CMD;
		return ;
	}
	if (_current == ':')
		get_prefix();
	else if (_current != '\n')
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
	if (isdigit(*it) || isspecial(*it))
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

bool	valid_mode(const std::string& mode)
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

bool	valid_channel(const std::string& channel)
{
	std::string::const_iterator	it = channel.begin();

	if (channel.empty())
		return (false);
	if (*it == '#' || *it == '+' || *it == '&' || *it == '!')
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
