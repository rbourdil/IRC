#ifndef LEXER_HPP
#define LEXER_HPP

#include "scanner.hpp"
#include "token.hpp"

#define CR 0xd
#define LF 0xa

class	lexer {

	scanner	_scan;

	public:

	lexer(const char* message) :
		_scan(message)
	{ }
	
	token	get_token(void);

};

#endif
