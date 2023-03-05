#include "parser.hpp"

void	parser::get_prefix(void)
{
	match(':');
	update_prefix();
	match(WORD_TOKEN);
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
		get_trailing();
	}
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
	if (_current == EOF_TOKEN)
	{
		_panic = true;
		return;
	}
	if (_current == ':')
		get_prefix();
	else if (_current != '\r')
		get_cmd();
	get_crlf();
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
