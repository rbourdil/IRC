#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "parser.hpp"

int	main(void)
{
	token	current;
	char	buff[512];
	irc_cmd	out;

	while (fgets(buff, 512, stdin) != NULL)
	{
		parser	pars(buff);
		pars.parse();
		out = pars.out();
		print_irc_cmd(out);
	}
	return (0);
}
