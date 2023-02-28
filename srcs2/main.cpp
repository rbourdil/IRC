#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include "parser.hpp"

#define BUFSIZE 512

void	print_irc_cmd(const irc_cmd& cmd)
{
	std::cerr << "prefix:\t" << cmd._prefix << std::endl;
	std::cerr << "cmd:\t" << cmd._cmd << std::endl;
	std::cerr << "params:" << std::endl;
	for (std::vector<std::string>::const_iterator it = cmd._params.begin(); it != cmd._params.end(); it++)
		std::cerr << "\t" << *it << std::endl;
}

int	main(void)
{
	token	current;
	irc_cmd	out;
	parser	p(0);

	p.parse();
	while (p.state() != STOP_PARSE)
	{
		if (p.state() == VALID_CMD)
		{
			out = p.out();
			print_irc_cmd(out);
		}
		else
		{
			p.skip_cmd();
			p.reset();
		}
		p.parse();
	}
	return (0);
}
