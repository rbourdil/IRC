#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "lexer.hpp"

int	main(void)
{
	token	current;
	char	buff[512];
	int		cr = 0;

	while (fgets(buff, 512, stdin) != NULL)
	{
		lexer	lex(buff);
		while (1)
		{
		current = lex.get_token();
		if (current._type == ERROR_TOKEN)
		{
			std::cout << current << std::endl;
			exit(EXIT_SUCCESS);
		}
		else if (current._type == CR_TOKEN)
		{
			if (cr == 1)
				cr = 0;
			else
				cr = 1;	
		}
		else if (cr == 1 && current._type == LF_TOKEN)
		{
			std::cout << current << std::endl;
			exit(EXIT_SUCCESS);
		}
		else if (current._type == END_TOKEN)
			break;
		std::cout << current << std::endl;
		}
	}
	return (0);
}
