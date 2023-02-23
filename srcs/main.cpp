#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "parser.hpp"

#define BUFSIZE 512

int	main(void)
{
	token	current;
	char	buff[BUFSIZE + 1];
	irc_cmd	out;
	parser	p;
	int		count;

	while ((count = read(STDIN_FILENO, buff, BUFSIZE)) > 0)
	{
		buff[count] = '\0';
		p.load(buff);
		p.parse();
		while (p.cmd_state() == COMPLETE_CMD || p.cmd_state() == DUMP_CMD)
		{
			if (p.cmd_state() == COMPLETE_CMD)
			{
				out = p.out();
				print_irc_cmd(out);
				p.reset();
				p.parse();
			}
		}
	}
	return (0);
}
