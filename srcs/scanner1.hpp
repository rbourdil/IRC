#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>

class	scanner {

	const char*	_current;
	std::string	_buff;

	public:

	scanner(const char* message) :
		_current(message),
		_buff()
		{ }
	
	void	load(const char* message)
	{
		_current = message;
	}

	int	getc(void)
	{
		_buff.push_back(*_current);
		return (*_current++);
	}

	void	ungetc(void)
	{
		_buff.pop_back();
		_current--;
	}

	int	peek(void)
	{
		return (*_current);
	}

	std::string	flush(void)
	{
		std::string	tmp(_buff);
		_buff.clear();
		return (tmp);
	}

};

#endif
