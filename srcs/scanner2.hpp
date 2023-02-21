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
	
	int	getc(void)
	{
		return (*_current++);
	}

	void	addc(int c)
	{
		_buff.push_back(c);
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
