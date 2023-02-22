#include "parser.hpp"

void	parser::match(int types)
{
	if (_current == ERROR_TOKEN)
	{
		std::cerr << "unrecognized token: " << _current._lexeme << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (_current == END_TOKEN)
	{
		std::cerr << "reached end of input" << std::endl;
		print_irc_cmd(_irc_cmd);
		exit(EXIT_FAILURE);
	}
	else if (_current == types)
		get_token();
	else
	{
		std::cerr << "unexpected token: " << _current._lexeme << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	parser::get_prefix(void)
{
	update_prefix();
	if (_current == SRV_NAME_GRP)
		match(SRV_NAME_GRP);
	else if (_current == NICK_GRP)
	{
		match(NICK_GRP);
		if (_current == EXCL_TOKEN)
		{
			merge_prefix();
			match(EXCL_TOKEN);
			merge_prefix();
			match(PARAM_TOKEN);
		}
		if (_current == AT_TOKEN)
		{
			merge_prefix();
			match(AT_TOKEN);
			merge_prefix();
			match(SRV_NAME_TOKEN);
		}
	}
	else
		match(ERROR_TOKEN);
	match(SPACE_TOKEN);
	get_cmd();
}

void	parser::get_cmd(void)
{
	update_cmd();
	match(CMD_L_TOKEN | CMD_D_TOKEN);
	if (_current == SPACE_TOKEN)
		get_params();
}

void	parser::get_params(void)
{
	match(SPACE_TOKEN);
	if (_current == COLON_TOKEN)
	{
		match(COLON_TOKEN);
		update_params();
		match(PARAM_GRP);
		while (_current == PARAM_GRP || _current == SPACE_TOKEN)
		{
			merge_params();
			match(PARAM_GRP | SPACE_TOKEN);
		}
	}
	else
	{
		update_params();
		match(PARAM_GRP);
		get_params();
	}
	get_crlf();
}

void	parser::get_crlf(void)
{
	match(CR_TOKEN);
	match(LF_TOKEN);
}

void	parser::parse(void)
{
	get_token();
	if (_current == COLON_TOKEN)
	{
		match(COLON_TOKEN);
		get_prefix();
	}
	else
		get_cmd();
}

// DEBUG
void	print_irc_cmd(const irc_cmd& cmd)
{
	std::cerr << "prefix:\t" << cmd._prefix << std::endl;
	std::cerr << "cmd:\t" << cmd._cmd << std::endl;
	std::cerr << "params:\t" << std::endl;
	std::vector<std::string>::const_iterator it = cmd._params.begin();
	for (; it != cmd._params.end(); it++)
		std::cerr << "\t" << *it << std::endl;
}


