#ifndef BUFFER_HPP
#define BUFFER_HPP

struct Buffer {

	char*	_buff;
	char*	_current;

	Buffer(void)
	{
		_buff = new char[BUFSIZE];
		_current = _buff;
	}

	Buffer(const Buffer& from)
	{
		_buff = new char[BUFSIZE];		
		_current = _buff;
		for (ssize_t i = 0; i < from.size(); i++)
			*_current++ = from._buff[i];
	}

	Buffer&	operator=(const Buffer& from)
	{
		_current = _buff;
		append(from._buff, from.size());
		return (*this);
	}

	~Buffer(void)
	{
		delete[] _buff;
	}

	void	append(const char* buff, ssize_t count)
	{
		for (ssize_t i = 0; i < count; i++)
			*_current++ = buff[i];
	}

	ssize_t	size(void) const
	{
		return (_current - _buff);
	}

	void	reset(void)
	{
		_current = _buff;
	}

};

#endif
