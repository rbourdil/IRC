#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <unistd.h>
#include <stdexcept>
#include <iostream>

#include "headers.hpp"

#define BUFFSIZE 512
#define _EOF_ -1

class	scanner {

	const std::string&			_buff;
	std::string::const_iterator	_current;

	public:

	scanner(const std::string& buff) :
		_buff(buff),
		_current(buff.begin())
		{ }

	int	get(void)
	{
		if (_current == _buff.end())
			return (_EOF_);
		return (*_current++);
	}

	void	unget(void)
	{
		_current--;	
	}

	size_t	count(void)
	{
		return (_current - _buff.begin());
	}

};

#endif
