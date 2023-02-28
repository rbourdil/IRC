#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <unistd.h>
#include <stdexcept>
#include <iostream>

#define BUFFSIZE 512
#define _EOF_ -1

class	scanner {

	int			_fd;
	char		_buff[BUFFSIZE];
	char*		_current;
	ssize_t		_count;

	public:

	scanner(int fd) :
		_fd(fd),
		_current(NULL),
		_count(0)
		{ }

	int	get(void)
	{
		if (_count == 0)
		{
			_count = read(_fd, _buff, BUFFSIZE);
			if (_count < 0)
				throw std::runtime_error("read failed");
			else if (_count == 0)
				return (_EOF_);
			_current = _buff;
		}
		_count--;
		return (*_current++);
	}

	void	unget(void)
	{
		if (_current >= _buff)
		{
			_current--;	
			_count++;
		}
	}

	size_t	count(void)
	{
		return (_current - _buff);
	}

};

#endif
