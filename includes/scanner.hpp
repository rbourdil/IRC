#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <unistd.h>
#include <stdexcept>
#include <iostream>

#include "headers.hpp"
#include "Buffer.hpp"

#define BUFFSIZE 512
#define _EOF_ -1

class	scanner {

	const char*	_start;
	const char*	_current;
	const char*	_end;

	public:

	scanner(const Buffer& buff) :
		_start(buff._buff),
		_current(buff._buff),
		_end(buff._current)
		{ }

	int	get(void)
	{
		if (_current == _end)
			return (_EOF_);
		return (*_current++);
	}

	void	unget(void)
	{
		_current--;	
	}

	size_t	count(void)
	{
		return (_current - _start);
	}

};

#endif
