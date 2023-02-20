#include <iostream>
#include <cstdlib>

#include "lexer.hpp"

int	main(int argc, char *argv[])
{
	lexer	lex(argv[1]);
	token	current;

	while ((current = lex.get_token())._type != END_TOKEN && current._type != ERROR_TOKEN)
		std::cout << current << std::endl;
	return (0);
}
